/******************************************************************************
 * @file     Arduino.h
 * @version  V2.00
 * $Date: 13/10/07 3:57p $ 
 * @brief    MINI51 Series Global Control for Arduino
 *
 * @note
 * Copyright (C) 2012 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
 
#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "m460.h"

#include "Pins_arduino.h"
#include "wiring_constants.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "wiring.h"
#include "wiring_digital.h"
#include "wiring_analog.h"
#include "WInterrupts.h"
#include "itoa.h"
#include "binary.h"
void yield(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"
#include "WMath.h"
#include "HardwareSerial.h"
#include "HardwareTimer.h"
#include "wiring_pulse.h"
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);
#endif


#if defined(__M460MINIMA__) 
#if (defined(__NVTKB__))
	#include "vcom_serial_kb.h"
#else
    #include "vcom_serial.h"
#endif
    #include "isp_user.h"
    #include "fmc_user.h"
#else
    #include "USBCore.h"
    #include "USBAPI.h"
#endif//(__M460MINIMA__) 	

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )

static volatile uint8_t init_flag=0;	
extern uint32_t SystemTickClock;

void setup(void);
void loop(void);
#endif//_ARDUINO_H_
