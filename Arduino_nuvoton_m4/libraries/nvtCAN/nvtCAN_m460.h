/**************************************************************************//**
 * @file     nvtCAN_m460.h
 * @version  V1.00
 * @brief    M4 Series of Arduino CAN Library Header File
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef _NVTCAN_M460_H_
#define _NVTCAN_M460_H_



#include "Arduino.h"
#include "Pins_arduino.h"
#include "mcp_can.h"
#include "canfd.h"
#include "nvtCAN_dfs_m460.h"

typedef struct _RxMsgAndMskType
{
    uint32_t u32IDType;
    uint32_t u32ID; 
    unsigned long ulIDMask;
} RxMsgAndMskType;


#define MAX_CHAR_IN_MESSAGE 8
#define NVT_MAXFILTER_NUM 6


//Dedinition for CAN-FD Buffer and FIFO resources
#define CANFD_TXBUF0  (0)
#define CANFD_TXBUF1  (1)

#define CANFD_RXBUF0  (0)
#define CANFD_RXBUF1  (1)
#define CANFD_RXFIFO0 (0)
#define CANFD_RXFIFO1 (1)

#define CANFD_GFC_KEEPALL    (0)
#define CANFD_GFC_REJEALL    (1)

#define IS_FD          (1)
class nvtCAN_m460
{
public:
    
    nvtCAN_m460(byte _CANSEL);
    
public:
    /*CAN operator function*/
    byte begin(uint32_t speedset);
    byte begin(uint32_t normalspeed, uint32_t dataspeed, uint32_t opmode); 
    byte beginCANIntLBKMode(uint32_t speedset);

	byte setMode(uint32_t opmode);
	byte getMode(void)              { return _canmode;        }
	
    byte setMsgFilter(uint32_t en);
	
	
	uint32_t getSetSpeed(void)     { return _canspeed_set;   }
	uint32_t getNormalSpeed(void)  { return _normalspeed_set;}
    uint32_t getDataSpeed(void)    { return _dataspeed_set;  } 
    
	
    byte init_Mask(byte num, byte ext, unsigned long ulData);                                                                                   // init Masks
    byte init_Filt(byte num, byte ext, unsigned long ulData);                                                                                   // init filters
   
    byte checkReceive(void);                                                                                                                    // if something received
   
    byte readMsgBuf(byte *len, byte *buf);
    byte readMsgBufID(unsigned long *ID, byte *len, byte *buf);
   
    byte sendMsgBuf(unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte *buf);                                  // send message buf
 
    byte sendMsgBufwMsgObj(byte status, unsigned long id, byte ext, byte rtrBit, byte len, volatile const byte *buf);              // send message buf by using Msg Object
    
    /* could be called after a successful readMsgBufID() */
    unsigned long getCanId(void) { return can_id; }
    byte isRemoteRequest(void)   { return rtr;    }
    byte isExtendedFrame(void)   { return ext_flg;}
 

    byte sendMsgBuf(unsigned long id, byte ext, byte len, volatile const byte *buf);
    byte sendMsgBuf(unsigned long id, byte ext, byte len, volatile const byte* buf, uint8_t is_fd);
	
	uint32_t BaudRateSelector(uint32_t u32mcpBaudRate);
	byte BaudRateParser(uint32_t u32mcpBaudRate);
	
private:
    /*Nuvoton CAN controller(ccan) driver function */
    void ncan_reset(void); // reset ncan
    void ncan_resetIF(uint8_t u8IF_Num);//clear IF reg, for test_basic mode Rx 
    int32_t ncan_syncInit(void);    
	
	uint8_t ncan_configGFC(uint8_t gfc_config);//keep in FIFO or reject unmatched CAN msg. 
 
    byte ncan_enableInterrupt(void);
    byte ncan_disableInterrupt(void);
 
    //byte ncan_configRate(const uint32_t canSpeed, const byte clock); // set baudrate
    byte ncan_init(const byte canSpeed, const byte clock);       // ncan init

    byte sendMsg(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf, bool wait_sent = true); // send message
    
public:    
    RxMsgAndMskType rxMsgAMsk[NVT_MAXFILTER_NUM];
    /*functions to access static member data*/
    //static CANFD_FD_MSG_T* getrxCANMsgPtr() { return &(sRxMsgFrame); }
    static void setg32IIDRStatus(uint32_t val) { g32IIDRStatus = val; }
    

private:
    byte nReservedTx; // Count of tx buffers for reserved send
    union{
        void *_vncan;
        CANFD_T *ncan;
    };
    uint32_t module;
    //uint32_t opmode;
	uint32_t _canmode;
	uint32_t _rxmode;
	
	uint32_t _canspeed_set = 0;
	uint32_t _normalspeed_set = 0;
	uint32_t _dataspeed_set = 0;
	
	uint8_t  gfcconfig =  CANFD_GFC_KEEPALL;//demo purpose, let gfc keepall when no id/xid masks are set.
	uint8_t  idfilter_set = 0;
	uint8_t  xidfilter_set = 0;
	
   
    
	byte nCANSel;
    IRQn_Type id;
    byte ext_flg; 			// identifier xxxID, either extended (the 29 LSB) or standard (the 11 LSB)
    unsigned long can_id;   // can id
    byte rtr;               // is remote frame
	byte fdformat;          // FD format
	byte switchBR;          // Bit rate switching for FD
	
    /*static member data*/
    
    static uint32_t g32IIDRStatus;
    //static CANFD_FD_MSG_T sRxMsgFrame;
};

#ifdef __cplusplus
extern "C" {
#endif
byte BaudRateCheck(uint32_t u32BaudRate, uint32_t u32RealBaudRate);
static void CANFD_0_Init(void);
static byte CANFD_0_SetConfig(uint8_t u8OpMode, uint32_t u32normalBitRate, uint32_t u32dataBitRate ); 
static interruptCB callbackCAN0;
void attachInterruptCAN(void (*callback)(void));
#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
