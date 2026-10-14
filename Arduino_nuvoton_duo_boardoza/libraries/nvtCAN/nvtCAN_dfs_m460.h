/**************************************************************************//**
 * @file     nvtCAN_dfs.h
 * @version  V1.00
 * @brief    NUC131 Series of Arduino CAN Library Macro Definition Header File
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#ifndef _NVTCANDFS_H_
#define _NVTCANDFS_H_

#include "Arduino.h"
#include "nvtCAN_m460.h"
#include <inttypes.h>


#ifndef DEBUG_EN
    #define DEBUG_EN        1
#endif

#define CAN_BAUDRATE_4M        (uint32_t)(4000000)


#define CAN_BAUDRATE_750K      (uint32_t)(750000)
#define CAN_BAUDRATE_1M        (uint32_t)(1000000)
#define CAN_BAUDRATE_1M5       (uint32_t)(1500000)
#define CAN_BAUDRATE_1M        (uint32_t)(1000000)
#define CAN_BAUDRATE_2M        (uint32_t)(2000000)
#define CAN_BAUDRATE_4M        (uint32_t)(4000000)


#define CAN_BAUDRATE_1K        (uint32_t)(1000)
#define CAN_BAUDRATE_10K       (uint32_t)(10000)
#define CAN_BAUDRATE_20K       (uint32_t)(20000)
#define CAN_BAUDRATE_25K       (uint32_t)(25000)
#define CAN_BAUDRATE_40K       (uint32_t)(40000)
#define CAN_BAUDRATE_100K      (uint32_t)(100000)
#define CAN_BAUDRATE_200K      (uint32_t)(200000)
#define CAN_BAUDRATE_125K      (uint32_t)(125000)
#define CAN_BAUDRATE_250K      (uint32_t)(250000)
#define CAN_BAUDRATE_500K      (uint32_t)(500000)
#define CAN_BAUDRATE_666K      (uint32_t)(666000)
#define CAN_BAUDRATE_800K      (uint32_t)(800000)
#define CAN_BAUDRATE_1000K     (uint32_t)(1000000)



typedef enum {
    CAN_BAUDRATE_NOBPS,//0
    CAN_BAUDRATE_1KBPS,
    CAN_BAUDRATE_10KBPS,
    CAN_BAUDRATE_20KBPS,
    CAN_BAUDRATE_25KBPS,
    CAN_BAUDRATE_40KBPS  ,//5
    CAN_BAUDRATE_100KBPS ,
    CAN_BAUDRATE_125KBPS ,
    CAN_BAUDRATE_200KBPS ,
    CAN_BAUDRATE_250KBPS ,
    CAN_BAUDRATE_500KBPS ,//10
    CAN_BAUDRATE_666KBPS ,
    CAN_BAUDRATE_800KBPS ,
    CAN_BAUDRATE_1000KBPS//13
} NVT_NROMALBITTIME_SETUP;

                                                   
static const uint32_t CAN_BAUDRATE_100K_400K =  ( 4UL << 24) | (100000UL);                      
static const uint32_t CAN_BAUDRATE_200K_800K =  ( 8UL << 24) | (200000UL);

//static const uint32_t CAN_BAUDRATE_250K_500K = ( 4UL << 24) | (250000UL);
//static const uint32_t CAN_BAUDRATE_250K_750K = ( 3UL << 24) | (250000UL);
//static const uint32_t CAN_BAUDRATE_250K_1M   = ( 4UL << 24) | (250000UL);
//static const uint32_t CAN_BAUDRATE_250K_1M5  = ( 6UL << 24) | (250000UL);
//static const uint32_t CAN_BAUDRATE_250K_2M   = ( 8UL << 24) | (250000UL);
//static const uint32_t CAN_BAUDRATE_250K_3M   = (12UL << 24) | (250000UL);
//static const uint32_t CAN_BAUDRATE_250K_4M   = (16UL << 24) | (250000UL);
//static const uint32_t CAN_BAUDRATE_500K_1M   = ( 2UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_500K_2M   = ( 4UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_500K_3M   = ( 6UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_500K_4M   = ( 8UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_500K_5M   = (10UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_500K_6M5  = (13UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_500K_8M   = (16UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_500K_10M  = (20UL << 24) | (500000UL);
//static const uint32_t CAN_BAUDRATE_1000K_4M  = ( 4UL << 24) |(1000000UL);
//static const uint32_t CAN_BAUDRATE_1000K_8M  = ( 8UL << 24) |(1000000UL);



//Macros used in nvtCAN library
#define STD_ID 0
#define NVT_CAN_IDX 0

typedef void (*interruptCB)(void);
#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
