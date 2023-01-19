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


/*********************************************************************************************************
** Function name:           nvtCAN(constructor)
** Descriptions:            reset the device
*********************************************************************************************************/
nvtCAN::nvtCAN(byte _CANSEL)
{
   uint8_t nn;
   /*Keep the _CANSEL to suuport muti CAN MFP, if presents */ 
   nCANSel = _CANSEL;
   canspeed_set = CAN_100KBPS;
   /*Variable initialization */
   nReservedTx = 0;
   opmode = CAN_BASIC_MODE;
   //rxCANMsg(0);
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
   for(nn=0; nn < NVT_MAXFILTER_NUM; nn++) rxMsgAMsk[nn].ulIDMask = 0;


}

/*********************************************************************************************************
** Function name:           mcp2515_reset
** Descriptions:            reset the device
*********************************************************************************************************/
void nvtCAN::ncan_reset(void) {

    /* Unlock protected registers */
    SYS_UnlockReg();
     /* Reset IP */
    SYS_ResetModule(module);
    /* Lock protected registers */
    SYS_LockReg();

    CAN_0_Init();
  
}


/*********************************************************************************************************
** Function name:           mcp2515_configRate
** Descriptions:            set baudrate
*********************************************************************************************************/
byte nvtCAN::ncan_configRate(const uint32_t canSpeed, const byte clock) {

    //Add NUC131 standard driver code to reset and enable CAN IP
    //SelectCANSpeed()
    uint32_t RealBaudRate = 0;
    uint32_t BaudRate = canSpeed;
    byte res = 0x00;
    /*Set target baud-rate and operation mode.*/
    RealBaudRate = CAN_Open(ncan,  BaudRate, CAN_BASIC_MODE);
    opmode = CAN_BASIC_MODE;
    /* Check the real baud-rate is OK */
    res = BaudRateCheck(BaudRate, RealBaudRate);
  	ncan->CON |= (CAN_TEST_LBACK_Msk);
    return res;
  
   
}


/*********************************************************************************************************
** Function name:           mcp2515_write_canMsg
** Descriptions:            write msg
**                          Note! There is no check for right address!
*********************************************************************************************************/
void nvtCAN::ncan_write_canMsg(const byte buffer_sidh_addr, unsigned long id, byte ext, byte rtrBit, byte len,
                                   volatile const byte* buf) {

    //Add NUC131 standard driver code to reset and enable CAN IP
    delay(10);


}



/*********************************************************************************************************
** Function name:           begin
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN::begin(uint32_t speedset, const byte clockset) {
    
    uint32_t nvtspeed;
    byte res ;
    
    nvtspeed = BaudRateSelector(speedset);  
    canspeed_set = speedset;
    res = ncan_configRate(nvtspeed, clockset);
    //if(res) return res;
   
    res = ncan_enableInterrput();
    return res;
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvtCAN::sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte* buf) {
   
    int32_t ires;
    uint8_t nn;
  
    STR_CANMSG_T msg1;

    ncan_enableInterrput();
    
    if(rtrBit==0x01){
        msg1.FrameType = CAN_REMOTE_FRAME;
        len = 0; /*DLC=0 for remote frame*/
    }
    else if(rtrBit==0x00){
        msg1.FrameType = CAN_DATA_FRAME;
    }
    else return 0xFF; 

    msg1.IdType   = ext;
    msg1.Id       = id;
    msg1.DLC      = len;
    for(nn=0; nn<len;nn++)
    {
        msg1.Data[nn] = buf[nn];
    }
    ires = CAN_Transmit(ncan, 0, &msg1); 
   
    return (byte)(ires);
}

/*********************************************************************************************************
** Function name:           sendMsgBuf Data Frame Only
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvtCAN::sendMsgBuf(unsigned long id, byte ext, byte len, volatile const byte* buf) {
   
    int32_t ires;
    uint8_t nn;
  
    STR_CANMSG_T msg1;

    ncan_enableInterrput();
    
    /* Send Message No.1 */
    msg1.FrameType = CAN_DATA_FRAME;
    msg1.IdType   = ext;
    msg1.Id       = id;
    msg1.DLC      = len;
    for(nn=0; nn<len;nn++)
    {
        msg1.Data[nn] = buf[nn];
    }
    ires = CAN_Transmit(ncan, 0, &msg1); 
   
    return (byte)(ires);
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvtCAN::sendMsgBufwMsgObj(byte status, unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte* buf) {
   
    int32_t ires;
    uint8_t nn;
  
    STR_CANMSG_T tMsg;
    uint32_t BaudRate;
    byte res = 0x00;
    uint32_t nvtspeed;

    if(ncan->CON & CAN_CON_TEST_Msk) /*If CAN is set to test mode, set it back to normal mode*/
    {
        ncan->TEST &= ~(CAN_TEST_LBACK_Msk|CAN_TEST_SILENT_Msk|CAN_TEST_BASIC_Msk);

        ncan->CON &= (~CAN_CON_TEST_Msk);
        
        nvtspeed = BaudRateSelector(canspeed_set);/*Recall baudrate from class variable*/

        BaudRate = CAN_Open(ncan,  nvtspeed, CAN_NORMAL_MODE);
         /* Check the real baud-rate is OK */
        res = BaudRateCheck(nvtspeed, BaudRate);

        ncan_enableInterrput();

        opmode = CAN_NORMAL_MODE;

    }


    /* Send a 11-bits message */
    tMsg.FrameType = CAN_DATA_FRAME;
    tMsg.IdType   = ext;
    tMsg.Id       = id;
    tMsg.DLC      = len;
    
    for(nn=0; nn<len;nn++)
    {
        tMsg.Data[nn] = buf[nn];
    }

    if(CAN_Transmit(ncan, MSG(0), &tMsg) == FALSE)  // Configure Msg RAM and send the Msg in the RAM
    {
        //printf("Set Tx Msg Object failed\n");
        return 0xFF;
    }
    return (byte)(0x00);

}

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            read message data field and its data length
*********************************************************************************************************/
 byte nvtCAN::readMsgBuf(byte *len, byte *buf)
 {
     //uint32_t u32IIDRstatus;
     if(opmode == CAN_BASIC_MODE)
     {
        *len =  rxCANMsg.DLC;
        buf =  rxCANMsg.Data;
        ext_flg = rxCANMsg.IdType;          // type, either extended (the 29 LSB) or standard (the 11 LSB)
        can_id  = rxCANMsg.Id;           // can id
        rtr     = !(rxCANMsg.FrameType);             // is remote frame, add "!", see can.c CAN_BasicReceiveMsg
     }
     else /*Normal Mode*/
     {
         //CAN_Receive(ncan, (uint32_t)(g32IIDRStatus-1), &rxCANMsg);
         *len =  rxCANMsg.DLC;
          buf =  rxCANMsg.Data;
          ext_flg = rxCANMsg.IdType;          // type, either extended (the 29 LSB) or standard (the 11 LSB)
          can_id  = rxCANMsg.Id;           // can id
          rtr     = rxCANMsg.FrameType;              // is remote frame

     }
     return (byte)(g32IIDRStatus);

 }

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            read message data field and its data length
*********************************************************************************************************/
 byte nvtCAN::readMsgBufID(unsigned long *ID, byte *len, byte *buf)
 {
    *ID = rxCANMsg.Id;
    *len =  rxCANMsg.DLC;
     buf =  rxCANMsg.Data;
   
     return 0;
 }

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            send buf
*********************************************************************************************************/
//byte nvtCAN::sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, const byte* buf, bool wait_sent) {
//    return sendMsg(id, ext, rtrBit, len, buf, wait_sent);
//}



      
/*********************************************************************************************************
** Function name:           checkReceive
** Descriptions:            check if got something, if yes, keep it in rxCANMsg
*********************************************************************************************************/
byte nvtCAN::checkReceive(void) {
    byte resb;
    int32_t resint;
    static byte resetFlag = 0;
    if(resetFlag==0x00) 
    {
        ncan_resetIF(1); //In basic mode
        resetFlag = 1;
    }
    resint = CAN_Receive(ncan, 0, &rxCANMsg);
    if(resint==TRUE) 
    {
        resb = CAN_MSGAVAIL;
        resetFlag = 0;
    }
    else
    {
        resb = CAN_NOMSG;
    }
    return resb;
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

    GPIO_SetMode(PB, BIT2, GPIO_PMD_OUTPUT);
    PB2=1;
   
    return res;
}


/*********************************************************************************************************
** Function name:           init_Mask
** Descriptions:            init msg ID mask
*********************************************************************************************************/
byte nvtCAN::init_Mask(byte num, byte ext, unsigned long ulData)
{
    uint32_t BaudRate;
    byte res = 0x00;
    uint32_t nvtspeed;

    if(ncan->CON & CAN_CON_TEST_Msk) /*If CAN is set to test mode, set it back to normal mode*/
    {
        ncan->TEST &= ~(CAN_TEST_LBACK_Msk|CAN_TEST_SILENT_Msk|CAN_TEST_BASIC_Msk);

        ncan->CON &= (~CAN_CON_TEST_Msk);
        
        nvtspeed = BaudRateSelector(canspeed_set);/*Recall baudrate from class variable*/

        BaudRate = CAN_Open(ncan,  nvtspeed, CAN_NORMAL_MODE);
         /* Check the real baud-rate is OK */
        res = BaudRateCheck(nvtspeed, BaudRate);

        ncan_enableInterrput();

        opmode = CAN_NORMAL_MODE;

    }
    
    if(num > 0x01) return num;

    rxMsgAMsk[num].ulIDMask = ulData;
    return res;
}


/*********************************************************************************************************
** Function name:           init_Filt
** Descriptions:            init msg ID filter
*********************************************************************************************************/
byte nvtCAN::init_Filt(byte num, byte ext, unsigned long ulData)
{
  
    int32_t  res;
    if(num > 0x05) return num; /*Follow MCP2515, provides six msg ID filters*/
    //if(rxMsgAMsk[0].ulIDMask) == 0) return num; /*To use msg ID filter, need to set msg ID mask first*/

    switch(num)
    {
            case 0:/*filter num 0,1 use mask 1 setting, follow MCP2515*/
            case 1:
                res = CAN_SetRxMsgAndMsk(ncan, MSG(num), ext, ulData, rxMsgAMsk[0].ulIDMask);
                break;
            
            case 2:/*filter num 2~5, use mask 2 setting, follow MCP2515*/
            case 3:
            case 4:
            case 5:
                res = CAN_SetRxMsgAndMsk(ncan, MSG(num), ext, ulData, rxMsgAMsk[1].ulIDMask);
                break;
            
            default:
                res = 0xFF; 
                break;
    }
    
    return (byte)(res);
}




//Dummy implementation for virtual function

/*********************************************************************************************************
** Function name:           ncan_init
** Descriptions:            init ncan
*********************************************************************************************************/
byte nvtCAN::ncan_init(const byte canSpeed, const byte clock) {
    
    byte res = 0;
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
#ifdef __cplusplus
extern "C" {
#endif
#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle CAN interrupt event                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void CAN_MsgInterrupt(CAN_T *tCAN, uint32_t u32IIDR)
{

    CAN_Receive(tCAN, (uint32_t)(u32IIDR-1), &rxCANMsg);
    if(u32IIDR == 1)
    {
        //rxCANMsg.Id = 0;
        //printf("Msg-0 INT and Callback\n");
        //CAN_Receive(tCAN, 0, &rxCANMsg);
        //CAN_ShowMsg(&rxCANMsg);
    }
    if(u32IIDR == 5 + 1)
    {
        //printf("Msg-5 INT and Callback \n");
        //CAN_Receive(tCAN, 5, &rxCANMsg);
        //CAN_ShowMsg(&rxCANMsg);
    }
   
}
#endif
void CAN0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;
    //PB2 = 1;
    u8IIDRstatus = CAN0->IIDR; /*Read Interrupr Identifier Register*/
    //g32IIDRStatus = u8IIDRstatus;
    if(u8IIDRstatus == 0x00008000)        /* Check Status Interrupt Flag (Error status Int and Status change Int) */
    {
        /**************************/
        /* Status Change interrupt*/
        /**************************/
        if(CAN0->STATUS & CAN_STATUS_RXOK_Msk)
        {
            PB2 = 1;
            CAN0->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear RxOK status*/
            PB2 = 0;
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
        PB2 = 1;
        //CAN_MsgInterrupt(CAN0, u8IIDRstatus);
        CAN_Receive(CAN0, (uint32_t)(u8IIDRstatus-1), &rxCANMsg);
        CAN_CLR_INT_PENDING_BIT(CAN0, (u8IIDRstatus - 1)); /* Clear Interrupt Pending */
        PB2 = 0;
    }
    else if(CAN0->WU_STATUS == 1)/*Wake-up Interrupt*/
    {
        //printf("Wake up\n");

        CAN0->WU_STATUS = 0;    /* Write '0' to clear */
    }
    //PB2 = 0;
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
