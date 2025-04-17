/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file HardwareSerial.cpp
 * @brief Wirish serial port implementation.
 */

#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#if defined(__M460__)
#if defined(__M460MINIMA__)
    #define RXBUFSIZE           512 /* RX buffer size */
    #define TXBUFSIZE           512 /* TX buffer size */
    uint16_t gCtrlSignal = 0;     /* BIT0: DTR(Data Terminal Ready) , BIT1: RTS(Request To Send) */
    STR_VCOM_LINE_CODING gLineCoding = {115200, 0, 0, 8};   /* Baud rate : 115200    */
    volatile uint16_t comRbytes = 0;
    volatile uint16_t comRhead = 0;
    volatile uint16_t comRtail = 0;
    volatile uint16_t comTbytes = 0;
    volatile uint16_t comThead = 0;
    volatile uint16_t comTtail = 0;
    uint32_t gu32RxSize = 0;
    uint32_t gu32RxCount = 0;
    uint32_t gu32TxSize = 0;
    volatile uint8_t comRbuf[RXBUFSIZE] __attribute__((aligned(4)));
    volatile uint8_t comTbuf[TXBUFSIZE]__attribute__((aligned(4)));
    uint8_t gRxBuf[RXBUFSIZE] __attribute__((aligned(4))) = {0};
    uint8_t gUsbRxBuf[RXBUFSIZE] __attribute__((aligned(4))) = {0};
    volatile int8_t gi8BulkOutReady = 0;
#endif
//For M460Minima board, map Serial1 to VCOM
#if defined(__M460MINIMA__)//VCOM, UART0, UART1
    #if(UART_MAX_COUNT>0)
    ring_buffer rx_buffer= { { 0 }, 0, 0};
    HardwareSerial Serial(HSUSBD, UART_Desc[UART_DESC_IDX0].modulenum, &rx_buffer);//VCOM
    #endif
	#if(UART_MAX_COUNT>1)
    ring_buffer rx_buffer1 = { { 0 }, 0, 0};
    HardwareSerial Serial1(UART_Desc[1].U, UART_DESC_IDX1 , UART_Desc[1].clksrcsel, UART_Desc[1].clkdiv, UART_Desc[1].irq, UART_Desc[1].modulenum, &rx_buffer1);//UART0
    #endif
	#if(UART_MAX_COUNT>2)
    ring_buffer rx_buffer2 = { { 0 }, 0, 0};
    HardwareSerial Serial2(UART_Desc[2].U, UART_DESC_IDX2, UART_Desc[2].clksrcsel, UART_Desc[2].clkdiv, UART_Desc[2].irq, UART_Desc[2].modulenum, &rx_buffer2);//UART1
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if defined(__M460MINIMA__)//VCOM, UART0, UART1
#if(UART_MAX_COUNT>1)
void UART0_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART0->DAT;
            rx_buffer1.head = i;
        }
		else//i == rx_buffer1.tail
		{
			unsigned char dummy = UART0->DAT;
		}
    }

}
#endif  

#if(UART_MAX_COUNT>2)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer2.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer2.tail)
        {
            rx_buffer2.buffer[rx_buffer2.head] = UART1->DAT;
            rx_buffer2.head = i;
        }
		else//i == rx_buffer1.tail
		{
			unsigned char dummy = UART1->DAT;
		}
    }

}
#endif
 
#else//defined(__M460MINIMA__), UART0, UART1, UART4(dummy)


#endif//defined(__M460MINIMA__)

#ifdef __cplusplus
}
#endif

#endif

#if(UART_MAX_COUNT>0)
    void serialEvent() __attribute__((weak));
#endif

#if(UART_MAX_COUNT>1)
    void serial1Event() __attribute__((weak));
#endif

#if defined(__M467SJHAN__)
#if(UART_MAX_COUNT>2)
  void serial2Event() __attribute__((weak));
#endif
#endif//defined(__M467SJHAN__)

void serialEventRun(void)
{
#if(UART_MAX_COUNT>0)
    if (Serial.available()) serialEvent();
#endif

#if(UART_MAX_COUNT>1)
    if (Serial1.available()) serial1Event();
#endif

#if defined(__M467SJHAN__)
#if(UART_MAX_COUNT>2)
   if (Serial2.available()) serial2Event();
#endif
#endif//defined(__M467SJHAN__)
}

HardwareSerial::HardwareSerial(UART_T *uart_device,
                               uint32_t u32Idx,
                               uint32_t u32ClkSrc,
                               uint32_t u32ClkDiv,
                               IRQn_Type u32IrqId,
							   uint32_t u32ModuleNum,
                               ring_buffer *rx_buffer)
{
    this->use_vcom = 0;
    this->uart_device = uart_device;
    this->u32Idx = u32Idx;
    this->u32ClkSrc = u32ClkSrc;
    this->u32ClkDiv = u32ClkDiv;
    this->u32IrqId = u32IrqId;
	this->u32ModuleNum = u32ModuleNum;
    this->_rx_buffer = rx_buffer;
    
}

#if defined(__M460MINIMA__)
HardwareSerial::HardwareSerial(VCOM_T *vcom_device, uint32_t u32ModuleNum, ring_buffer *rx_buffer)
{
    this->use_vcom = 1;
    this->vcom_device = vcom_device;
	this->u32ModuleNum = u32ModuleNum;
    this->_rx_buffer = rx_buffer;
    this->vcom_init_done = 0;
}
#endif
/*
 * Set up/tear down
 */
//For RS485 support
void HardwareSerial::begin(uint32_t baud, uint16_t config)
{
	
	this->u16config = config;
	begin(baud);
} 
 
 
void HardwareSerial::begin(uint32_t baud)
{

	if(u32ModuleNum == UART_USE_VCOM)
	{
	   if( use_vcom == 1)//Vcom object
       {
           if( vcom_init_done == 0 ) 
           {
#if defined(__M460MINIMA__)           
                VcomBegin(baud);
#endif                
                vcom_init_done = 1;//
           }
       }
       return;	   
	}
    else if(u32ModuleNum == UART_USE_NOUSE)
	{
		return;
	}
	else
	{
		//u32ModuleNum = UART_USE_UART0, UART_USE_UART1... etc
       UART_Config(UART_Desc[u32Idx]);
	   
	   //RS485-AUD
	   if(u16config==RS485_OVER_SERIAL)
		   DE_Config(DEPin_Desc[u32Idx]);
	   
       SYS_UnlockReg();		
	}
	
#if defined(__M460__)

	/* Enable IP clock */
    CLK_EnableModuleClock(UART_Desc[u32Idx].module);

    /* Select IP clock source and clock divider */
    if(uart_device == UART0)
	{
		CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART0(u32ClkDiv));
	}
    else if(uart_device == UART1)
	{
		CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART1(u32ClkDiv));
	}
	else if(uart_device == UART3)
	{	
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV4_UART3(u32ClkDiv));
	}
	else if(uart_device == UART4)
	{	
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV4_UART4(u32ClkDiv));
	}   
	/* Reset IP */
    //SYS_ResetModule(UART_Desc[u32Idx].module);

    /* Enable Interrupt */
    UART_ENABLE_INT(uart_device, UART_INTEN_RDAIEN_Msk);
    NVIC_EnableIRQ(u32IrqId);

    /* Configure UART and set UART Baudrate */
    UART_Open(uart_device, baud);   
	
	if(u16config==RS485_OVER_SERIAL)
	{
        UART_SelectRS485Mode(uart_device, UART_ALTCTL_RS485AUD_Msk, 0);		
		/* Set RTS pin active level as high level active */
        uart_device->MODEM = (uart_device->MODEM & (~UART_MODEM_RTSACTLV_Msk)) | UART_RTS_IS_HIGH_LEV_ACTIVE;
	}
	    

#endif//#if defined(__M451__)
    SYS_LockReg();

}

void HardwareSerial::end(void)
{
    if(u32ModuleNum == UART_USE_VCOM)
    {
		this->vcom_init_done = 0;//[2025-03-03]To hook VCOM in before setup function. Default baud is 115200.
		                         //in case someone want to change baudrate. Call end, then do begin(default_BR)   
		return;
	}
    else if(u32ModuleNum == UART_USE_NOUSE)
	{
		return;
	}
	else
	{
		//u32ModuleNum = UART_USE_UART0, UART_USE_UART1... etc
        UART_Close(uart_device);
	}	
}	
/*
 * I/O
 */

int HardwareSerial::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer->head == _rx_buffer->tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
        _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % SERIAL_BUFFER_SIZE;
#if defined(__M460MINIMA__)  
        if(u32ModuleNum == UART_USE_VCOM)
        {
            gu32RxCount--;
	    }
#endif		
		return c;
    }
}

int HardwareSerial::availableForWrite(void)
{
    /* For M460 */
#if defined(__M460__)
    if(u32ModuleNum == UART_USE_VCOM)
    {
		/*
            To do VCOM availability
        */
#if defined(__M460MINIMA__)          
        return ((int)(EPA_MAX_PKT_SIZE) - (int)(vcom_device->EP[EPA].EPDATCNT & 0xffff));
#else
        return 0;
#endif        
	}
    else if(u32ModuleNum == UART_USE_NOUSE)
	{
		return -1;
	}
	else//u32ModuleNum = UART_USE_UART0, UART_USE_UART1... etc
	{
		if (UART_IS_TX_FULL(uart_device))
        {
            return 0;
        }
        else
        {
            if(uart_device == UART0)
                return (UART0_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
            else if(uart_device == UART1)
                return (UART1_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
		    else if(uart_device == UART3)
                return (UART3_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
            else if(uart_device == UART4)
                return (UART4_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
        }
	}

#endif

}

int HardwareSerial::available(void)
{
    if(u32ModuleNum == UART_USE_VCOM)
    {
#if defined(__M460MINIMA__)         
		return (int)(gu32RxCount);
#else
        return 0;
#endif        
	}
    else if(u32ModuleNum == UART_USE_NOUSE)
	{
		return -1;
	}
	else
	{
		//u32ModuleNum = UART_USE_UART0, UART_USE_UART1... etc
        return (unsigned int)(SERIAL_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % SERIAL_BUFFER_SIZE;
	}	  
}

size_t HardwareSerial::write(const uint8_t ch)
{
#if defined(__M460__)
	if(u32ModuleNum == UART_USE_VCOM)
    {
#if defined(__M460MINIMA__) 
		/*To do:VCOM Tx, single byte TX*/
        vcom_device->EP[EPA].EPDAT_BYTE = ch;
        vcom_device->EP[EPA].EPRSPCTL = HSUSBD_EP_RSPCTL_SHORTTXEN;    // packet end
        vcom_device->EP[EPA].EPTXCNT = 1;
        HSUSBD_ENABLE_EP_INT(EPA, HSUSBD_EPINTEN_INTKIEN_Msk);
#endif        
	}
    else if(u32ModuleNum == UART_USE_NOUSE)
	{
		return -1;
	}
	else
	{
		//u32ModuleNum = UART_USE_UART0, UART_USE_UART1... etc
        while (uart_device->FIFOSTS & UART_FIFOSTS_TXFULL_Msk);
        uart_device->DAT = ch;
	}	  
    return 1;

#endif
}

void HardwareSerial::flush(void)
{

    /* No need to implement because we use hardware UART and it is with hardware FIFO*/
}


int HardwareSerial::peek(void)
{
    if (_rx_buffer->head == _rx_buffer->tail)
    {
        return -1;
    }
    else
    {
        return _rx_buffer->buffer[_rx_buffer->tail];
    }
}
