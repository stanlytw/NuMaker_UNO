/*
    mcp_can_dfs.h
    2012 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author:Loovee (loovee@seeed.cc)
    2014-1-16

    Contributor:

    Cory J. Fowler
    Latonita
    Woodward1
    Mehtajaghvi
    BykeBlast
    TheRo0T
    Tsipizic
    ralfEdmund
    Nathancheek
    BlueAndi
    Adlerweb
    Btetz
    Hurvajs
    xboxpro1
    ttlappalainen

    The MIT License (MIT)

    Copyright (c) 2013 Seeed Technology Inc.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
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
#define CAN_BAUDRATE_500K      (uint32_t)(500000)
#define CAN_BAUDRATE_250K      (uint32_t)(250000)
#define CAN_BAUDRATE_125K      (uint32_t)(125000)
#define CAN_BAUDRATE_100K      (uint32_t)(100000)
#define CAN_BAUDRATE_50K       (uint32_t)(50000)

#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
