/*
  This file is part of the ArduinoBLE library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _HCI_NUVOTON_TRANSPORT_H_
#define _HCI_NUVOTON_TRANSPORT_H_

#include "HCITransport.h"

typedef enum
{
    eNUVOTON_TRANSPORT_FAIL          = 0,
    eNUVOTON_TRANSPORT_SUCCESS       = 1,
} HCINuvotonTransportErrorCode;

class HCINuvotonTransportClass : public HCITransportInterface
{
public:
    HCINuvotonTransportClass(bool);
    virtual ~HCINuvotonTransportClass();

    virtual int begin();
    virtual void end();

    virtual void wait(unsigned long timeout);

    virtual int available();
    virtual int peek();
    virtual int read();

    friend void HCI_EVENT_CALLBACK(uint8_t *pHCIbufAddr, uint16_t length);

    HCINuvotonTransportErrorCode readRawData2ParserBuf(uint8_t *, uint16_t);

    virtual size_t write(const uint8_t *data, size_t length);

    void cb_clrDataReady(void)
    {
        _bleDataReady = 0;
    };
    uint16_t getRcvLen(void)
    {
        return _u16rcv_length;
    };

    void setRandomSeed(uint32_t seed)
    {
        _u32seed = seed;
    };
    uint32_t getRandomNumberSeed()
    {
        return _u32seed;
    };
    void clrRandomSeed()
    {
        _u32seed = 0;
    };

private:
    void _cb_setDataReady(uint8_t *pHCIbufAddr, uint16_t length);
    void _nuvoton_BLE_LL_init(void);
    void _init_peripheral_for_LL(void);
    BleStackStatus _BLE_StackInit(void);

    volatile bool _bleDataReady;
    uint8_t *_pu8_rawdataBuf;
    uint16_t _u16rcv_length;
    bool _begun;
    volatile uint32_t _u32seed;
};

extern HCINuvotonTransportClass &HCINuvotonTransport;

#endif
