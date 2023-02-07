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

#include <Arduino.h>
#include <nvtCAN.h>
#include <inttypes.h>


#ifndef DEBUG_EN
    #define DEBUG_EN        1
#endif

#define CAN_BAUDRATE_1000K     (uint32_t)(1000000)
#define CAN_BAUDRATE_800K      (uint32_t)(800000)
#define CAN_BAUDRATE_666K      (uint32_t)(666000)
#define CAN_BAUDRATE_500K      (uint32_t)(500000)
#define CAN_BAUDRATE_250K      (uint32_t)(250000)
#define CAN_BAUDRATE_200K      (uint32_t)(200000)
#define CAN_BAUDRATE_125K      (uint32_t)(125000)
#define CAN_BAUDRATE_100K      (uint32_t)(100000)
#define CAN_BAUDRATE_95K       (uint32_t)(95000)
#define CAN_BAUDRATE_83K3      (uint32_t)(83800)
#define CAN_BAUDRATE_80K       (uint32_t)(50000)
#define CAN_BAUDRATE_50K       (uint32_t)(50000)
#define CAN_BAUDRATE_40K       (uint32_t)(40000)
#define CAN_BAUDRATE_33K       (uint32_t)(33000)
#define CAN_BAUDRATE_31K25     (uint32_t)(31250)
#define CAN_BAUDRATE_25K       (uint32_t)(25000)
#define CAN_BAUDRATE_20K       (uint32_t)(20000)
#define CAN_BAUDRATE_10K       (uint32_t)(10000)
#define CAN_BAUDRATE_5K        (uint32_t)(5000)
#define CAN_BAUDRATE_NO        (uint32_t)(0)

//Macros used in nvtCAN library
#define STD_ID 0
#define NVT_CAN_IDX 0

typedef void (*interruptCB)(void);
#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
