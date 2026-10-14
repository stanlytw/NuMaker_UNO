/**************************************************************************//**
 * @file     SDGlue.h
 * @version  V1.00
 * @brief    SD glue functions header for FATFS
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef  __SDGLUE_H__
#define  __SDGLUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NuMicro.h"
#include "sdh.h"
#include "./fatfs/diskio.h"     /* FatFs lower layer API */
#include "./fatfs/ff.h" 

//Macro
#if defined(__M467SJHAN__)
#define TARGET_SDH SDH1
#define DEF_CARD_DETECT_SOURCE       CardDetect_From_DAT3
#else//HJ
#define TARGET_SDH SDH0
#define DEF_CARD_DETECT_SOURCE       CardDetect_From_GPIO
#endif

//Struct Definition
typedef struct
{
   uint32_t CardType;                     /*!< Specifies the card Type                         */
   uint32_t CardVersion;                  /*!< Specifies the card version                      */
   uint32_t Class;                        /*!< Specifies the class of the card class           */
   uint32_t RelCardAdd;                   /*!< Specifies the Relative Card Address             */
   uint32_t BlockNbr;                     /*!< Specifies the Card Capacity in blocks           */
   uint32_t BlockSize;                    /*!< Specifies one block size in bytes               */
   uint32_t LogBlockNbr;                  /*!< Specifies the Card logical Capacity in blocks   */
   uint32_t LogBlockSize;                 /*!< Specifies logical block size in bytes           */
} _SD_CardInfo;

//Function Prototypes
#ifdef __cplusplus
extern "C" {
#endif
int SDGlue_Open_Disk(SDH_T *sdh, uint32_t u32CardDetSrc);
void SDGlue_Close_Disk(SDH_T *sdh);
int8_t SDGlue_Get_CardInfo(_SD_CardInfo * psdCardInfo);
void SDH0_IRQHandler(void);//[2024-11-20]
void SDH1_IRQHandler(void);//[2025-03-05]

#ifdef __cplusplus
}
#endif

#endif  /* __SDGLUE_H__ */
