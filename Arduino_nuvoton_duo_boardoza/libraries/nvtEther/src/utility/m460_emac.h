/**************************************************************************//**
 * @file     m460_emac.h
 * @version  V3.00
 * @brief    M460 EMAC driver header file
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2023 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef  __M460_EMAC_H__
#define  __M460_EMAC_H__

#include "NuMicro.h"
#include "synopGMAC_network_interface.h"
#include "nvtEthernet_helper.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t  EMAC_Open(uint8_t *macaddr);
uint32_t EMAC_ReceivePkt(PKT_FRAME_T **psPktFrame);
void EMAC_ReceivePkt_Free(PKT_FRAME_T *psPktFrame);
int32_t  EMAC_TransmitPkt(uint8_t *pbuf, uint32_t len);
uint8_t *EMAC_AllocatePktBuf(void);

#ifdef __cplusplus
}
#endif
#endif  /* __M460_EMAC_H__ */
