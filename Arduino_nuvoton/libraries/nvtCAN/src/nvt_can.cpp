/*
    nvt_can.cpp
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
#define DEBUG_EN 0
#include "nvt_can.h"

//#define spi_readwrite      pSPI->transfer
//#define spi_read()         spi_readwrite(0x00)
//#define spi_write(spi_val) spi_readwrite(spi_val)
//#define SPI_BEGIN()        pSPI->beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0))
//#define SPI_END()          pSPI->endTransaction()





/*********************************************************************************************************
** Function name:           nvt_can(constructor)
** Descriptions:            reset the device
*********************************************************************************************************/
 nvt_can::nvt_can(byte _CANSEL) {
    //Add NUC131 standard driver code to reset and enable CAN IP

 }





/*********************************************************************************************************
** Function name:           mcp2515_reset
** Descriptions:            reset the device
*********************************************************************************************************/
void nvt_can::ncan_reset(void) {
    //Add NUC131 standard driver code to reset and enable CAN IP
    delay(10);
}


/*********************************************************************************************************
** Function name:           mcp2515_configRate
** Descriptions:            set baudrate
*********************************************************************************************************/
byte nvt_can::ncan_configRate(const byte canSpeed, const byte clock) {
   
   
}


/*********************************************************************************************************
** Function name:           mcp2515_write_canMsg
** Descriptions:            write msg
**                          Note! There is no check for right address!
*********************************************************************************************************/
void nvt_can::ncan_write_canMsg(const byte buffer_sidh_addr, unsigned long id, byte ext, byte rtrBit, byte len,
                                   volatile const byte* buf) {
    byte load_addr = txSidhToTxLoad(buffer_sidh_addr);

    byte tbufdata[4];
    byte dlc = len | (rtrBit ? MCP_RTR_MASK : 0) ;
    byte i;

    mcp2515_id_to_buf(ext, id, tbufdata);

    #ifdef SPI_HAS_TRANSACTION
    SPI_BEGIN();
    #endif
    MCP2515_SELECT();
    spi_readwrite(load_addr);
    for (i = 0; i < 4; i++) {
        spi_write(tbufdata[i]);
    }
    spi_write(dlc);
    for (i = 0; i < len && i < CAN_MAX_CHAR_IN_MESSAGE; i++) {
        spi_write(buf[i]);
    }

    MCP2515_UNSELECT();
    #ifdef SPI_HAS_TRANSACTION
    SPI_END();
    #endif

    mcp2515_start_transmit(buffer_sidh_addr);

}



/*********************************************************************************************************
** Function name:           begin
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvt_can::begin(uint32_t speedset, const byte clockset) {
    
    byte res = ncan_init((byte)speedset, clockset);

    return res;
}

/*********************************************************************************************************
** Function name:           enableTxInterrupt
** Descriptions:            enable interrupt for all tx buffers
*********************************************************************************************************/
void nvt_can::enableTxInterrupt(bool enable) {
   
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvt_can::sendMsgBuf(byte status, unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte* buf) {
    byte txbuf_n = statusToTxSidh(status);

    if (txbuf_n == 0) {
        return CAN_FAILTX;    // Invalid status
    }

    mcp2515_modifyRegister(MCP_CANINTF, status, 0);  // Clear interrupt flag
    mcp2515_write_canMsg(txbuf_n, id, ext, rtrBit, len, buf);

    return CAN_OK;
}



/*********************************************************************************************************
** Function name:           sendMsg
** Descriptions:            send message
*********************************************************************************************************/
byte nvt_can::sendMsg(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf, bool wait_sent) {
    byte res, res1, txbuf_n;
    uint16_t uiTimeOut = 0;

    can_id = id;
    ext_flg = ext;
    rtr = rtrBit;

    do {
        if (uiTimeOut > 0) {
            delayMicroseconds(10);
        }
        res = mcp2515_getNextFreeTXBuf(&txbuf_n);                       // info = addr.
        uiTimeOut++;
    } while (res == MCP_ALLTXBUSY && (uiTimeOut < TIMEOUTVALUE));

    if (uiTimeOut == TIMEOUTVALUE) {
        return CAN_GETTXBFTIMEOUT;                                      // get tx buff time out
    }
    mcp2515_write_canMsg(txbuf_n, id, ext, rtrBit, len, buf);

    if (wait_sent) {
        uiTimeOut = 0;
        do {
            if (uiTimeOut > 0) {
                delayMicroseconds(10);
            }
            uiTimeOut++;
            res1 = mcp2515_readRegister(txbuf_n - 1);  // read send buff ctrl reg
            res1 = res1 & 0x08;
        } while (res1 && (uiTimeOut < TIMEOUTVALUE));

        if (uiTimeOut == TIMEOUTVALUE) {                                     // send msg timeout
            return CAN_SENDMSGTIMEOUT;
        }
    }

    return CAN_OK;

}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            send buf
*********************************************************************************************************/
byte nvt_can::sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf, bool wait_sent) {
    return sendMsg(id, ext, rtrBit, len, buf, wait_sent);
}



/*********************************************************************************************************
** Function name:           checkReceive
** Descriptions:            check if got something
*********************************************************************************************************/
byte nvt_can::checkReceive(void) {
    byte res;
    //res = mcp2515_readStatus();                                         // RXnIF in Bit 1 and 0
    return res;
}




#if CAN_MAX_COUNT > 0
#define CAN_IDX 
static void CAN0_Init(void) {	
	CAN_Config(CAN_Desc[CAN_IDX]);	
}
//nvt_can SPI(SPI_Desc[SPI_ID0].S,SPI_Desc[SPI_ID0].module,SPI_Desc[SPI_ID0].clksel,SPI_Desc[SPI_ID0].irq,SPI_0_Init);


#endif

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
