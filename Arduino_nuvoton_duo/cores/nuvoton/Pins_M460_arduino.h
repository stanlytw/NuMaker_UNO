/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _PINS_M460_ARDUINO_H_
#define _PINS_M460_ARDUINO_H_

#if defined(__M460__)&& defined(__M467HJHAN__)
/* Types used for the tables below */
typedef struct _PinType
{
    uint32_t num;
    uint32_t type;
} PinType;

typedef struct _PinDescription
{
    uint32_t MFP;
    uint32_t Mask;
    uint32_t Type;
} PinDescription;

typedef struct _GPIOPinDescription
{
    GPIO_T *P;
    uint32_t bit;
    PinDescription Pin;
} GPIOPinDescription;

typedef struct _ADCPinDescription
{
    EADC_T *A;
    uint32_t module;
    uint32_t ch;
    PinType pintype;
} ADCPinDescription;

typedef struct _EPWMPinDescription
{
    EPWM_T  *P;
    uint32_t module;
	uint32_t moduletype;
    IRQn_Type irq;
    uint32_t ch;
    uint32_t freq;
    PinType pintype;
} EPWMPinDescription;

typedef struct _SPIPinDescription
{
    union {
        void *V;
        SPI_T *S;
        USPI_T *U;
    };

    uint32_t module;
    IRQn_Type irq;
    uint32_t clksel;
    PinType pintype[4];
} SPIPinDescription;

typedef struct _UARTPinDescription
{
    UART_T *U;
    uint32_t module;
	uint32_t  modulenum;
    IRQn_Type irq;
    PinType pintype[2];
    uint32_t clksrcsel;
    uint32_t clkdiv;
} UARTPinDescription;

typedef struct _I2CPinDescription
{
    I2C_T *I;
    uint32_t module;
    PinType pintype[2];
} I2CPinDescription;

typedef struct _CANFDPinDescription
{
    CANFD_T *CFD;
    uint32_t module;
	uint32_t  modulenum;
    IRQn_Type irq;
    PinType pintype[2];
    //uint32_t clksrcsel;
    //uint32_t clkdiv;
} CANFDPinDescription;

typedef struct _BoardToPin
{
    int32_t pin;
    uint8_t type;
    uint8_t num;
} BoardToPin;


/* Board definition */
#ifdef USE_BoardToPin
#define GPIO_TYPE   0
#define UART_TYPE   1
#define I2C_TYPE    2
#define SPI_TYPE    3
#define ADC_TYPE    4
#define PWM_TYPE    5
#define CAN_TYPE    6

#define BoardToPin_MAX_COUNT 45
extern BoardToPin BoardToPinInfo[];
#endif

/*========== GPIO definition ==========*/
#define GPIO_MAX_COUNT 129
extern GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)

/*========== PWM definition ==========*/
#define PWM_MAX_COUNT 1
#define PWM_USE_BPWM              (0xE0)
#define PWM_USE_EPWM              (0xE1)
#define PWM_CHANNEL_CH0           (0x00)
#define PWM_CHANNEL_CH1           (0x01)
#define PWM_CHANNEL_CH2           (0x02)
#define PWM_CHANNEL_CH3           (0x03)
#define PWM_CHANNEL_CH4           (0x04)
#define PWM_CHANNEL_CH5           (0x05)
#define PWM_FREQUENCY_500HZ       (500)
#define PWM_DESC_IDX0             (0x00)
#define PWM_DESC_IDX1             (0x01)
#define PWM_DESC_IDX2             (0x02)
#define PWM_DESC_IDX_NOUSE        (0xF0)
extern EPWMPinDescription PWM_Desc[];
#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);

/*========== ADC definition ==========*/
#define ADC_MAX_COUNT 6
extern ADCPinDescription ADC_Desc[];
#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);

/*========== SPI definition ==========*/
//#define SPI_MAX_COUNT 1// Kept SPI1 PA8~PA11
#define SPI_MAX_COUNT 2//[2024-11-06]Add support to UNO nu5 connector
#define SPI_CHANNELS_NUM 1
extern SPIPinDescription SPI_Desc[];

// #define SPI_SCK   0 //defined but not used
// #define SPI_MOSI  1 //defined but not used
// #define SPI_MISO  2 //defined but not used
// #define SPI_SS    3 //defined but not used
#define SPI_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<4;i++) \
     if(Desc.pintype[i].num != 0) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

/*========== UART definition ==========*/
#define UART_MAX_COUNT 2
extern UARTPinDescription UART_Desc[];
extern PinType DEPin_Desc[];
#define UART_RX 0
#define UART_TX 1
#define UART_USE_UART0           (0x00)
#define UART_USE_UART1           (0x01)
#define UART_USE_UART2           (0x02)
#define UART_USE_UART3           (0x03)
#define UART_USE_UART4           (0x04)
#define UART_USE_VCOM            (0x0C)
#define UART_USE_NOUSE           (0x0F)
#define UART_CLKDIV_1            (0x01)
#define UART_CLKDIV_2            (0x02)
#define UART_DESC_IDX0           (0x00)
#define UART_DESC_IDX1           (0x01)
#define UART_DESC_IDX2           (0x02)

#define RS485_OVER_SERIAL        (0x85)
#define UART_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<2;i++) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

#define DE_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<(1);i++) \
        outp32(GPIO_Desc[Desc.num].Pin.MFP,(inp32(GPIO_Desc[Desc.num].Pin.MFP) & ~GPIO_Desc[Desc.num].Pin.Mask) | Desc.type); \
}while(0);

/*========== I2C definition ==========*/
#define I2C_MAX_COUNT 1

extern I2CPinDescription I2C_Desc[];
#define I2C_SDA 0
#define I2C_SCL 1
#define I2C_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<2;i++) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

/*========== CANFD definition ==========*/
#define CANFD_MAX_COUNT 1
extern CANFDPinDescription CANFD_Desc[];
#define CANFD_CLKDIV_1           (0x01)
#define CANFD_USE_CANFD0         (0x00)
#define CANFD_USE_CANFD1         (0x01)

#define CANFD_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<2;i++) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);



/*========== Arduino PIN mapping definition ==========*/
static const uint8_t SS   = 41;
static const uint8_t MOSI = 43;
static const uint8_t MISO = 40;
static const uint8_t SCK  = 42;
static const uint8_t LED_BUILTIN = 13;
static const uint8_t SDA = 14;
static const uint8_t SCL = 15;

static const uint8_t A0  = 0;
static const uint8_t A1  = 1;
static const uint8_t A2  = 2;
static const uint8_t A3  = 3;
static const uint8_t A4  = 4;
static const uint8_t A5  = 5;
static const uint8_t A6  = 6;
static const uint8_t A7  = 7;
static const uint8_t A8  = 8;
static const uint8_t A9  = 9;



#endif
#endif