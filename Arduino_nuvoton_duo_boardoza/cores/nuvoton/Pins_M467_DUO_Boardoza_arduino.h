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
#ifndef _PINS_M467_DUO_ARDUINO_H_
#define _PINS_M467_DUO_ARDUINO_H_

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
#define GPIO_MAX_COUNT 65//129

enum GPIODescIndex
{
    GPIO_DESC_IDX_NOUSE = -1,
    GPIO_DESC_IDX_NONE = 0,
    GPIO_DESC_IDX_PB6 = 1,
    GPIO_DESC_IDX_PB5 = 2,
    GPIO_DESC_IDX_PB4 = 3,
    GPIO_DESC_IDX_PB3 = 4,
    GPIO_DESC_IDX_PB2 = 5,
    GPIO_DESC_IDX_PB1 = 6,
    GPIO_DESC_IDX_PB0 = 7,
    GPIO_DESC_IDX_PA11 = 8,
    GPIO_DESC_IDX_PA10 = 9,
    GPIO_DESC_IDX_PA9 = 10,
    GPIO_DESC_IDX_PA8 = 11,
    GPIO_DESC_IDX_PF6 = 12,
    GPIO_DESC_IDX_VBAT = 13,
    GPIO_DESC_IDX_PF5 = 14,
    GPIO_DESC_IDX_PF4 = 15,
    GPIO_DESC_IDX_PF3 = 16,
    GPIO_DESC_IDX_PF2 = 17,
    GPIO_DESC_IDX_PC7 = 18,
    GPIO_DESC_IDX_PC6 = 19,
    GPIO_DESC_IDX_PA7 = 20,
    GPIO_DESC_IDX_PA6 = 21,
    GPIO_DESC_IDX_VDD = 22,
    GPIO_DESC_IDX_VSS = 23,
    GPIO_DESC_IDX_LDO_CAP = 24,
    GPIO_DESC_IDX_PA5 = 25,
    GPIO_DESC_IDX_PA4 = 26,
    GPIO_DESC_IDX_PA3 = 27,
    GPIO_DESC_IDX_PA2 = 28,
    GPIO_DESC_IDX_PA1 = 29,
    GPIO_DESC_IDX_PA0 = 30,
    GPIO_DESC_IDX_VDDIO = 31,
    GPIO_DESC_IDX_NRST = 32,
    GPIO_DESC_IDX_PF0 = 33,
    GPIO_DESC_IDX_PF1 = 34,
    GPIO_DESC_IDX_PC5 = 35,
    GPIO_DESC_IDX_PC4 = 36,
    GPIO_DESC_IDX_PA12 = 37,
    GPIO_DESC_IDX_PA13 = 38,
    GPIO_DESC_IDX_PA14 = 39,
    GPIO_DESC_IDX_PA15 = 40,
    GPIO_DESC_IDX_RESERVED41 = 41,
    GPIO_DESC_IDX_RESERVED42 = 42,
    GPIO_DESC_IDX_RESERVED43 = 43,
    GPIO_DESC_IDX_RESERVED44 = 44,
    GPIO_DESC_IDX_RESERVED45 = 45,
    GPIO_DESC_IDX_RESERVED46 = 46,
    GPIO_DESC_IDX_RESERVED47 = 47,
    GPIO_DESC_IDX_RESERVED48 = 48,
    GPIO_DESC_IDX_RESERVED49 = 49,
    GPIO_DESC_IDX_RESERVED50 = 50,
    GPIO_DESC_IDX_RESERVED51 = 51,
    GPIO_DESC_IDX_PC14 = 52,
    GPIO_DESC_IDX_PB15 = 53,
    GPIO_DESC_IDX_PB14 = 54,
    GPIO_DESC_IDX_PB13 = 55,
    GPIO_DESC_IDX_PB12 = 56,
    GPIO_DESC_IDX_RESERVED57 = 57,
    GPIO_DESC_IDX_RESERVED58 = 58,
    GPIO_DESC_IDX_RESERVED59 = 59,
    GPIO_DESC_IDX_PB11 = 60,
    GPIO_DESC_IDX_PB10 = 61,
    GPIO_DESC_IDX_PB9 = 62,
    GPIO_DESC_IDX_PB8 = 63,
    GPIO_DESC_IDX_PB7 = 64
};

extern GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)

/*========== PWM definition ==========*/
#define PWM_MAX_COUNT 6
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
#define PWM_DESC_IDX3             (0x03)
#define PWM_DESC_IDX4             (0x04)
#define PWM_DESC_IDX5             (0x05)
#define PWM_DESC_IDX_NOUSE        (0xF0)

extern EPWMPinDescription PWM_Desc[];
#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);

/*========== ADC definition ==========*/
#define ADC_MAX_COUNT 6
#define ADC_USE_EADC0             (0xD0)
#define ADC_USE_EADC1             (0xD1)
#define ADC_CHANNEL_CH0           (0)
#define ADC_CHANNEL_CH1           (1)
#define ADC_CHANNEL_CH2           (2)
#define ADC_CHANNEL_CH3           (3)
#define ADC_CHANNEL_CH4           (4)
#define ADC_CHANNEL_CH5           (5)
#define ADC_CHANNEL_CH6           (6)
#define ADC_CHANNEL_CH7           (7)
#define ADC_CHANNEL_CH8           (8)
#define ADC_CHANNEL_CH9           (9)
#define ADC_CHANNEL_CH10          (10)
#define ADC_CHANNEL_CH11          (11)
#define ADC_CHANNEL_CH12          (12)
#define ADC_CHANNEL_CH13          (13)
#define ADC_CHANNEL_CH14          (14)
#define ADC_CHANNEL_CH15          (15)

#define ADC_DESC_IDX0             (0x00)
#define ADC_DESC_IDX1             (0x01)
#define ADC_DESC_IDX2             (0x02)
#define ADC_DESC_IDX3             (0x03)
#define ADC_DESC_IDX4             (0x04)
#define ADC_DESC_IDX5             (0x05)



extern ADCPinDescription ADC_Desc[];
#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);

/*========== SPI definition ==========*/
#define SPI_MAX_COUNT 2//[2025-11-26]
#define SPI_CHANNELS_NUM 1
extern SPIPinDescription SPI_Desc[];

#define SPI_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<4;i++) \
     if(Desc.pintype[i].num != 0) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

/*========== UART definition ==========*/
#define UART_MAX_COUNT 1// __DUO__
extern UARTPinDescription UART_Desc[];
extern PinType DEPin_Desc[];
#define UART_RX 0
#define UART_TX 1
#define UART_USE_UART0           (0x00)
#define UART_USE_UART1           (0x01)
#define UART_USE_UART2           (0x02)
#define UART_USE_UART3           (0x03)
#define UART_USE_UART4           (0x04)
#define UART_USE_UART5           (0x05)
#define UART_USE_UART6           (0x06)
#define UART_USE_UART7           (0x07)
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
    for(i=0;i<(2);i++) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

#define DE_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<(1);i++) \
        outp32(GPIO_Desc[Desc.num].Pin.MFP,(inp32(GPIO_Desc[Desc.num].Pin.MFP) & ~GPIO_Desc[Desc.num].Pin.Mask) | Desc.type); \
}while(0);


/*========== I2C definition ==========*/
#define I2C_MAX_COUNT 1 //Sync to R4

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
#define CANFD_MAX_COUNT 1  //Sync to R4
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
static const uint8_t SDA = 14;
static const uint8_t SCL = 15;
static const uint8_t LED_BUILTIN = 7;

/*
   ADC pin number(ulPin)does not use USE_BoardToPin scheme.
   example : ADC_Desc[ulPin]
*/
static const uint8_t A0  = 0;//0th element in ADC_Desc[]
static const uint8_t A1  = 1;//1th element in ADC_Desc[]
static const uint8_t A2  = 2;//2th element in ADC_Desc[]
static const uint8_t A3  = 3;//3th element in ADC_Desc[]
static const uint8_t A4  = 4;//4th element in ADC_Desc[]
static const uint8_t A5  = 5;//5th element in ADC_Desc[]

static const uint8_t D0  = 0;
static const uint8_t D1  = 1;
static const uint8_t D2  = 2;
static const uint8_t D3  = 3;
static const uint8_t D4  = 4;
static const uint8_t D5  = 5;
static const uint8_t D6  = 6;
static const uint8_t D7  = 7;
static const uint8_t D8  = 8;
static const uint8_t D9  = 9;
static const uint8_t D10  = 10;
static const uint8_t D11  = 11;
static const uint8_t D12  = 12;
static const uint8_t D13  = 13;
static const uint8_t D14  = 14;
static const uint8_t D15  = 15;
static const uint8_t D16  = 16;
static const uint8_t D17  = 17;
static const uint8_t D18  = 18;
static const uint8_t D19  = 19;

#endif//_PINS_M467_DUO_ARDUINO_H_