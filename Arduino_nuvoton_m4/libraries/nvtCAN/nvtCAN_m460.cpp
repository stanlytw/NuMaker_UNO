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
#include "nvtCAN_m460.h"
#include "canfd.h"



/*static member data initialization*/
//uint32_t g32IIDRStatus = 0;
//CANFD_FD_MSG_T rxCANMsg;// ={0};
static CANFD_FD_MSG_T sRxMsgFrame;
/*********************************************************************************************************
** Function name:           nvtCAN(constructor)
** Descriptions:            reset the device
*********************************************************************************************************/
nvtCAN_m460::nvtCAN_m460(byte _CANSEL)
{
   uint8_t nn;
   /*Keep the _CANSEL to suuport muti CAN MFP, if presents */ 
   nCANSel = _CANSEL;
   
   /*Variable initialization */
   _canspeed_set = CAN_BAUDRATE_10K;
   nReservedTx = 0;
   _canmode = CANFD_OP_CAN_MODE;
  
   if( _CANSEL)
   {
       module = CANFD0_MODULE;
       ncan = (CANFD_T *)CANFD0;
    
   }
   else//Update code for more than 2 CAN interface's device 
   {
       module = CANFD0_MODULE;
       ncan = (CANFD_T *)CANFD0;
   }
   
   //[2025-03-17]Reset CANFD　IP.
   //This will be done in CANFD_Open
   //ncan_reset();
   
  
   //for(nn=0; nn < NVT_MAXFILTER_NUM; nn++) rxMsgAMsk[nn].ulIDMask = 0;


}

/*********************************************************************************************************
** Function name:           ncan_reset
** Descriptions:            reset the device
*********************************************************************************************************/
void nvtCAN_m460::ncan_reset(void) {

    /* Unlock protected registers */
    SYS_UnlockReg();
    /* Reset IP */
    SYS_ResetModule(module);
    /* Lock protected registers */
    SYS_LockReg();
  
}


/*********************************************************************************************************
** Function name:           ncan_configRate
** Descriptions:            set baudrate
*********************************************************************************************************/
//byte nvtCAN_m460::ncan_configRate(const uint32_t canSpeed, const byte clock) {

//    //Add NUC131 standard driver code to reset and enable CAN IP
 
//    uint32_t RealBaudRate = 0;
//    uint32_t BaudRate = canSpeed;
//    byte res = 0x00;
    /*Set target baud-rate and operation mode.*/
    //RealBaudRate = CAN_Open(ncan,  BaudRate, CANFD_OP_CAN_MODE);
    //opmode = CANFD_OP_CAN_MODE;
    /* Check the real baud-rate is OK */
    //res = BaudRateCheck(BaudRate, RealBaudRate);
    //ncan->CON |= (CAN_TEST_LBACK_Msk);
//    return res;
  
//   
//}

/*********************************************************************************************************
** Function name:           setMode
** Descriptions:            set CAN op mode
*********************************************************************************************************/
byte nvtCAN_m460::setMode(uint32_t opmode) {

    _canmode = opmode;

}

/*********************************************************************************************************
** Function name:           BaudRateParser
** Descriptions:            to get the desired BaudRate. also returns if it is for CAN or CAN-FD
*********************************************************************************************************/
byte nvtCAN_m460::BaudRateParser(uint32_t u32mcpBaudRate)
{
   	uint32_t  normalspeed_val;
	uint32_t  dataspeed_val;
	uint8_t   dataspeed_idx;
	byte      res;
	/* Get dataspeed index */
	dataspeed_idx = (uint8_t)(u32mcpBaudRate >> 24) & 0xFF ;
	
	if( dataspeed_idx )//CAN-FD
	{
		//e.g: CAN_BAUDRATE_100K_400K =  ( 4UL << 24) | (100000UL);
		/*
		    Get normalspeed
		*/
		normalspeed_val = u32mcpBaudRate & 0x00FFFFFF;//lower 24-bit
		
		/*
		    Get dataspeed
		*/
		//[2025-03-20]Simple take the dataspeed_val = dataspeed_idx*100K
		dataspeed_val = (uint32_t)(dataspeed_idx) * 100 *1000; 
		
		
		_normalspeed_set = normalspeed_val;
	    _dataspeed_set = dataspeed_val;
		
		res = 1;
	}
	else//CAN
	{
	    _normalspeed_set = BaudRateSelector(u32mcpBaudRate);
	  	res = 0;
	}

    //Serial.print("bit check:");
	//Serial.println(_normalspeed_set, DEC);
	//Serial.println(_dataspeed_set, DEC);
	

	return res;
       
}

/*********************************************************************************************************
** Function name:           BaudRateSelector
** Descriptions:            to map MCP2515 BaudRate setting to NUC131
*********************************************************************************************************/
uint32_t nvtCAN_m460::BaudRateSelector(uint32_t u32mcpBaudRate)
{
    uint32_t u32NvtBaudRate;

   switch(u32mcpBaudRate)
    {
        case CAN_BAUDRATE_1KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_1K;
            break;
			
		case CAN_BAUDRATE_10KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_10K;
            break;

        case CAN_BAUDRATE_20KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_20K;
            break;

        case CAN_BAUDRATE_25KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_25K;
            break;

        case CAN_BAUDRATE_40KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_40K;
            break;

        case CAN_BAUDRATE_100KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_100K;
            break;

        case CAN_BAUDRATE_125KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_125K;
            break;

        case CAN_BAUDRATE_200KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_200K;
            break;

        case CAN_BAUDRATE_250KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_250K;
            break;

        case CAN_BAUDRATE_500KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_500K;
            break;

        case CAN_BAUDRATE_666KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_666K;
            break;        

        case CAN_BAUDRATE_800KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_800K;
            break;

        case CAN_BAUDRATE_1000KBPS:
             u32NvtBaudRate = CAN_BAUDRATE_1000K;
            break;

        default:
             u32NvtBaudRate = CAN_BAUDRATE_10K;
            break;
    
     }
     return u32NvtBaudRate;
}
/*********************************************************************************************************
** Function name:           begin
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN_m460::begin(uint32_t speedset) {
    
    uint32_t nvtspeed;
    byte speedtype;
	byte res;
	/* Set CANFD Pin MFP. Currently CANFD0 only */
    CANFD_0_Init();
	
	//[2025-03-20]To combine CAN/CAN-FD setting within one begin function. MCP2515/2518 compatible
    /*
	    speedtype=1, means CAN-FD speed(normal+data);
		speedtype=0, means CAN    speed(normal);
	*/
	speedtype = BaudRateParser(speedset);
	
    //Check if mode vs speed settings are coincident
	if(speedtype == 1)//CAN-FD
	{
		if( _canmode != CANFD_OP_CAN_FD_MODE)
		{
		    return -1;	
		} 
		else
		{
			res =  CANFD_0_SetConfig(CANFD_OP_CAN_FD_MODE, _normalspeed_set, _dataspeed_set);
		}
	}
	else//CAN
	{
		if( _canmode != CANFD_OP_CAN_MODE)
		{
		    return -2;	
		}
        else
        {
			res = CANFD_0_SetConfig(CANFD_OP_CAN_MODE, _normalspeed_set, 0);
		}		
	}
	
	//[2025-03-21] CAN TX configuration
	/*
	    On the fly
	*/
	
	
	
	//[2025-03-21] CAN RX configuration
    /*
	    Default: Keep all so that some msg can be get without setting SID/XID fileter
	*/	
	ncan_configGFC(gfcconfig);
		
	//Sync CCCR[0]
	ncan_syncInit();
	//[2025-03-18] todo 
    //res = ncan_enableInterrupt();
	return res;
}

/*********************************************************************************************************
** Function name:           beginCANIntLBKMode
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN_m460::beginCANIntLBKMode(uint32_t speedset){
    
    uint32_t nvtspeed;
    byte speedtype;
	byte res;
	/* Set CANFD Pin MFP. Currently CANFD0 only */
    CANFD_0_Init();
	
	//[2025-03-20]To combine CAN/CAN-FD setting within one begin function. MCP2515/2518 compatible
    /*
	    speedtype=1, means CAN-FD speed(normal+data);
		speedtype=0, means CAN    speed(normal);
	*/
	speedtype = BaudRateParser(speedset);
	
    //Check if mode vs speed settings are coincident
	if(speedtype == 1)//CAN-FD
	{
		return -1;	
	}
	else//CAN
	{
		if( _canmode != CANFD_OP_CAN_MODE)
		{
		    return -2;	
		}
        else
        {
			res = CANFD_0_SetConfig(CANFD_OP_CAN_MODE, _normalspeed_set, 0);
				
	        //[2025-03-31]Test CAN_FD Internal loopback mode
	        CANFD0->CCCR |= CANFD_CCCR_MON_Msk;
	        CANFD0->CCCR |= CANFD_CCCR_TEST_Msk;
	        CANFD0->TEST |= CANFD_TEST_LBCK_Msk;
		}		
	}
	
	//[2025-03-21] CAN TX configuration
	/*
	    On the fly
	*/
	
	
	
	//[2025-03-21] CAN RX configuration
    /*
	    Default: Keep all so that some msg can be get without setting SID/XID fileter
	*/	
	ncan_configGFC(gfcconfig);
		
	//Sync CCCR[0]
	ncan_syncInit();
	//[2025-03-18] todo 
    //res = ncan_enableInterrupt();
	return res;
}

/*********************************************************************************************************
** Function name:           ncan_configGFC
** Descriptions:            keep in FIFO or reject unmatched CAN msg. Wrapper to set CANFD GFC register 
*********************************************************************************************************/
uint8_t nvtCAN_m460::ncan_configGFC(uint8_t gfc_config)
{
    if(gfc_config == CANFD_GFC_REJEALL)//default
	{
       /* Reject Non-Matching Standard ID and Extended ID Filter(RX FIFO0) */
	   CANFD_SetGFC(CANFD0, eCANFD_REJ_NON_MATCH_FRM, eCANFD_REJ_NON_MATCH_FRM, 1, 1);	   
	}
	else if(gfc_config == CANFD_GFC_KEEPALL)
	{
	   /* Reject Non-Matching Standard ID and Extended ID Filter(all to FIFO0) */
	   CANFD_SetGFC(CANFD0, eCANFD_ACC_NON_MATCH_FRM_RX_FIFO0, eCANFD_ACC_NON_MATCH_FRM_RX_FIFO0, 0, 0);
	}
	else
	{
		return -1;
	}
	
	gfcconfig = gfc_config;
    return 0;
}

/*********************************************************************************************************
** Function name:           setMsgFilter
** Descriptions:            set is ID,xID filter are set. 
*********************************************************************************************************/
byte nvtCAN_m460::setMsgFilter(uint32_t en)
{
	if(en)
	{
	    gfcconfig = CANFD_GFC_REJEALL;
 	}
	else
	{
		gfcconfig = CANFD_GFC_KEEPALL;
	}
	return 0;	
}





/*********************************************************************************************************
** Function name:           ncan_syncInit
** Descriptions:            Sync CCCR[0] once Init. configuration changes 
*********************************************************************************************************/
int32_t nvtCAN_m460::ncan_syncInit(void)
{
     /* CAN Run to Normal mode */
	return CANFD_RunToNormal(CANFD0, 1);
}


/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvtCAN_m460::sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte* buf) {
   
    int32_t ires = 0x00;
    uint8_t nn;

	CANFD_FD_MSG_T      sTxMsgFrame;
	
	//ID or xID
    if(ext==0x01){
        sTxMsgFrame.eIdType = eCANFD_XID;
    }
    else if(ext==0x00){
        sTxMsgFrame.eIdType = eCANFD_SID;
    }
    else return 0xFF; 
	
	
	//remote frame or data frame
	if(rtrBit==0x01){
        sTxMsgFrame.eFrmType = eCANFD_REMOTE_FRM;
    }
    else if(rtrBit==0x00){
        sTxMsgFrame.eFrmType = eCANFD_DATA_FRM;
    }
    else return 0xFF; 


	sTxMsgFrame.u32Id = id;
    sTxMsgFrame.u32DLC = len;
	sTxMsgFrame.bBitRateSwitch = 0;
	
	for(nn=0; nn<len;nn++)
    {
        sTxMsgFrame.au8Data[nn] = buf[nn];
    }
	
	/* Use message buffer 0 */
    
    ires = CANFD_TransmitTxMsg(CANFD0, CANFD_TXBUF0, &sTxMsgFrame);
	return (byte)(ires);

}

/*********************************************************************************************************
** Function name:           sendMsgBuf Data Frame Only
** Descriptions:            Send message by using buffer read as free from CANINTF status
**                          Status has to be read with readRxTxStatus and filtered with checkClearTxStatus
*********************************************************************************************************/
byte nvtCAN_m460::sendMsgBuf(unsigned long id, byte ext, byte len, volatile const byte* buf) {
   
    int32_t ires = 0x00;
    uint8_t nn;
  
    CANFD_FD_MSG_T      sTxMsgFrame;
	
	
    if(ext==0x01){
        sTxMsgFrame.eIdType = eCANFD_XID;
    }
    else if(ext==0x00){
        sTxMsgFrame.eIdType = eCANFD_SID;
    }
    else return 0xFF; 
	sTxMsgFrame.eFrmType = eCANFD_DATA_FRM;
	sTxMsgFrame.u32Id = id;
	
    sTxMsgFrame.u32DLC = len;
	sTxMsgFrame.bBitRateSwitch = 0;
	
	for(nn=0; nn<len;nn++)
    {
        sTxMsgFrame.au8Data[nn] = buf[nn];
    }
	
	/* Use message buffer 0 */
    
    ires = CANFD_TransmitTxMsg(CANFD0, CANFD_TXBUF0, &sTxMsgFrame);
	return (byte)(ires);

}


byte nvtCAN_m460::sendMsgBuf(unsigned long id, byte ext, byte len, volatile const byte* buf, uint8_t is_fd) {
   
    int32_t ires = 0x00;
    uint8_t nn;
  
    CANFD_FD_MSG_T      sTxMsgFrame;
	if(!is_fd)
		return 0xF1;//For FD only
	if(_canmode!=CANFD_OP_CAN_FD_MODE)
		return 0xF2;//For FD only
	
	//Processing FD packets
    if(ext==0x01){
        sTxMsgFrame.eIdType = eCANFD_XID;
    }
    else if(ext==0x00){
        sTxMsgFrame.eIdType = eCANFD_SID;
    }
    else return 0xFF;
	
	/* Set the id */
	sTxMsgFrame.u32Id = id;
	/* Set data length */
    sTxMsgFrame.u32DLC = len;
	/* Set the frame type */
    sTxMsgFrame.eFrmType = eCANFD_DATA_FRM;
    
	/* Set CAN FD frame format */
    sTxMsgFrame.bFDFormat = 1;
    /* Set the bitrate switch */
    sTxMsgFrame.bBitRateSwitch = 1;
			
	for(nn=0; nn<len;nn++)
    {
        sTxMsgFrame.au8Data[nn] = buf[nn];
    }
	
	/* Use message buffer 0 */
    ires = CANFD_TransmitTxMsg(CANFD0, CANFD_TXBUF0, &sTxMsgFrame);
	return (byte)(ires);

}

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            read message data field and its data length
*********************************************************************************************************/
 byte nvtCAN_m460::readMsgBuf(byte *len, byte *buf)
 {
    uint32_t res = 0x00;
    uint32_t ii;
	 
	 
	*len =  sRxMsgFrame.u32DLC;           // data length
     
	for(ii=0;ii<*len;ii++)                // data content
	*buf++ = sRxMsgFrame.au8Data[ii];
     
	ext_flg = sRxMsgFrame.eIdType;        // type, either extended (the 29 LSB) or standard (the 11 LSB)
     
	can_id  = sRxMsgFrame.u32Id;          // can id
     
	rtr     = !(sRxMsgFrame.eFrmType);    // is remote frame, add "!", see can.c CAN_BasicReceiveMsg
	 
	fdformat = sRxMsgFrame.bFDFormat;      // FD format bit
	 
	switchBR = sRxMsgFrame.bBitRateSwitch; // Bit rate switching bit
     
	res     = 0xFF;

    return (byte)(res);

 }

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            read message data field and its data length
*********************************************************************************************************/
 byte nvtCAN_m460::readMsgBufID(unsigned long *ID, byte *len, byte *buf)
 {
    uint32_t ii;
    //*ID = rxCANMsg.Id;
    //*len =  rxCANMsg.DLC;
    //for(ii=0;ii<*len;ii++) *buf++ = nvtCAN::rxCANMsg.Data[ii];
   
     return 0;
 }

      
/*********************************************************************************************************
** Function name:           checkReceive
** Descriptions:            check if got something, if yes, keep it in rxCANMsg
*********************************************************************************************************/
byte nvtCAN_m460::checkReceive(void) 
{
	
	byte res =0x00;
	if( gfcconfig == CANFD_GFC_KEEPALL )
	{
	    /*
	       By designed, FIFO will either KeepAll or RejectAll
	       Msg FIFO0(ID) and  Msg FIFO1(xID)
	    */	
	   
	    /* Check for any received unmatched STD messages on CAN message FIFO0 */
		if(CANFD_ReadRxFifoMsg(CANFD0, CANFD_RXFIFO0, &sRxMsgFrame) == 1)
        {
            //printf("Rx FIFO 0: Received message 0x%08X (11-bit)\r\n", sRxMsgFrame.u32Id);
            //printf("Message Data : ");

            //for(u8Cnt = 0; u8Cnt < sRxMsgFrame.u32DLC; u8Cnt++)
            //{
                //printf("%02d ,", sRxMsgFrame.au8Data[u8Cnt]);

            //    if(sRxMsgFrame.au8Data[u8Cnt] != u8Cnt + u8RxTestNum)
            //    {
            //        u8ErrFlag = 1;
            //    }
			//}
			res = 0x03;
        }
     
	}
	else if( gfcconfig == CANFD_GFC_REJEALL )
	{
	    /*
	     Msg Buffer0(ID, xID)
	    */
	    if(CANFD_ReadRxBufMsg(CANFD0, CANFD_RXBUF0, &sRxMsgFrame) == 1)
	    {
		    res = 0x03;
	    }
	}
	
	
	
	
	return res;
}


/*********************************************************************************************************
** Function name:           ncan_ResetIF
** Descriptions:            Reset message interface parameters 
*********************************************************************************************************/
void nvtCAN_m460::ncan_resetIF(uint8_t u8IF_Num)
{
   
}



/*********************************************************************************************************
** Function name:           ncan_enableInterrput
** Descriptions:            init can and set speed
*********************************************************************************************************/
byte nvtCAN_m460::ncan_enableInterrupt(void) {
    
    byte res = 0;
    /* Enable CAN interrupt */
    //CAN_EnableInt(CAN0, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);
    /* Set Interrupt Priority */
    //NVIC_SetPriority(CAN0_IRQn, (1 << __NVIC_PRIO_BITS) - 2);
    /* Enable External Interrupt */
    //NVIC_EnableIRQ(CAN0_IRQn);

    //GPIO_SetMode(PB, BIT2, GPIO_PMD_OUTPUT);
    //PB2=1;
   
    return res;
}

/*********************************************************************************************************
** Function name:           ncan_disableInterrupt
** Descriptions:            disable can interrupt
*********************************************************************************************************/
byte nvtCAN_m460::ncan_disableInterrupt(void) {
    
    byte res = 0;
    /* Disable CAN interrupt */
    //CAN_DisableInt(CAN0, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);
    /* Disable External Interrupt */
    //NVIC_DisableIRQ(CAN0_IRQn);

    //GPIO_SetMode(PB, BIT2, GPIO_PMD_OPEN_DRAIN);

    return res;
}

/*********************************************************************************************************
** Function name:           init_Mask
** Descriptions:            init msg ID mask
*********************************************************************************************************/
byte nvtCAN_m460::init_Mask(byte num, byte ext, unsigned long ulData)
{
    /* configuration change enable */
    CANFD0->CCCR = CANFD_CCCR_INIT_Msk;
    CANFD0->CCCR |= CANFD_CCCR_CCE_Msk;
    
	
	/* set GFC to reject all unmatched Msg, keep Msg in filter*/
	gfcconfig = CANFD_GFC_REJEALL;
	ncan_configGFC(gfcconfig);
	
	/* set Mask */
	//skip, since M460 CAN-FD stdDriver API handle these items, combined with filter
	//std ID: CANFD_SetSIDFltr
	//ext ID: CANFD_SetXIDFltr
	
	/* Set to normal mode*/
	ncan_syncInit();
	
	
	byte res = 0x00;

    return res;
}


/*********************************************************************************************************
** Function name:           init_Filt
** Descriptions:            init msg ID filter
*********************************************************************************************************/
byte nvtCAN_m460::init_Filt(byte num, byte ext, unsigned long ulData)
{
    byte res =0;
	/* configuration change enable */
    CANFD0->CCCR = CANFD_CCCR_INIT_Msk;
    CANFD0->CCCR |= CANFD_CCCR_CCE_Msk;
	
	/* set Filter */
	
	if(ext==0)//StdID, use rx message buffer 0
	{
		CANFD_SetSIDFltr(CANFD0, num, CANFD_RX_BUFFER_STD(ulData, CANFD_RXBUF0));
		
	}
	else if(ext==1)//ExtID, use rx message buffer 0
	{
		CANFD_SetXIDFltr(CANFD0, num, CANFD_RX_BUFFER_EXT_LOW(ulData, CANFD_RXBUF0), CANFD_RX_BUFFER_EXT_HIGH(ulData, CANFD_RXBUF0));
	}
	else 
		res = -1;
	/* Set to normal mode*/
	ncan_syncInit();
       
    return (res);
}




/*********************************************************************************************************
** Function name:           ncan_init
** Descriptions:            init ncan
*********************************************************************************************************/
byte nvtCAN_m460::ncan_init(const byte canSpeed, const byte clock) {
    
    byte res = 0;
    return res;
}

#if CANFD_MAX_COUNT > 0
#define CAN_ID0 0
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* CAN0 interrupt handler                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle CAN interrupt event                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void CANFD0_IRQHandler(void)
{
    uint32_t u8IIDRstatus;
    CANFD_FD_MSG_T* prxCANMsg;
    //u8IIDRstatus = CANFD0->IIDR; /*Read Interrupr Identifier Register*/
  

   
}

static void CANFD_0_Init(void) 
{	
	/*
	    Set CANFD MFP
	*/
	CANFD_Config(CANFD_Desc[CAN_ID0]);	
}

static byte CANFD_0_SetConfig(uint8_t u8OpMode, uint32_t u32normalBitRate, uint32_t u32dataBitRate ) 
{	
	CANFD_FD_T sCANFD_Config;
	uint32_t nRate;
	uint32_t bRate;
	//Mode check 
	
	
	if(u8OpMode == CANFD_OP_CAN_MODE)//Normal
	{
		nRate = u32normalBitRate;
		bRate = 0;
	}
	else if(u8OpMode == CANFD_OP_CAN_FD_MODE)//FD　mode
	{
	    nRate = u32normalBitRate;
		bRate = u32dataBitRate;
	}
	else//Other mode
	{
		return -1;
	}
	
	/* Get the CAN FD configuration value */
    CANFD_GetDefaultConfig(&sCANFD_Config, u8OpMode);
    sCANFD_Config.sBtConfig.sNormBitRate.u32BitRate = nRate;
    sCANFD_Config.sBtConfig.sDataBitRate.u32BitRate = bRate;
	
    /* Open the CAN FD feature */
    CANFD_Open(CANFD0, &sCANFD_Config);
	
	NVIC_EnableIRQ(CANFD00_IRQn);
	
	/* CAN Run to Normal mode */
    //CANFD_RunToNormal(CANFD0, TRUE);
	
	return 0;
	
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


/* Configure CAN interrupt resources */
static void __initializeCAN() {
	
    callbackCAN0 = NULL;
    NVIC_EnableIRQ(CANFD00_IRQn);
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

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
