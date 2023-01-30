/**************************************************************************//**
 * @file     nvtCAN.h
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 1/30/23 2:33p $
 * @brief    NUC131 Series of Arduino CAN Library Header File
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef _NVTCAN_H_
#define _NVTCAN_H_



#include "Arduino.h"
#include "Pins_arduino.h"
#include "mcp_can.h"
#include "nvtCAN_dfs.h"

typedef struct _RxMsgAndMskType
{
    uint32_t u32IDType;
    uint32_t u32ID; 
    unsigned long ulIDMask;
} RxMsgAndMskType;


#define MAX_CHAR_IN_MESSAGE 8
#define NVT_MAXFILTER_NUM 6
class nvtCAN //: public MCP_CAN
{
public:
    
    
    nvtCAN(byte _CANSEL);
    /*
        Nuvoton CAN controller(ccan) driver function
    */
public:

    virtual byte begin(uint32_t speedset, const byte clockset = MCP_16MHz);
                                                                           
    virtual byte init_Mask(byte num, byte ext, unsigned long ulData);                                                                                   // init Masks
    virtual byte init_Filt(byte num, byte ext, unsigned long ulData);                                                                                   // init filters
   
    virtual byte checkReceive(void);                                                                                                                    // if something received
   
    virtual byte readMsgBuf(byte *len, byte *buf);
    virtual byte readMsgBufID(unsigned long *ID, byte *len, byte *buf);
   
    virtual byte sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte *buf);                                  // send message buf by using parsed buffer status
 
    virtual byte sendMsgBufwMsgObj(byte status, unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte *buf);                                  // send message buf by using parsed buffer status
    
    /* could be called after a successful readMsgBufID() */
    unsigned long getCanId(void) { return can_id; }
    byte isRemoteRequest(void)   { return rtr;    }
    byte isExtendedFrame(void)   { return ext_flg;}
 

    byte sendMsgBuf(unsigned long id, byte ext, byte len, volatile const byte *buf);

private:
    void ncan_reset(void); // reset ncan

     void ncan_resetIF(uint8_t u8IF_Num);//clear IF reg, for test_basic mode Rx 

     byte ncan_enableInterrput(void);

     void ncan_disableInterrupt();

 
    byte ncan_configRate(const uint32_t canSpeed, const byte clock); // set baudrate
    byte ncan_init(const byte canSpeed, const byte clock);       // ncan init

  
    /*
        can operator function
    */

    byte sendMsg(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf, bool wait_sent = true); // send message


    /*
        auxillary function
    */
 

public:    
    uint32_t ncan_readStatus(void);
    //STR_CANMSG_T rxCANMsg;
    RxMsgAndMskType rxMsgAMsk[NVT_MAXFILTER_NUM];

private:
    byte nReservedTx; // Count of tx buffers for reserved send

	union{
		void *_vncan;
		CAN_T *ncan;

	};
	uint32_t module;
    uint32_t opmode;
    uint32_t canspeed_set;
    byte nCANSel;
	IRQn_Type id;
    byte ext_flg; // identifier xxxID
    // either extended (the 29 LSB) or standard (the 11 LSB)
    unsigned long can_id; // can id
    byte rtr;             // is remote frame
    
};

#ifdef __cplusplus
extern "C" {
#endif
byte BaudRateCheck(uint32_t u32BaudRate, uint32_t u32RealBaudRate);
uint32_t BaudRateSelector(uint32_t u32mcpBaudRate);
static void CAN_0_Init(void);

#ifdef __cplusplus
}
#endif
static STR_CANMSG_T rxCANMsg;
static uint32_t g32IIDRStatus;
#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
