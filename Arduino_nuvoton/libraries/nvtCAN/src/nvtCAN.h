/*
    nvt_can.h
    2022 Copyright (c) Nuvoton Technology Inc.

    Author: 
    2022-12-12

    Contributor:

    

    The MIT License (MIT)

    2022 Copyright (c) Nuvoton Technology Inc.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef _NVTCAN_H_
#define _NVTCAN_H_



#include "Arduino.h"
#include "Pins_arduino.h"
#include "mcp_can.h"
#include "nvtCAN_dfs.h"




#define MAX_CHAR_IN_MESSAGE 8
class nvtCAN //: public MCP_CAN
{
public:
    //nvt_can(byte _CS) : MCP_CAN(_CS), nReservedTx(0){};
    //nvt_can(void *_can,uint32_t module,IRQn_Type _id, void(*_initCb)(void));
    
    nvtCAN(byte _CANSEL);// :nReservedTx(0){};
    /*
        Nuvoton CAN controller(ccan) driver function
    */
public:
    //virtual void enableTxInterrupt(bool enable = true); // enable transmit interrupt
    //virtual void reserveTxBuffers(byte nTxBuf = 0);
//    {
//       nReservedTx = (nTxBuf < MCP_N_TXBUFFERS ? nTxBuf : MCP_N_TXBUFFERS - 1);
//    }
 //   virtual byte getLastTxBuffer();
//    {
//        return MCP_N_TXBUFFERS - 1; // read index of last tx buffer
//    }
    virtual byte begin(uint32_t speedset, const byte clockset = MCP_16MHz);                                                                                 // init can
    //virtual byte init_Mask(byte num, byte ext, unsigned long ulData);                                                                                   // init Masks
    //virtual byte init_Filt(byte num, byte ext, unsigned long ulData);                                                                                   // init filters
    //virtual void setSleepWakeup(byte enable);                                                                                                           // Enable or disable the wake up interrupt (If disabled the ncan will not be woken up by CAN bus activity, making it send only)
    //virtual byte sleep();                                                                                                                               // Put the ncan in sleep mode
    //virtual byte wake();                                                                                                                                // Wake ncan manually from sleep
    //virtual byte setMode(byte opMode);                                                                                                                  // Set operational mode
    //virtual byte getMode();                                                                                                                             // Get operational mode
    //virtual byte checkError(uint8_t* err_ptr = NULL);                                                                                                   // if something error

    virtual byte checkReceive(void);                                                                                                                    // if something received
    //virtual byte readMsgBufID(byte status, volatile unsigned long *id, volatile byte *ext, volatile byte *rtr, volatile byte *len, volatile byte *buf); // read buf with object ID
    /* wrapper */
    //byte readMsgBufID(unsigned long *ID, byte *len, byte *buf) {
    //    return readMsgBufID(readRxTxStatus(), ID, &ext_flg, &rtr, len, buf);
    //}
    //byte readMsgBuf(byte *len, byte *buf) {
    //    return readMsgBufID(readRxTxStatus(), &can_id, &ext_flg, &rtr, len, buf);
    //}

    //virtual byte trySendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf, byte iTxBuf = 0xff);                                 // as sendMsgBuf, but does not have any wait for free buffer
    virtual byte sendMsgBuf(byte status, unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte *buf);                                  // send message buf by using parsed buffer status
    virtual byte sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf, bool wait_sent = true);                                 // send buf


    //virtual void clearBufferTransmitIfFlags(byte flags = 0);                                                                                            // Clear transmit flags according to status
    //virtual byte readRxTxStatus(void);                                                                                                                  // read has something send or received
    //virtual byte checkClearRxStatus(byte *status);                                                                                                      // read and clear and return first found rx status bit
    //virtual byte checkClearTxStatus(byte *status, byte iTxBuf = 0xff);                                                                                  // read and clear and return first found or buffer specified tx status bit
    //virtual bool mcpPinMode(const byte pin, const byte mode);                                                                                           // switch supported pins between HiZ, interrupt, output or input
    //virtual bool mcpDigitalWrite(const byte pin, const byte mode);                                                                                      // write HIGH or LOW to RX0BF/RX1BF
    //virtual byte mcpDigitalRead(const byte pin);

private:
    void ncan_reset(void); // reset ncan

    //byte ncan_readRegister(const byte address); // read ncan's register

    //void ncan_readRegisterS(const byte address,
    //                           byte values[],
    //                           const byte n);
    //void ncan_setRegister(const byte address, // set ncan's register
    //                         const byte value);

    //void ncan_setRegisterS(const byte address, // set ncan's registers
    //                          const byte values[],
    //                          const byte n);

    //void ncan_initCANBuffers(void);

    //void ncan_modifyRegister(const byte address, // set bit of one register
    //                            const byte mask,
    //                            const byte data);

    //byte ncan_readStatus(void);                                  // read ncan's Status
    //byte ncan_setCANCTRL_Mode(const byte newmode);               // set mode
    //byte ncan_requestNewMode(const byte newmode);                // Set mode
    byte ncan_configRate(const uint32_t canSpeed, const byte clock); // set baudrate
    byte ncan_init(const byte canSpeed, const byte clock);       // ncan init

    //void ncan_write_id(const byte mcp_addr, // write can id
    //                      const byte ext,
    //                      const unsigned long id);

    //void ncan_read_id(const byte mcp_addr, // read can id
    //                     byte *ext,
    //                     unsigned long *id);

    void ncan_write_canMsg(const byte buffer_sidh_addr, unsigned long id, byte ext, byte rtr, byte len,
                              volatile const byte *buf); // read can msg
    //void ncan_read_canMsg(const byte buffer_load_addr, volatile unsigned long *id, volatile byte *ext,
    //                         volatile byte *rtr, volatile byte *len, volatile byte *buf); // write can msg
    //void ncan_start_transmit(const byte mcp_addr);                                     // start transmit
    //byte ncan_getNextFreeTXBuf(byte *txbuf_n);                                         // get Next free txbuf
    //byte ncan_isTXBufFree(byte *txbuf_n, byte iBuf);                                   // is buffer by index free

    /*
        can operator function
    */

    byte sendMsg(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf, bool wait_sent = true); // send message
private:
    byte nReservedTx; // Count of tx buffers for reserved send

	union{
		void *_vncan;
		CAN_T *ncan;

	};
	uint32_t module;
    byte nCANSel;
	IRQn_Type id;
};


byte BaudRateCheck(uint32_t u32BaudRate, uint32_t u32RealBaudRate);
static void CAN_0_Init(void);
#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
