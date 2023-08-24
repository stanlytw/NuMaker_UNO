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
#include "Arduino.h"
#include "Pins_arduino.h"

#if defined(__M032KG__)
#ifdef USE_BoardToPin
BoardToPin BoardToPinInfo[] =
{
    {  0, UART_TYPE, 0}, //  PB2(UART1_RX)   (D0/UART_RX0)
    {  1, UART_TYPE, 0}, //  PB3(UART1_TX)   (D1/UART_TX0)
    {  2, PWM_TYPE,  0}, //  PC4(PWM_CH)     (D2/PWM0)
    {  3, PWM_TYPE,  1}, //  PC5(PWM_CH)     (D3/PWM1)
    {  4, PWM_TYPE,  2}, //  PC3(PWM_CH)     (D4/PWM2)
    {  5, PWM_TYPE,  3}, //  PC2(PWM_CH)     (D5/PWM3)
    {  6, PWM_TYPE,  4}, //  PA7(PWM_CH)     (D6/PWM4)
    {  7, PWM_TYPE,  5}, //  PA6(PWM_CH)     (D7/PWM5)

    { 16, ADC_TYPE, 0},  // PB7(EADC_CH)    (A0)
    { 17, ADC_TYPE, 1},  // PB6(EADC_CH)    (A1)
    { 18, ADC_TYPE, 2},  // PB5(EADC_CH)    (A2)
    { 19, ADC_TYPE, 3},  // PB4(EADC_CH)    (A3)
    { 20, ADC_TYPE, 4},  // PB0(EADC_CH)    (A4/I2C1_SDA)
    { 21, ADC_TYPE, 5},  //13 PB1(EADC_CH)    (A5/I2C1_SCL)

    { 8, PWM_TYPE,  6},  //14 PA5(PWM0_CH1)   (D8/PWM6)
    { 9, PWM_TYPE,  7},  //15 PA4(PWM0_CH0)   (D9/PWM7)
    { 10, SPI_TYPE,  0}, //16 PA3(SPI0_CS)    (D10/SPI0_CS)
    { 11, SPI_TYPE,  0}, //17 PA0(SPI0_MOSI)  (D11/SPI0_MOSI)
    { 12, SPI_TYPE,  0}, //18 PA1(SPI0_MISO)  (D12/SPI0_MISO)
    { 13, SPI_TYPE,  0}, //19 PA2(SPI0_SCK)   (D13/SPI0_SCK)
    { 14, I2C_TYPE,  0}, //20 PC0(I2C0_SDA)   (I2C0_SDA)
    { 15, I2C_TYPE,  0}, //21 PC1(I2C0_SCL)   (I2C0_SCL)

    { -1,        0, 0}, //22: NC
    { -1,        0, 0}, //23: NC
    { -1,        0, 0}, //24: NC
    { -1,        0, 0}, //25: NC
    { -1,        0, 0}, //26: NC
    { -1,        0, 0}, //27: NC
    { -1,        0, 0}, //28: NC
    { -1,        0, 0}, //29: NC
    { -1,        0, 0}, //30: NC
    { -1,        0, 0}, //31: NC
    { -1,        0, 0}, //32: NC
    { -1,        0, 0}, //33: NC
    { -1,        0, 0}, //34: NC
    { -1,        0, 0}, //35: NC
    { -1,        0, 0}, //36: NC
    { -1,        0, 0}, //37: NC
    { -1,        0, 0}, //38: NC
    { -1,        0, 0}, //39: NC

    { 22, SPI_TYPE,  1}, //40: PA9(USCI0_DAT1)     (MISO)
    { 23, SPI_TYPE,  1}, //41: PA11(USCI0_CLK)     (CLK)
    { 24, SPI_TYPE,  1}, //42: PA10(USCI0_DAT0)    (MOSI)
    { 25, GPIO_TYPE,  1}, //43: PA8(USCI0_CTL1)     (SS)
    { 26, GPIO_TYPE, 1}, //44: PB15(USCI0_CTL1)    (GPIO) 
    //{ 27, UART_TYPE, 1}, //45: PB12(ICE_TX)        (NA)
    // 28, UART_TYPE, 1}, //46: PB13(ICE_RX)        (NA)
    { 29, GPIO_TYPE, 2}, //45: PB14(LED_R)         (NA)
};
#endif

GPIOPinDescription GPIO_Desc[] =
{
   /* UNO involved physical pins */
   /* List pins defined by UNO
   /* D0,  PB2    ,0 */   {PB, BIT2,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB2MFP_Msk,  SYS_GPB_MFPL_PB2MFP_GPIO  }}, 
   /* D1,  PB3    ,1 */   {PB, BIT3,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB3MFP_Msk,  SYS_GPB_MFPL_PB3MFP_GPIO  }}, 
   /* D2,  PC4    ,2 */   {PC, BIT4,   {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC4MFP_Msk,  SYS_GPC_MFPL_PC4MFP_GPIO  }},
   /* D3,  PC5    ,3 */   {PC, BIT5,   {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC5MFP_Msk,  SYS_GPC_MFPL_PC5MFP_GPIO  }},
   /* D4,  PC3    ,4 */   {PC, BIT3,   {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC3MFP_Msk,  SYS_GPC_MFPL_PC3MFP_GPIO  }},//10
   /* D5,  PC2    ,5 */   {PC, BIT2,   {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC2MFP_Msk,  SYS_GPC_MFPL_PC2MFP_GPIO  }},
   /* D6,  PA7    ,6 */   {PA, BIT7,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA7MFP_Msk,  SYS_GPA_MFPL_PA7MFP_GPIO  }},
   /* D7,  PA6    ,7 */   {PA, BIT6,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA6MFP_Msk,  SYS_GPA_MFPL_PA6MFP_GPIO  }},
   /* D8,  PA5    ,8 */   {PA, BIT5,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA5MFP_Msk,  SYS_GPA_MFPL_PA5MFP_GPIO  }},
   /* D9,  PA4    ,9 */   {PA, BIT4,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA4MFP_Msk,  SYS_GPA_MFPL_PA4MFP_GPIO  }},//15 
   /* D10, PA3    ,10 */  {PA, BIT3,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA3MFP_Msk,  SYS_GPA_MFPL_PA3MFP_GPIO  }}, 
   /* D11, PA0    ,11 */  {PA, BIT0,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA0MFP_Msk,  SYS_GPA_MFPL_PA0MFP_GPIO  }},
   /* D12, PA1    ,12 */  {PA, BIT1,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA1MFP_Msk,  SYS_GPA_MFPL_PA1MFP_GPIO  }},
   /* D13, PA2    ,13 */  {PA, BIT2,   {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA2MFP_Msk,  SYS_GPA_MFPL_PA2MFP_GPIO  }},
   /* SDA, PC0    ,14 */  {PC, BIT0,   {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC0MFP_Msk,  SYS_GPC_MFPL_PC0MFP_GPIO  }},//20
   /* SCL, PC1    ,15 */  {PC, BIT1,   {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC1MFP_Msk,  SYS_GPC_MFPL_PC1MFP_GPIO  }},//21
   /* A0,  PB7    ,16 */  {PB, BIT7,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB7MFP_Msk,  SYS_GPB_MFPL_PB7MFP_GPIO  }},
   /* A1,  PB6    ,17 */  {PB, BIT6,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB6MFP_Msk,  SYS_GPB_MFPL_PB6MFP_GPIO  }},//1 
   /* A2,  PB5    ,18 */  {PB, BIT5,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB5MFP_Msk,  SYS_GPB_MFPL_PB5MFP_GPIO  }},//2 
   /* A3,  PB4    ,19 */  {PB, BIT4,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB4MFP_Msk,  SYS_GPB_MFPL_PB4MFP_GPIO  }},//3 
   /* A4,  PB0    ,20 */  {PB, BIT0,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB0MFP_Msk,  SYS_GPB_MFPL_PB0MFP_GPIO  }},//4                                                                  
   /* A5,  PB1    ,21 */  {PB, BIT1,   {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB1MFP_Msk,  SYS_GPB_MFPL_PB1MFP_GPIO  }},//5 
   /* MISO,PA9    ,22 */  {PA, BIT9,   {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA9MFP_Msk,  SYS_GPA_MFPH_PA9MFP_GPIO  }},//22 
   /* CLK, PA11   ,23 */  {PA, BIT11,  {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA11MFP_Msk, SYS_GPA_MFPH_PA11MFP_GPIO }},//23 
   /* MOSI,PA10   ,24 */  {PA, BIT10,  {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA10MFP_Msk, SYS_GPA_MFPH_PA10MFP_GPIO }},//25 
   /* SS,  PA8    ,25 */  {PA, BIT8,   {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA8MFP_Msk,  SYS_GPA_MFPH_PA8MFP_GPIO  }},//24 
   /* GPIO,PB15   ,26 */  {PB, BIT15,  {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB15MFP_Msk, SYS_GPB_MFPH_PB15MFP_GPIO }},//26
   /* ICE_TX,PB12 ,27 */  {PB, BIT12,  {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB12MFP_Msk, SYS_GPB_MFPH_PB12MFP_UART0_RXD }},//27 
   /* ICE_RX,PB13 ,28 */  {PB, BIT13,  {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB13MFP_Msk, SYS_GPB_MFPH_PB13MFP_UART0_TXD }},//28
   /* R_LED,PB14  ,29 */  {PB, BIT14,  {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB14MFP_Msk, SYS_GPB_MFPH_PB14MFP_GPIO }},//29
};




PWMPinDescription PWM_Desc[PWM_MAX_COUNT] =
{

/* D2  */ {PWM0,  PWM0_MODULE,  PWM0_IRQn,  1, 500, {  2, SYS_GPC_MFPL_PC4MFP_PWM1_CH1 }}, 
/* D3  */ {PWM1,  PWM1_MODULE,  PWM1_IRQn,  0, 500, {  3, SYS_GPC_MFPL_PC5MFP_PWM1_CH0 }}, 
/* D4  */ {PWM1,  PWM1_MODULE,  PWM1_IRQn,  2, 500, {  4, SYS_GPC_MFPL_PC3MFP_PWM1_CH2 }}, 
/* D5  */ {PWM1,  PWM1_MODULE,  PWM1_IRQn,  3, 500, {  5, SYS_GPC_MFPL_PC2MFP_PWM1_CH3 }}, 
/* D6  */ {BPWM1, BPWM1_MODULE, BPWM1_IRQn, 2, 500, {  6, SYS_GPA_MFPL_PA7MFP_BPWM1_CH2 }},
/* D7  */ {BPWM1, BPWM1_MODULE, BPWM1_IRQn, 3, 500, {  7, SYS_GPA_MFPL_PA6MFP_BPWM1_CH3 }},
/* D8  */ {BPWM0, BPWM0_MODULE, BPWM0_IRQn, 5, 500, {  8, SYS_GPA_MFPL_PA5MFP_BPWM0_CH5 }},
/* D9  */ {BPWM0, BPWM0_MODULE, BPWM0_IRQn, 4, 500, {  9, SYS_GPA_MFPL_PA4MFP_BPWM0_CH4 }},

};

ADCPinDescription ADC_Desc[ADC_MAX_COUNT] =
{

  /* A0 */  {ADC, ADC_MODULE,  7, { 16,  SYS_GPB_MFPL_PB7MFP_ADC_CH7 }},  
  /* A1 */  {ADC, ADC_MODULE,  6, { 17,  SYS_GPB_MFPL_PB6MFP_ADC_CH6 }},   
  /* A2 */  {ADC, ADC_MODULE,  5, { 18,  SYS_GPB_MFPL_PB5MFP_ADC_CH5 }}, 
  /* A3 */  {ADC, ADC_MODULE,  4, { 19,  SYS_GPB_MFPL_PB4MFP_ADC_CH4 }}, 
  /* A4 */  {ADC, ADC_MODULE,  0, { 20,  SYS_GPB_MFPL_PB0MFP_ADC_CH0 }},    
  /* A5 */  {ADC, ADC_MODULE,  1, { 21,  SYS_GPB_MFPL_PB1MFP_ADC_CH1 }},    

};
UARTPinDescription UART_Desc[] =
{   
    {UART1, UART1_MODULE, UART13_IRQn, {{0, SYS_GPB_MFPL_PB2MFP_UART1_RXD}, {1, SYS_GPB_MFPL_PB3MFP_UART1_TXD} }},
    //ICE uart pin
    {UART0, UART0_MODULE, UART02_IRQn, {{27, SYS_GPB_MFPH_PB12MFP_UART0_RXD}, {28, SYS_GPB_MFPH_PB13MFP_UART0_TXD} }},
};


I2CPinDescription I2C_Desc[] =
{
    {I2C0, I2C0_MODULE, {{14, SYS_GPC_MFPL_PC0MFP_I2C0_SDA},  {15, SYS_GPC_MFPL_PC1MFP_I2C0_SCL}}},
    {I2C1, I2C1_MODULE, {{20,  SYS_GPB_MFPL_PB0MFP_I2C1_SDA}, {21,  SYS_GPB_MFPL_PB1MFP_I2C1_SCL}}},
}; 


SPIPinDescription SPI_Desc[] =
{
           

    SPI0, SPI0_MODULE, SPI0_IRQn, CLK_CLKSEL2_SPI0SEL_HIRC,
    {   
        { 11, SYS_GPA_MFPL_PA0MFP_SPI0_MOSI }, { 12, SYS_GPA_MFPL_PA1MFP_SPI0_MISO},
        
        { 13, SYS_GPA_MFPL_PA2MFP_SPI0_CLK},   { 10, SYS_GPA_MFPL_PA3MFP_GPIO}
    },

    USPI0, USCI0_MODULE, USCI01_IRQn, 0,
    {   
        { 23, SYS_GPA_MFPH_PA11MFP_USCI0_CLK }, { 24, SYS_GPA_MFPH_PA10MFP_USCI0_DAT0},
        
        { 22, SYS_GPA_MFPH_PA9MFP_USCI0_DAT1},  { 25, SYS_GPA_MFPH_PA8MFP_GPIO},
        //skip: {18, SYS_GPB_MFPL_PB5MFP_USCI1_CTL0}
        //arduino seems prefer using gpio doing SS 
    },                                   
};

#endif//#if defined(__M032KG__)
