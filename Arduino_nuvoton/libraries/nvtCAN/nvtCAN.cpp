/**************************************************************************//**
 * @file     nvtCAN.cpp
 * @version  V1.00
 * @brief    NUC131 Series of Arduino CAN Library Source File
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#define DEBUG_EN 0
#include "nvtCAN.h"
#include "can.h"

/*static member data initialization*/
uint32_t nvtCAN::g32IIDRStatus = 0;
STR_CANMSG_T nvtCAN::rxCANMsg ={0};

/*********************************************************************************************************
** Function name:           nvtCAN(constructor)
** Descriptions:            reset the device
*********************************************************************************************************/
nvtCAN::nvtCAN(byte _CANSEL)
{
   uint8_t nn;
   /*Keep the _CANSEL to suuport muti CAN MFP, if presents */ 
   nCANSel = _CANSEL;
   
   /*Variable initialization */
   canspeed_set = CAN_100KBPS;
   nReservedTx = 0;
   opmode = CAN_BASIC_MODE;
  
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
** Function name:           ncan_reset
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
** Function name:           ncan_configRate
** Descriptions:            set baudrate
*********************************************************************************************************/
byte nvtCAN::ncan_configRate(const uint32_t canSpeed, const byte clock) {

    //Add NUC131 standard driver code to reset and enable CAN IP
 
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
** Function name:           begin
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN::begin(uint32_t speedset, const byte clockset) {
    
    uint32_t nvtspeed;
    byte res ;
    
    nvtspeed = BaudRateSelector(speedset);  
    canspeed_set = speedset;
    res = ncan_configRate(nvtspeed, clockset);
     
    res = ncan_enableInterrupt();
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
        ncan_disableInterrupt();
        
        ncan->TEST &= ~(CAN_TEST_LBACK_Msk|CAN_TEST_SILENT_Msk|CAN_TEST_BASIC_Msk);

        ncan->CON &= (~CAN_CON_TEST_Msk);
        
        nvtspeed = BaudRateSelector(canspeed_set);/*Recall baudrate from class variable*/

        BaudRate = CAN_Open(ncan,  nvtspeed, CAN_NORMAL_MODE);
         /* Check the real baud-rate is OK */
        res = BaudRateCheck(nvtspeed, BaudRate);

        ncan_enableInterrupt();

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
     uint32_t res;
     uint32_t ii;
     if(opmode == CAN_BASIC_MODE)
     {
        *len =  nvtCAN::rxCANMsg.DLC;
        for(ii=0;ii<*len;ii++) *buf++ = nvtCAN::rxCANMsg.Data[ii];
        ext_flg = nvtCAN::rxCANMsg.IdType;          // type, either extended (the 29 LSB) or standard (the 11 LSB)
        can_id  = nvtCAN::rxCANMsg.Id;              // can id
        rtr     = !(nvtCAN::rxCANMsg.FrameType);    // is remote frame, add "!", see can.c CAN_BasicReceiveMsg
        res     = 0xFFFFFFFF;
     }
     else if(opmode == CAN_NORMAL_MODE)/*Normal Mode*//*Normal Mode*/
     {
        *len =  nvtCAN::rxCANMsg.DLC;
        for(ii=0;ii<*len;ii++) *buf++ = nvtCAN::rxCANMsg.Data[ii];
        ext_flg = nvtCAN::rxCANMsg.IdType;          // type, either extended (the 29 LSB) or standard (the 11 LSB)
        can_id  = nvtCAN::rxCANMsg.Id;              // can id
        rtr     = nvtCAN::rxCANMsg.FrameType;      // is remote frame
        res     = nvtCAN::g32IIDRStatus;
     }
     return (byte)(res);

 }

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            read message data field and its data length
*********************************************************************************************************/
 byte nvtCAN::readMsgBufID(unsigned long *ID, byte *len, byte *buf)
 {
    uint32_t ii;
    *ID = rxCANMsg.Id;
    *len =  rxCANMsg.DLC;
    for(ii=0;ii<*len;ii++) *buf++ = nvtCAN::rxCANMsg.Data[ii];
   
     return 0;
 }

      
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
    ncan->IF[u8IF_Num].MCON     = 0x0;          // DLC
    ncan->IF[u8IF_Num].DAT_A1   = 0x0;          // data0,1
    ncan->IF[u8IF_Num].DAT_A2   = 0x0;          // data2,3
    ncan->IF[u8IF_Num].DAT_B1   = 0x0;          // data4,5
    ncan->IF[u8IF_Num].DAT_B2   = 0x0;          // data6,7
}



/*********************************************************************************************************
** Function name:           ncan_enableInterrput
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN::ncan_enableInterrupt(void) {
    
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
** Function name:           ncan_disableInterrupt
** Descriptions:            disable can interrupt
*********************************************************************************************************/
byte nvtCAN::ncan_disableInterrupt(void) {
    
    byte res = 0;
    /* Disable CAN interrupt */
    CAN_DisableInt(CAN0, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);
    /* Disable External Interrupt */
    NVIC_DisableIRQ(CAN0_IRQn);

    GPIO_SetMode(PB, BIT2, GPIO_PMD_OPEN_DRAIN);

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
        ncan_disableInterrupt();
        
        ncan->TEST &= ~(CAN_TEST_LBACK_Msk|CAN_TEST_SILENT_Msk|CAN_TEST_BASIC_Msk);

        ncan->CON &= (~CAN_CON_TEST_Msk);
        
        nvtspeed = BaudRateSelector(canspeed_set);/*Recall baudrate from class variable*/

        BaudRate = CAN_Open(ncan,  nvtspeed, CAN_NORMAL_MODE);
         /* Check the real baud-rate is OK */
        res = BaudRateCheck(nvtspeed, BaudRate);

        ncan_enableInterrupt();

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
                               /*To use msg ID filter, need to set msg ID mask first*/

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

/*---------------------------------------------------------------------------------------------------------*/
/* CAN0 interrupt handler                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle CAN interrupt event                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void CAN0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;
    STR_CANMSG_T* prxCANMsg;
    u8IIDRstatus = CAN0->IIDR; /*Read Interrupr Identifier Register*/
  
    if(u8IIDRstatus == 0x00008000)        /* Check Status Interrupt Flag (Error status Int and Status change Int) */
    {
        if(CAN0->STATUS & CAN_STATUS_RXOK_Msk)/* Status Change interrupt*/
        {
            
            CAN0->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear RxOK status*/
           
        }

        if(CAN0->STATUS & CAN_STATUS_TXOK_Msk)
        {
            CAN0->STATUS &= ~CAN_STATUS_TXOK_Msk;    /* Clear TxOK status*/
        }

        if(CAN0->STATUS & CAN_STATUS_BOFF_Msk) /* Error Status interrupt */
        {
            
        }
        else if(CAN0->STATUS & CAN_STATUS_EWARN_Msk)
        {
            
        }
        else if((CAN0->ERR & CAN_ERR_TEC_Msk) != 0)
        {
           
        }
        else if((CAN0->ERR & CAN_ERR_REC_Msk) != 0)
        {
           
        }

    }
    else if((u8IIDRstatus >= 0x1) || (u8IIDRstatus <= 0x20))/*Message Object Interrupt*/
    {
        PB2 = 1;
        prxCANMsg = nvtCAN::getrxCANMsgPtr();
        nvtCAN::setg32IIDRStatus(u8IIDRstatus);
        CAN_Receive(CAN0, (uint32_t)(u8IIDRstatus-1),prxCANMsg);
        if (callbackCAN0) callbackCAN0();	  	
        PB2 = 0;
        CAN_CLR_INT_PENDING_BIT(CAN0, (u8IIDRstatus - 1)); /* Clear Interrupt Pending */
    }
    else if(CAN0->WU_STATUS == 1)/*Wake-up Interrupt*/
    {
        CAN0->WU_STATUS = 0;    /* Write '0' to clear */
    }
   
}


/*********************************************************************************************************
** Function name:           BaudRateCheck
** Descriptions:            check if CAN Buadrate equals the desired value
*********************************************************************************************************/
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


/*********************************************************************************************************
** Function name:           BaudRateSelector
** Descriptions:            to map MCP2515 BaudRate setting to NUC131
*********************************************************************************************************/
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

/* Configure CAN interrupt resources */
static void __initializeCAN() {
	
    callbackCAN0 = NULL;
    NVIC_EnableIRQ(CAN0_IRQn);
}


void attachInterruptCAN(void (*callback)(void))
{
    static int enabledCAN = 0;
	
    if (!enabledCAN) {
        __initializeCAN();
        enabledCAN = 1;
    }

    callbackCAN0 = callback;

}

#ifdef __cplusplus
}
#endif
#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
