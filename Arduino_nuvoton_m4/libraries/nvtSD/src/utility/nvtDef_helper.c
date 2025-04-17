/**************************************************************************//**
 * @file     nvtDef_helper.c
 * @version  V1.00
 * @brief    M460 helper funtion source file
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2022 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "NuMicro.h"
//#define NVT_DBG_MSG
static size_t _Trace(const char *s)
{
    int i = 0 ;
    while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
    DEBUG_PORT->DAT = (uint32_t)'_';
    
    while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
    DEBUG_PORT->DAT = (uint32_t)'N';

    while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
    DEBUG_PORT->DAT = (uint32_t)'V';

    while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
    DEBUG_PORT->DAT = (uint32_t)'T';

    while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
    DEBUG_PORT->DAT = (uint32_t)'-';

    while (s[i] != '\0')
    {
        while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
        DEBUG_PORT->DAT = (uint32_t)s[i];

        i++;
    }

    while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
    DEBUG_PORT->DAT = (uint32_t)'\r';

    while (DEBUG_PORT->FIFOSTS & UART_FIFOSTS_TXFULL_Msk) {}
    DEBUG_PORT->DAT = (uint32_t)'\n';

    return i;
}

size_t nvtDbg_printf(const char *format, ...)
{
#ifdef NVT_DBG_MSG
#ifndef PPRINTF_BUFSIZE
#define PPRINTF_BUFSIZE 256
#endif

    char buf[PPRINTF_BUFSIZE];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

    return (_Trace(buf));
#endif
}
