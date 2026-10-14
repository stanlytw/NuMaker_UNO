/*
  EEPROM.h - EEPROM library
  Copyright (c) 2006 David A. Mellis.  All right reserved.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>
#include "Wire.h"

#define DEFAULT_EEPROM_ID    (0x50)

class EEPROMClass
{
public:
    EEPROMClass();
    void begin();
    void begin(uint8_t id);
    //To support 2-byte address case
    uint8_t read(int);
    void write(int, uint8_t);
    void update(int, uint8_t);
    uint8_t get(int address);
    uint16_t length()         {
        return 0xFFF + 1;
    }
    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T &get( int idx, T &t ) {
        uint8_t *ptr = (uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++idx )  *ptr++ = read(idx);
        return t;
    }

    template< typename T > const T &put( int idx, const T &t ) {
        const uint8_t *ptr = (const uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++idx )  write(idx, (*ptr++));
        return t;
    }


    uint8_t eeprom_id;
private:
    bool begin_done;
};

extern EEPROMClass EEPROM;

#endif

