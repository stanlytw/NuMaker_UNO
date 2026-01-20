/*
SoftwareSerial.cpp (formerly NewSoftSerial.cpp) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.

************************* 
*2018/11/16: ported from the SoftwareSerial for Nuvoton MCU NUC131: 28800 bps OK @50MHz CPU. 
*/

// When set, _DEBUG co-opts pins 11 and 13 for debugging with an
// oscilloscope or logic analyzer.  Beware: it also slightly modifies
// the bit times, so don't rely on it too much at high baud rates

// 
// Includes
// 
#include <Arduino.h>
#include <nvtSoftwareSerial.h>

#define GPIO_BASE (GPIOA_BASE)
//
// Statics
//
SoftwareSerial *SoftwareSerial::active_object = 0;
uint8_t SoftwareSerial::_receive_buffer[_SS_MAX_RX_BUFF]; 
volatile uint8_t SoftwareSerial::_receive_buffer_tail = 0;
volatile uint8_t SoftwareSerial::_receive_buffer_head = 0;

extern volatile uint8_t g_u8Softserail_enable;
extern volatile uint8_t g_u8Softserail_port_num;
extern volatile uint8_t g_u8Softserail_pin_num;
extern GPIO_T* g_u8Softserail_port_base;

//
// Debugging
//
// This function generates a brief pulse
// for debugging or measuring on an oscilloscope.

static void BSP_TimerDelaySetting(uint32_t u32DelayUs)
{    
    
#if defined(__M460__) 
    SYS_UnlockReg();
    
    #if (SOFTWARE_UART_TIMER_SELECT==(0))
        CLK_EnableModuleClock(TMR0_MODULE);
        CLK_SetModuleClock(TMR0_MODULE,CLK_CLKSEL1_TMR0SEL_HIRC, 0);
    #elif (SOFTWARE_UART_TIMER_SELECT==(1))
        CLK_EnableModuleClock(TMR1_MODULE);
        CLK_SetModuleClock(TMR1_MODULE,CLK_CLKSEL1_TMR1SEL_HIRC, 0);
    #elif (SOFTWARE_UART_TIMER_SELECT==(2))
        CLK_EnableModuleClock(TMR2_MODULE);
        CLK_SetModuleClock(TMR2_MODULE,CLK_CLKSEL1_TMR2SEL_HIRC, 0);
    #elif (SOFTWARE_UART_TIMER_SELECT==(3))
        CLK_EnableModuleClock(TMR3_MODULE);
        CLK_SetModuleClock(TMR3_MODULE,CLK_CLKSEL1_TMR3SEL_HIRC, 0); 
    #else
        #error "SOFTWARE_UART_TIMER def error!"
    #endif
           
    SYS_LockReg();
   

#endif
}

static inline void BPS_delay() //using systick
{
    SOFTWARE_UART_TIMER->INTSTS = TIMER_INTSTS_TIF_Msk;
    SOFTWARE_UART_TIMER->CTL |= TIMER_CTL_CNTEN_Msk;
        
    while(SOFTWARE_UART_TIMER->CTL & TIMER_CTL_ACTSTS_Msk);
    while(!(SOFTWARE_UART_TIMER->INTSTS & TIMER_INTSTS_TIF_Msk));

}

static inline uint32_t digitalPinToPort(uint32_t pin_num)
{
    return ((uint32_t)(GPIO_Desc[BoardToPinInfo[pin_num].pin].P - (uint32_t)GPIO_BASE)/0x40);
}

static inline uint32_t PortToPortNum(GPIO_T* port)
{
    return ((uint32_t)((uint32_t)port - (uint32_t)GPIO_BASE)/0x40);
}

static inline uint32_t digitalPinToPinNum(uint32_t tx)
{   
    /*calculate the pin number of Digital Pin*/
    for(int i=0 ; i<16 /* GPIO pin maximum */; i++)
    {  
        if((GPIO_Desc[BoardToPinInfo[tx].pin].bit>>i)&0x1) return i;
    }
    
    return 0xDEADBEEF; //error
}


static inline uint32_t BaudSetbyTimer(uint32_t u32Baud)
{
    uint32_t u32Clk = TIMER_GetModuleClock(SOFTWARE_UART_TIMER);
    uint32_t u32Cmpr = 0UL, u32Prescale = 0UL;
    
    /* Fastest possible timer working freq is (u32Clk / 2). While cmpr = 2, prescaler = 0. */
    if(u32Baud > (u32Clk / 2UL))
    {
        u32Cmpr = 2UL;
    }
    else
    {
        u32Cmpr = u32Clk / u32Baud;
        u32Prescale = (u32Cmpr >> 24);  /* for 24 bits CMPDAT */
        if (u32Prescale > 0UL)
            u32Cmpr = u32Cmpr / (u32Prescale + 1UL);
    }

    SOFTWARE_UART_TIMER->CTL = TIMER_ONESHOT_MODE | u32Prescale;
    SOFTWARE_UART_TIMER->CMP = u32Cmpr;

    return(u32Clk / (u32Cmpr * (u32Prescale + 1UL)));
}


//
// Private methods
//

/* static */ 
inline void SoftwareSerial::tunedDelay() { 
   BPS_delay();
}

// This function sets the current object as the "listening"
// one and returns true if it replaces another 
bool SoftwareSerial::listen()
{ 
  
  if (active_object != this)
  {
    if (active_object)
      active_object->stopListening();

    _buffer_overflow = false;
    _receive_buffer_head = _receive_buffer_tail = 0;
    active_object = this;
    
    //to fit Arduino Framework.: 
    //The following global will referenced by attachInterrupt API.
    //They must be set before calling attachInterrupt API.
    g_u8Softserail_enable = 1;
    g_u8Softserail_pin_num = this->_receivePinNum;
    g_u8Softserail_port_num = PortToPortNum(this->_pu32ReceivePort);;
    g_u8Softserail_port_base = _pu32ReceivePort;
    
    
    attachInterrupt(this->_receiveBoardPin , 
                    this->handle_interrupt, 
                    FALLING);  
   
    

    return true;
  } 
     
  return false;
  
}

// Stop listening. Returns true if we were actually listening.
bool SoftwareSerial::stopListening()
{  
  if (active_object == this)
  {
    detachInterrupt(this->_receiveBoardPin);
    active_object = NULL;

    return true;
  }
  return false;
}

//
// The receive routine called by the interrupt handler
//

void SoftwareSerial::recv()
{
    //nvtEthernet_printf("SoftwareSerial recv\r\n");
    if(GPIO_GET_INT_FLAG(this->_pu32ReceivePort,this->_receiveBitMask)) //RX
    {
        uint8_t u8RcvByteTmp=0;

        if (_inverse_logic ? !rx_pin_read() : rx_pin_read())
        {
            //START bit
		//	nvtEthernet_printf("SoftwareSerial recv error\r\n");
			//nvtEthernet_printf("_inverse_logic = %d \r\n", _inverse_logic);
			//nvtEthernet_printf("rx_pin_read() = %d \r\n", rx_pin_read());
            return; //error
        } else 
		{ //START bit
            uint32_t bit_cnt;
            uint32_t idx_next=(_receive_buffer_tail+1)%_SS_MAX_RX_BUFF;
            setRxIntMsk(false);
            
            tunedDelay();
            for(bit_cnt=0 ; bit_cnt < 8 ; bit_cnt++) {
				//nvtEthernet_printf("rx_pin_read() = %d \r\n", rx_pin_read());
                u8RcvByteTmp |= (rx_pin_read()?1:0)<<bit_cnt;
				//nvtEthernet_printf("%d th SoftwareSerial rxing u8RcvByteTmp = 0x%x\r\n", bit_cnt, u8RcvByteTmp);
                tunedDelay();
            }
            //nvtEthernet_printf("Final SoftwareSerial rxing u8RcvByteTmp = 0x%x\r\n", u8RcvByteTmp);
            //nvtEthernet_printf("Final SoftwareSerial str   u8RcvByteTmp = %s\r\n", u8RcvByteTmp);			
            //if (_inverse_logic)
            //    u8RcvByteTmp = ~u8RcvByteTmp;
            
            if(idx_next!=_receive_buffer_head){
                _receive_buffer[idx_next] = u8RcvByteTmp;
                _receive_buffer_tail=idx_next;
            } else {
                _buffer_overflow=true;
            }
            
            GPIO_CLR_INT_FLAG(this->_pu32ReceivePort, this->_receiveBitMask);
            setRxIntMsk(true);
        }
    }

}



uint8_t SoftwareSerial::rx_pin_read()
{
  return *(this->_p_receive_pin);
}

//
// Hook Interrupt Callback
//

/* static */

inline void SoftwareSerial::handle_interrupt()
{
  if (active_object)
  {
    active_object->recv();
  }
}

//
// Constructor
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic) : 
  _buffer_overflow(false),
  _inverse_logic(inverse_logic)
{
    //Do pin assignment in constructor. 
	//Do GPIO settings in begin() method.
	_RXPin = receivePin;
    _TXPin = transmitPin;

}


//
// Destructor
//

SoftwareSerial::~SoftwareSerial()
{
  end();
}

void SoftwareSerial::setTX(uint8_t tx)
{
  // First write, then set output. If we do this the other way around,
  // the pin would be output low for a short while before switching to
  // output high. Now, it is input with pullup for a short while, which
  // is fine. With inverse logic, either order is fine.
  digitalWrite(tx, _inverse_logic ? LOW : HIGH);
  //digitalWrite(tx, LOW);
  pinMode(tx, OUTPUT);
 
  _transmitBitMask = GPIO_Desc[BoardToPinInfo[tx].pin].bit;
  uint8_t port = digitalPinToPort(tx);
  _transmitPinNum = digitalPinToPinNum(tx);
  _p_transmit_pin = ((volatile uint32_t *)((GPIO_PIN_DATA_BASE+(0x40*(port))) + ((_transmitPinNum)<<2)));
  _pu32TransmitPort = GPIO_Desc[BoardToPinInfo[tx].pin].P;
  _transmitBoardPin = tx;

}

void SoftwareSerial::setRX(uint8_t rx)
{
  pinMode(rx, INPUT);
  if (!_inverse_logic)
    digitalWrite(rx, HIGH);  // pullup for normal logic!
  _receivePinNum = digitalPinToPinNum(rx);
  _receiveBitMask = GPIO_Desc[BoardToPinInfo[rx].pin].bit;
  
  uint8_t port = digitalPinToPort(rx);
  _p_receive_pin = ((volatile uint32_t *)((GPIO_PIN_DATA_BASE+(0x40*(port))) + ((_receivePinNum)<<2)));
  _pu32ReceivePort = GPIO_Desc[BoardToPinInfo[rx].pin].P;
  _receiveBoardPin = rx;

}

uint16_t SoftwareSerial::subtract_cap(uint16_t num, uint16_t sub) {
  if (num > sub)
    return num - sub;
  else
    return 1;
}

//
// Public methods
//

void SoftwareSerial::begin(long speed)
{
  
  //Select Timer for buadrate generator   
  BSP_TimerDelaySetting(0);
    
  //Configure timer reg to get desired baudrate.
  uint32_t ss = BaudSetbyTimer((uint32_t)(speed));
  //nvtEthernet_printf("\n\n ss = %d \r\n", ss);
    
  // 12 (gcc 4.8.2) or 13 (gcc 4.3.2) cycles from start bit to first bit,
  // 15 (gcc 4.8.2) or 16 (gcc 4.3.2) cycles between bits,
  // 12 (gcc 4.8.2) or 14 (gcc 4.3.2) cycles from last bit to stop bit
  // These are all close enough to just use 15 cycles, since the inter-bit
  // timings are the most critical (deviations stack 8 times)
  //_tx_delay = subtract_cap(bit_delay, 15 / 4);

  // Only setup rx when we have a valid PCINT for this pin

  setTX(_TXPin);
  setRX(_RXPin);   
  listen();
  uint8_t _transmitPinNum;
}

void SoftwareSerial::setRxIntMsk(bool enable)
{
    if (enable) {
      GPIO_EnableInt( this->_pu32ReceivePort, 
                      this->_receivePinNum, 
                      GPIO_INT_FALLING);
    } else {
      GPIO_DisableInt(this->_pu32ReceivePort, this->_receivePinNum);;
    }

}


void SoftwareSerial::end()
{
  stopListening();
}


// Read data from buffer

int SoftwareSerial::read()
{

  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  uint32_t idx_fetch = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  uint8_t d = _receive_buffer[idx_fetch]; // grab next byte
  _receive_buffer_head = idx_fetch;
  return d;
}

int SoftwareSerial::available()
{
  if (!isListening())
    return 0;
  return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
}



size_t SoftwareSerial::write(uint8_t b)
{

    bool inv = _inverse_logic;
    
    if (inv)
        b = ~b;

    __set_PRIMASK(1); // turn off interrupts for a clean txmit


    if (inv)
	{
        *(this->_p_transmit_pin)=1; //START bit

    }
	else
	{
        *(this->_p_transmit_pin)=0; //START bit

    }
    tunedDelay();

    for(uint8_t i=0 ;i < 8; i++) {
        *(this->_p_transmit_pin) = (b >> i) & 0x1;

        tunedDelay();
    }
    
    // restore pin to natural state
    if (inv)
        *(this->_p_transmit_pin)=0;
    else
        *(this->_p_transmit_pin)=1;
    
    __set_PRIMASK(0); // turn interrupts back on

    tunedDelay();
    
  return 1;
}



void SoftwareSerial::flush()
{
  // There is no tx buffering, simply return
}



int SoftwareSerial::peek()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  uint32_t idx_peek = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;

  // Read from "head"
  return _receive_buffer[idx_peek];
}


