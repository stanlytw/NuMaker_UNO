/*
    nvtCAN.cpp
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
#include "nvtCAN.h"

//#define spi_readwrite      pSPI->transfer
//#define spi_read()         spi_readwrite(0x00)
//#define spi_write(spi_val) spi_readwrite(spi_val)
//#define SPI_BEGIN()        pSPI->beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0))
//#define SPI_END()          pSPI->endTransaction()


/*********************************************************************************************************
** Function name:           nvtCAN(constructor)
** Descriptions:            reset the device
*********************************************************************************************************/
nvtCAN::nvtCAN(byte _CANSEL)
{
   /*Keep the _CANSEL to suuport muti CAN MFP, if presents */ 
   nCANSel = _CANSEL;
   
   /*Variable initialization */
   nReservedTx = 0;

   if( _CANSEL)
   {
       module = CAN0_MODULE;
       ncan = CAN0;
   }
   else//Update code for more than 2 CAN interface's device 
   {
       module = CAN0_MODULE;
       ncan = CAN0;
   }
   
   ncan_reset();


}

/*********************************************************************************************************
** Function name:           mcp2515_reset
** Descriptions:            reset the device
*********************************************************************************************************/
void nvtCAN::ncan_reset(void) {
#if defined(__NUC131__)
    /* Unlock protected registers */
    SYS_UnlockReg();
    /* Reset IP */
    SYS_ResetModule(module);
     /* Enable CAN module clock */
    CLK_EnableModuleClock(module);
    /* Lock protected registers */
    SYS_LockReg();

    CAN_0_Init();
#endif    
}


/*********************************************************************************************************
** Function name:           mcp2515_configRate
** Descriptions:            set baudrate
*********************************************************************************************************/
byte nvtCAN::ncan_configRate(const uint32_t canSpeed, const byte clock) {
#if defined(__NUC131__)
    //Add NUC131 standard driver code to reset and enable CAN IP
    //SelectCANSpeed()
    uint32_t RealBaudRate = 0;
    uint32_t BaudRate = canSpeed;
    byte res = 0x00;
    /*Set target baud-rate and operation mode.*/
    RealBaudRate = CAN_Open(ncan,  BaudRate, CAN_BASIC_MODE);

    /* Check the real baud-rate is OK */
    res = BaudRateCheck(BaudRate, RealBaudRate);
    ncan->CON |= CAN_TEST_LBACK_Msk;
    return res;
#endif    
   
}


/*********************************************************************************************************
** Function name:           mcp2515_write_canMsg
** Descriptions:            write msg
**                          Note! There is no check for right address!
*********************************************************************************************************/
void nvtCAN::ncan_write_canMsg(const byte buffer_sidh_addr, unsigned long id, byte ext, byte rtrBit, byte len,
                                   volatile const byte* buf) {
#if defined(__NUC131__)
    //Add NUC131 standard driver code to reset and enable CAN IP
    delay(10);
#endif   

}



/*********************************************************************************************************
** Function name:           begin
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN::begin(uint32_t speedset, const byte clockset) {
    
    byte res = ncan_configRate(speedset, clockset);

    return res;
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvtCAN::sendMsgBuf(byte status, unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte* buf) {
   
    int32_t delaycount;

    /* Declare a CAN message structure */
    STR_CANMSG_T msg1;
    delaycount = 1000;

    /* Enable CAN interrupt */
    //CAN_EnableInt(ncan, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);
    /* Set Interrupt Priority */
    //NVIC_SetPriority(CAN0_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
    /* Enable External Interrupt */
    //NVIC_EnableIRQ(CAN0_IRQn);

    /* Send Message No.1 */
    msg1.FrameType = CAN_DATA_FRAME;
    msg1.IdType   = CAN_STD_ID;
    msg1.Id       = 0x0001;//(uint32_t)(id);
    msg1.DLC      = 2;
    msg1.Data[0]  = 0x00;
    msg1.Data[1]  = 0x02;
    CAN_Transmit(ncan, 0, &msg1);//Send CAN message
    //printf("Send STD_ID:0x1,Data[0,2]\n");
    CLK_SysTickDelay(delaycount);   /* Generate the Delay Time by Systick */
    return CAN_OK;
}



/*********************************************************************************************************
** Function name:           sendMsg
** Descriptions:            send message
*********************************************************************************************************/
byte nvtCAN::sendMsg(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf, bool wait_sent) {
    byte res, res1, txbuf_n;
    uint16_t uiTimeOut = 0;


    return CAN_OK;

}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            send buf
*********************************************************************************************************/
byte nvtCAN::sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf, bool wait_sent) {
    return sendMsg(id, ext, rtrBit, len, buf, wait_sent);
}



/*********************************************************************************************************
** Function name:           checkReceive
** Descriptions:            check if got something
*********************************************************************************************************/
byte nvtCAN::checkReceive(void) {
    byte res;
    //res = mcp2515_readStatus();                                         // RXnIF in Bit 1 and 0
    return res;
}


/*********************************************************************************************************
** Function name:           ncan_init
** Descriptions:            init ncan
*********************************************************************************************************/
byte nvtCAN::ncan_init(const byte canSpeed, const byte clock) {
    byte res;

    /* Unlock protected registers */
    SYS_UnlockReg();
    //res = mcp2515_readStatus();                                         // RXnIF in Bit 1 and 0
    return res;
}



byte BaudRateCheck(uint32_t u32BaudRate, uint32_t u32RealBaudRate)
{
    byte res;

    if(u32BaudRate != u32RealBaudRate)
    {
        res = 0x01;
    }
    else
    {
        res = 0x00;
    }
    return res;
}





#if CAN_MAX_COUNT > 0
#define CAN_ID0 0
static void CAN_0_Init(void) {	
	CAN_Config(CAN_Desc[CAN_ID0]);	
}
//To fit SeedArcuino
//nvtCAN SPI(SPI_Desc[SPI_ID0].S,SPI_Desc[SPI_ID0].module,SPI_Desc[SPI_ID0].clksel,SPI_Desc[SPI_ID0].irq,SPI_0_Init);
/*---------------------------------------------------------------------------------------------------------*/
/* CAN0 interrupt handler                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void CAN0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;

    u8IIDRstatus = CAN0->IIDR;

    if(u8IIDRstatus == 0x00008000)        /* Check Status Interrupt Flag (Error status Int and Status change Int) */
    {
        /**************************/
        /* Status Change interrupt*/
        /**************************/
        if(CAN0->STATUS & CAN_STATUS_RXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear RxOK status*/
        }

        if(CAN0->STATUS & CAN_STATUS_TXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_TXOK_Msk;    /* Clear TxOK status*/
        }

        /**************************/
        /* Error Status interrupt */
        /**************************/
        if(CAN0->STATUS & CAN_STATUS_BOFF_Msk)
        {
            //printf("BOFF INT\n") ;
        }
        else if(CAN0->STATUS & CAN_STATUS_EWARN_Msk)
        {
            //printf("EWARN INT\n") ;
        }
        else if((CAN0->ERR & CAN_ERR_TEC_Msk) != 0)
        {
           //printf("Transmit error!\n") ;
        }
        else if((CAN0->ERR & CAN_ERR_REC_Msk) != 0)
        {
            //printf("Receive error!\n") ;
        }

    }
    else if((u8IIDRstatus >= 0x1) || (u8IIDRstatus <= 0x20))
    {
        CAN_CLR_INT_PENDING_BIT(CAN0, (u8IIDRstatus - 1)); /* Clear Interrupt Pending */
    }
    else if(CAN0->WU_STATUS == 1)
    {
        //printf("Wake up\n");

        CAN0->WU_STATUS = 0;    /* Write '0' to clear */
    }

}



#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
