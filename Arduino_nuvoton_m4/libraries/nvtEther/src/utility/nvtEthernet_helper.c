/**************************************************************************//**
 * @file     m460_emac.c
 * @version  V3.00
 * @brief    M460 EMAC driver source file
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2022 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "NuMicro.h"

static size_t _Trace(const char *s)
{
    int i = 0 ;

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

size_t nvtEthernet_printf(const char *format, ...)
{
#ifndef PPRINTF_BUFSIZE
#define PPRINTF_BUFSIZE 256
#endif

    char buf[PPRINTF_BUFSIZE];

    va_list ap;
    va_start(ap, format);
    vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);

    return (_Trace(buf));
}
