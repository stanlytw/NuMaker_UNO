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
#include "can.h"
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
       ncan = (CAN_T *)CAN0;
    
   }
   else//Update code for more than 2 CAN interface's device 
   {
       module = CAN0_MODULE;
       ncan = (CAN_T *)CAN0;
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
    //SYS_UnlockReg();
     /* Enable CAN module clock */
    //CLK_EnableModuleClock(module);
     /* Reset IP */
    //SYS_ResetModule(module);
    /* Lock protected registers */
    //SYS_LockReg();

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
  	ncan->CON |= (CAN_TEST_LBACK_Msk);
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
    
    uint32_t nvtspeed = BaudRateSelector(speedset);
    byte res ;
    
    res = ncan_configRate(nvtspeed, clockset);
    //if(res) return res;
   
    //res = ncan_enableInterrput();
    return res;
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvtCAN::sendMsgBuf(byte status, unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte* buf) {
   
    int32_t delaycount;
    int8_t nn;
    uint32_t res =0 ;

    STR_CANMSG_T msg1;

    ncan_enableInterrput();
    delaycount = 1000;
    /* Send Message No.1 */
    msg1.FrameType = CAN_DATA_FRAME;
    msg1.IdType   = CAN_STD_ID;
    msg1.Id       = 0x6FF;
    msg1.DLC      = 2;
     /* Send a 11-bit Standard Identifier message */
    //tMsg.FrameType = CAN_DATA_FRAME;
    //tMsg.IdType   = CAN_STD_ID;
    //tMsg.Id       = 0x7FF;
    //tMsg.DLC      = 2;
    msg1.Data[0]  = 7;
    msg1.Data[1]  = 0xFF;

   CAN_Transmit(ncan, 0, &msg1); 
   
/* Send Message No.2 */
    msg1.FrameType = CAN_DATA_FRAME;
    msg1.IdType   = CAN_STD_ID;
    msg1.Id       = 0x1AC;
    msg1.DLC      = 8;
    msg1.Data[0]  = 0x11;
    msg1.Data[1]  = 0x12;
    msg1.Data[2]  = 0x13;
    msg1.Data[3]  = 0x14;
    msg1.Data[4]  = 0x15;
    msg1.Data[5]  = 0x16;
    msg1.Data[6]  = 0x17;
    msg1.Data[7]  = 0x18;
    CAN_Transmit(ncan, 0, &msg1);//Send CAN message
    return (byte)res;
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


/*********************************************************************************************************
** Function name:           ncan_ResetIF
** Descriptions:            Reset message interface parameters 
*********************************************************************************************************/
void nvtCAN::ncan_resetIF(uint8_t u8IF_Num)
{
    if(u8IF_Num > 1)
        return;
    ncan->IF[u8IF_Num].CREQ     = 0x0;          // set bit15 for sending
    ncan->IF[u8IF_Num].CMASK    = 0x0;
    ncan->IF[u8IF_Num].MASK1    = 0x0;          // useless in basic mode
    ncan->IF[u8IF_Num].MASK2    = 0x0;          // useless in basic mode
    ncan->IF[u8IF_Num].ARB1     = 0x0;          // ID15~0
    ncan->IF[u8IF_Num].ARB2     = 0x0;          // MsgVal, eXt, xmt, ID28~16
    ncan->IF[u8IF_Num].MCON     = 0x0;              // DLC
    ncan->IF[u8IF_Num].DAT_A1   = 0x0;          // data0,1
    ncan->IF[u8IF_Num].DAT_A2   = 0x0;          // data2,3
    ncan->IF[u8IF_Num].DAT_B1   = 0x0;          // data4,5
    ncan->IF[u8IF_Num].DAT_B2   = 0x0;          // data6,7
}



/*********************************************************************************************************
** Function name:           ncan_enableInterrput
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN::ncan_enableInterrput(void) {
    
    byte res = 0;
    /* Enable CAN interrupt */
    CAN_EnableInt(CAN0, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);
    /* Set Interrupt Priority */
    NVIC_SetPriority(CAN0_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
    /* Enable External Interrupt */
    NVIC_EnableIRQ(CAN0_IRQn);
    return res;
}

uint32_t nvtCAN::ncan_readStatus(void) 
{
    
  uint32_t u8IIDRstatus;

    u8IIDRstatus = CAN0->IIDR; /*Read Interrupr Identifier Register*/
    return u8IIDRstatus;
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
#ifdef __cplusplus
extern "C" {
#endif

void CAN0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;

    u8IIDRstatus = CAN0->IIDR; /*Read Interrupr Identifier Register*/
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
    else if((u8IIDRstatus >= 0x1) || (u8IIDRstatus <= 0x20))/*Message Object Interrupt*/
    {
        CAN_CLR_INT_PENDING_BIT(CAN0, (u8IIDRstatus - 1)); /* Clear Interrupt Pending */
    }
    else if(CAN0->WU_STATUS == 1)/*Wake-up Interrupt*/
    {
        //printf("Wake up\n");

        CAN0->WU_STATUS = 0;    /* Write '0' to clear */
    }

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

uint32_t BaudRateSelector(uint32_t u32mcpBaudRate)
{
    uint32_t u32NvtBaudRate;

   switch(u32mcpBaudRate)
    {
        case CAN_10KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_10K;
            break;

        case CAN_20KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_20K;
            break;

        case CAN_25KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_25K;
            break;

        case CAN_31K25BPS:
             u32NvtBaudRate = CAN_BAUDRATE_31K25;
            break;

        case CAN_33KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_33K;
            break;        

        case CAN_40KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_40K;
            break;

        case CAN_50KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_50K;
            break;

        case CAN_80KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_80K;
            break;

        case CAN_83K3BPS:
             u32NvtBaudRate = CAN_BAUDRATE_83K3;
            break;

        case CAN_95KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_95K;
            break;

         case CAN_100KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_100K;
            break;

        case CAN_125KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_125K;
            break;

        case CAN_200KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_200K;
            break;

        case CAN_250KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_250K;
            break;

        case CAN_666KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_666K;
            break;        

        case CAN_800KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_800K;
            break;

        case CAN_1000KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_1000K;
            break;

        default:
             u32NvtBaudRate = CAN_BAUDRATE_10K;
            break;

    
     }
     return u32NvtBaudRate;
}
#ifdef __cplusplus
}
#endif
#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
