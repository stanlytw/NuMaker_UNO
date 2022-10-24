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

#if defined(__M252__)

#ifdef USE_BoardToPin
BoardToPin BoardToPinInfo[] =
{
    {  4, UART_TYPE, 0},// 0: PB2(UART1_RX)     (D0/UART_RX0)
    {  3, UART_TYPE, 0},// 1: PB3(UART1_TX)     (D1/UART_TX0)
    { 70, PWM_TYPE, 0}, // 2: PC4(PWM1_CH1)     (D2/PWM0)
    { 69, PWM_TYPE, 1}, // 3: PC5(PWM1_CH0)     (D3/PWM1)
    { 71, PWM_TYPE, 2}, // 4: PC3(PWM1_CH2)     (D4/PWM2)
    { 72, PWM_TYPE, 3}, // 5: PC2(PWM1_CH3)     (D5/PWM3)
    { 50, PWM_TYPE, 4}, // 6: PA7(PWM1_CH4)     (D6/PWM4)
    { 51, PWM_TYPE, 5}, // 7: PA6(PWM1_CH5)     (D7/PWM5)

    { 55, PWM_TYPE, 6}, // 8: PA5(PWM0_CH0)     (D8/PWM6)
    { 56, PWM_TYPE, 7}, // 9: PA4(PWM0_CH1)     (D9/PWM7)
    { 57, SPI_TYPE, 0}, //10: PA3(SPI0_CS)      (D10/SPI0_CS)
    { 60, SPI_TYPE, 0}, //11: PA0(SPI0_MOSI)    (D11/SPI0_MOSI)
    { 59, SPI_TYPE, 0}, //12: PA1(SPI0_MISO)    (D12/SPI0_MISO)
    { 58, SPI_TYPE, 0}, //13: PA2(SPI0_SCK)     (D13/SPI0_SCK)
    { 74, I2C_TYPE, 0}, //14: PC0(I2C0_SDA)     (I2C0_SDA)
    { 73, I2C_TYPE, 0}, //15: PC1(I2C0_SCL)     (I2C0_SCL)
 
    {127, ADC_TYPE, 0}, //16: PB7(EADC0_CH7)    (A0)
    {128, ADC_TYPE, 1}, //17: PB6(EADC0_CH6)    (A1)
    {  1, ADC_TYPE, 2}, //18: PB5(EADC0_CH5)    (A2)
    {  2, ADC_TYPE, 3}, //19: PB4(EADC0_CH4)    (A3)
    { 10, ADC_TYPE, 4}, //20: PB0(EADC0_CH0)    (A4/I2C1_SDA)
    {  9, ADC_TYPE, 5}, //21: PB1(EADC0_CH1)    (A5/I2C1_SCL)

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

    { 15, SPI_TYPE, 1}, //40: PA9(USCI0_DAT1)    (MISO)
    { 13, SPI_TYPE, 1}, //41: PA11(USCI0_CLK)    (CLK)
    { 14, SPI_TYPE, 1}, //42: PA10(USCI0_DAT0)   (MOSI)
    { 16, SPI_TYPE, 1}, //43: PA8(USCI0_CTL1)    (SS)
    {116, GPIO_TYPE, 0},//44: PB15               (GPIO)
};
#endif
GPIOPinDescription GPIO_Desc[] =
{
    {NULL, 0, {0, 0, 0}}, //0
    {PB, BIT5, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB5MFP_Msk, SYS_GPB_MFPL_PB5MFP_GPIO}}, //1
    {PB, BIT4, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB4MFP_Msk, SYS_GPB_MFPL_PB4MFP_GPIO}}, //2
    {PB, BIT3, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB3MFP_Msk, SYS_GPB_MFPL_PB3MFP_GPIO}}, //3
    {PB, BIT2, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB2MFP_Msk, SYS_GPB_MFPL_PB2MFP_GPIO }}, //4
    {PC, BIT12, {(uint32_t) &SYS->GPC_MFPH, SYS_GPC_MFPH_PC12MFP_Msk, SYS_GPC_MFPH_PC12MFP_GPIO }}, //5
    {PC, BIT11, {(uint32_t) &SYS->GPC_MFPH, SYS_GPC_MFPH_PC11MFP_Msk, SYS_GPC_MFPH_PC11MFP_GPIO }}, //6
    {PC, BIT10, {(uint32_t) &SYS->GPC_MFPH, SYS_GPC_MFPH_PC10MFP_Msk, SYS_GPC_MFPH_PC10MFP_GPIO }}, //7
    {PC, BIT9, {(uint32_t) &SYS->GPC_MFPH, SYS_GPC_MFPH_PC9MFP_Msk, SYS_GPC_MFPH_PC9MFP_GPIO }}, //8
    {PB, BIT1, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB1MFP_Msk, SYS_GPB_MFPL_PB1MFP_GPIO}}, //9
    {PB, BIT0, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB0MFP_Msk, SYS_GPB_MFPL_PB0MFP_GPIO }}, //10
    {NULL, 0, {0, 0, 0}}, //11
    {NULL, 0, {0, 0, 0}}, //12
    {PA, BIT11, {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA11MFP_Msk, SYS_GPA_MFPH_PA11MFP_GPIO }}, //13
    {PA, BIT10, {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA10MFP_Msk, SYS_GPA_MFPH_PA10MFP_GPIO }}, //14
    {PA, BIT9, {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA9MFP_Msk, SYS_GPA_MFPH_PA9MFP_GPIO }}, //15
    {PA, BIT8, {(uint32_t) &SYS->GPA_MFPH, SYS_GPA_MFPH_PA8MFP_Msk, SYS_GPA_MFPH_PA8MFP_GPIO }}, //16
    {NULL, 0, {0, 0, 0}}, //17
    {PD, BIT12, {(uint32_t) &SYS->GPD_MFPH, SYS_GPD_MFPH_PD12MFP_Msk, SYS_GPD_MFPH_PD12MFP_GPIO }}, //18
    {PD, BIT11, {(uint32_t) &SYS->GPD_MFPH, SYS_GPD_MFPH_PD11MFP_Msk, SYS_GPD_MFPH_PD11MFP_GPIO }}, //19
    {PD, BIT10, {(uint32_t) &SYS->GPD_MFPH, SYS_GPD_MFPH_PD10MFP_Msk, SYS_GPD_MFPH_PD10MFP_GPIO }}, //20
    {NULL, 0, {0, 0, 0}}, //21
    {NULL, 0, {0, 0, 0}}, //22
    {NULL, 0, {0, 0, 0}}, //23
    {NULL, 0, {0, 0, 0}}, //24
    {NULL, 0, {0, 0, 0}}, //25
    {NULL, 0, {0, 0, 0}}, //26
    {NULL, 0, {0, 0, 0}}, //27
    {PF, BIT7, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF7MFP_Msk, SYS_GPF_MFPL_PF7MFP_GPIO }}, //28
    {PF, BIT6, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF6MFP_Msk, SYS_GPF_MFPL_PF6MFP_GPIO }}, //29
    {NULL, 0, {0, 0, 0}}, //30
    {PF, BIT5, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF5MFP_Msk, SYS_GPF_MFPL_PF5MFP_GPIO }}, //31
    {PF, BIT4, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF4MFP_Msk, SYS_GPF_MFPL_PF4MFP_GPIO }}, //32

    {NULL, 0, {0, 0, 0}}, //33
    {NULL, 0, {0, 0, 0}}, //34
    {NULL, 0, {0, 0, 0}}, //35
    {NULL, 0, {0, 0, 0}}, //36
    {PF, BIT3, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF3MFP_Msk, SYS_GPF_MFPL_PF3MFP_GPIO }}, //37
    {PF, BIT2, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF2MFP_Msk, SYS_GPF_MFPL_PF2MFP_GPIO }}, //38
    {NULL, 0, {0, 0, 0}}, //39
    {NULL, 0, {0, 0, 0}}, //40
    {PE, BIT8, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE8MFP_Msk, SYS_GPE_MFPH_PE8MFP_GPIO }}, //41
    {PE, BIT9, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE9MFP_Msk, SYS_GPE_MFPH_PE8MFP_GPIO }}, //42
    {PE, BIT10, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE10MFP_Msk, SYS_GPE_MFPH_PE10MFP_GPIO }}, //43
    {PE, BIT11, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE11MFP_Msk, SYS_GPE_MFPH_PE11MFP_GPIO }}, //44
    {PE, BIT12, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE12MFP_Msk, SYS_GPE_MFPH_PE12MFP_GPIO }}, //45
    {PE, BIT13, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE13MFP_Msk, SYS_GPE_MFPH_PE13MFP_GPIO }}, //46
    {PC, BIT8, {(uint32_t) &SYS->GPC_MFPH, SYS_GPC_MFPH_PC8MFP_Msk, SYS_GPC_MFPH_PC8MFP_GPIO }}, //47
    {PC, BIT7, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC7MFP_Msk, SYS_GPC_MFPL_PC7MFP_GPIO }}, //48
    {PC, BIT6, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC6MFP_Msk, SYS_GPC_MFPL_PC6MFP_GPIO }}, //49
    {PA, BIT7, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA7MFP_Msk, SYS_GPA_MFPL_PA7MFP_GPIO }}, //50
    {PA, BIT6, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA6MFP_Msk, SYS_GPA_MFPL_PA6MFP_GPIO }}, //51
    {NULL, 0, {0, 0, 0}}, //52
    {NULL, 0, {0, 0, 0}}, //53
    {PD, BIT15, {(uint32_t) &SYS->GPD_MFPH, SYS_GPD_MFPH_PD15MFP_Msk, SYS_GPD_MFPH_PD15MFP_GPIO }}, //54
    {PA, BIT5, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA5MFP_Msk, SYS_GPA_MFPL_PA5MFP_GPIO }}, //55
    {PA, BIT4, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA4MFP_Msk, SYS_GPA_MFPL_PA4MFP_GPIO }}, //56
    {PA, BIT3, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA3MFP_Msk, SYS_GPA_MFPL_PA3MFP_GPIO }}, //57
    {PA, BIT2, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA2MFP_Msk, SYS_GPA_MFPL_PA2MFP_GPIO }}, //58
    {PA, BIT1, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA1MFP_Msk, SYS_GPA_MFPL_PA1MFP_GPIO }}, //59
    {PA, BIT0, {(uint32_t) &SYS->GPA_MFPL, SYS_GPA_MFPL_PA0MFP_Msk, SYS_GPA_MFPL_PA0MFP_GPIO }}, //60
    {NULL, 0, {0, 0, 0}}, //61
    {PE, BIT14, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE14MFP_Msk, SYS_GPE_MFPH_PE14MFP_GPIO }}, //62
    {PE, BIT15, {(uint32_t) &SYS->GPE_MFPH, SYS_GPE_MFPH_PE15MFP_Msk, SYS_GPE_MFPH_PE15MFP_GPIO }}, //63
    {NULL, 0, {0, 0, 0}}, //64

    {PF, BIT0, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF0MFP_Msk, SYS_GPF_MFPL_PF0MFP_GPIO }}, //65
    {PF, BIT1, {(uint32_t) &SYS->GPF_MFPL, SYS_GPF_MFPL_PF1MFP_Msk, SYS_GPF_MFPL_PF1MFP_GPIO }}, //66
    {PD, BIT9, {(uint32_t) &SYS->GPD_MFPH, SYS_GPD_MFPH_PD9MFP_Msk, SYS_GPD_MFPH_PD9MFP_GPIO }}, //67
    {PD, BIT8, {(uint32_t) &SYS->GPD_MFPH, SYS_GPD_MFPH_PD8MFP_Msk, SYS_GPD_MFPH_PD8MFP_GPIO }}, //68
    {PC, BIT5, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC5MFP_Msk, SYS_GPC_MFPL_PC5MFP_GPIO }}, //69
    {PC, BIT4, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC4MFP_Msk, SYS_GPC_MFPL_PC4MFP_GPIO }}, //70
    {PC, BIT3, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC3MFP_Msk, SYS_GPC_MFPL_PC3MFP_GPIO }}, //71
    {PC, BIT2, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC2MFP_Msk, SYS_GPC_MFPL_PC2MFP_GPIO }}, //72
    {PC, BIT1, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC1MFP_Msk, SYS_GPC_MFPL_PC1MFP_GPIO }}, //73
    {PC, BIT0, {(uint32_t) &SYS->GPC_MFPL, SYS_GPC_MFPL_PC0MFP_Msk, SYS_GPC_MFPL_PC0MFP_GPIO }}, //74
    {NULL, 0, {0, 0, 0}}, //75
    {NULL, 0, {0, 0, 0}}, //76
    {NULL, 0, {0, 0, 0}}, //77
    {NULL, 0, {0, 0, 0}}, //78
    {NULL, 0, {0, 0, 0}}, //79
    {NULL, 0, {0, 0, 0}}, //80
    {NULL, 0, {0, 0, 0}}, //81
    {NULL, 0, {0, 0, 0}}, //82
    {NULL, 0, {0, 0, 0}}, //83
    {PD, BIT7, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD7MFP_Msk, SYS_GPD_MFPL_PD7MFP_GPIO }}, //84
    {PD, BIT6, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD6MFP_Msk, SYS_GPD_MFPL_PD6MFP_GPIO }}, //85
    {PD, BIT5, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD5MFP_Msk, SYS_GPD_MFPL_PD5MFP_GPIO }}, //86
    {PD, BIT4, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD4MFP_Msk, SYS_GPD_MFPL_PD4MFP_GPIO }}, //87
    {PD, BIT3, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD3MFP_Msk, SYS_GPD_MFPL_PD3MFP_GPIO }}, //88
    {PD, BIT2, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD2MFP_Msk, SYS_GPD_MFPL_PD2MFP_GPIO }}, //89
    {PD, BIT1, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD1MFP_Msk, SYS_GPD_MFPL_PD1MFP_GPIO }}, //90
    {PD, BIT0, {(uint32_t) &SYS->GPD_MFPL, SYS_GPD_MFPL_PD0MFP_Msk, SYS_GPD_MFPL_PD0MFP_GPIO }}, //91
    {PD, BIT13, {(uint32_t) &SYS->GPD_MFPH, SYS_GPD_MFPH_PD13MFP_Msk, SYS_GPD_MFPH_PD13MFP_GPIO }}, //92
    {NULL, 0, {0, 0, 0}}, //93
    {NULL, 0, {0, 0, 0}}, //94
    {NULL, 0, {0, 0, 0}}, //95
    {NULL, 0, {0, 0, 0}}, //96
 
    {PE, BIT7, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE7MFP_Msk, SYS_GPE_MFPL_PE7MFP_GPIO }}, //97
    {PE, BIT6, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE6MFP_Msk, SYS_GPE_MFPL_PE6MFP_GPIO }}, //98
    {PE, BIT5, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE5MFP_Msk, SYS_GPE_MFPL_PE5MFP_GPIO }}, //99
    {PE, BIT4, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE4MFP_Msk, SYS_GPE_MFPL_PE4MFP_GPIO }}, //100
    {PE, BIT3, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE3MFP_Msk, SYS_GPE_MFPL_PE3MFP_GPIO }}, //101
    {PE, BIT2, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE2MFP_Msk, SYS_GPE_MFPL_PE2MFP_GPIO }}, //102
    {NULL, 0, {0, 0, 0}}, //103
    {NULL, 0, {0, 0, 0}}, //104
    {PE, BIT1, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE1MFP_Msk, SYS_GPE_MFPL_PE1MFP_GPIO }}, //105
    {PE, BIT0, {(uint32_t) &SYS->GPE_MFPL, SYS_GPE_MFPL_PE0MFP_Msk, SYS_GPE_MFPL_PE0MFP_GPIO }}, //106
    {NULL, 0, {0, 0, 0}}, //107
    {NULL, 0, {0, 0, 0}}, //108
    {NULL, 0, {0, 0, 0}}, //109
    {NULL, 0, {0, 0, 0}}, //110
    {NULL, 0, {0, 0, 0}}, //111
    {NULL, 0, {0, 0, 0}}, //112
    {NULL, 0, {0, 0, 0}}, //113
    {NULL, 0, {0, 0, 0}}, //114
    {PC, BIT14, {(uint32_t) &SYS->GPC_MFPH, SYS_GPC_MFPH_PC14MFP_Msk, SYS_GPC_MFPH_PC14MFP_GPIO }}, //115
    {PB, BIT15, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB15MFP_Msk, SYS_GPB_MFPH_PB15MFP_GPIO }}, //116
    {PB, BIT14, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB14MFP_Msk, SYS_GPB_MFPH_PB14MFP_GPIO }}, //117
    {PB, BIT13, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB13MFP_Msk, SYS_GPB_MFPH_PB13MFP_GPIO }}, //118
    {PB, BIT12, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB12MFP_Msk, SYS_GPB_MFPH_PB12MFP_GPIO }}, //119
    {NULL, 0, {0, 0, 0}}, //120
    {NULL, 0, {0, 0, 0}}, //121
    {NULL, 0, {0, 0, 0}}, //122
    {PB, BIT11, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB11MFP_Msk, SYS_GPB_MFPH_PB11MFP_GPIO }}, //123
    {PB, BIT10, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB10MFP_Msk, SYS_GPB_MFPH_PB10MFP_GPIO }}, //124
    {PB, BIT9, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB9MFP_Msk, SYS_GPB_MFPH_PB9MFP_GPIO }}, //125
    {PB, BIT8, {(uint32_t) &SYS->GPB_MFPH, SYS_GPB_MFPH_PB8MFP_Msk, SYS_GPB_MFPH_PB8MFP_GPIO }}, //126
    {PB, BIT7, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB7MFP_Msk, SYS_GPB_MFPL_PB7MFP_GPIO}}, //127
    {PB, BIT6, {(uint32_t) &SYS->GPB_MFPL, SYS_GPB_MFPL_PB6MFP_Msk, SYS_GPB_MFPL_PB6MFP_GPIO}}, //128
};

PWMPinDescription PWM_Desc[] = 
{
    {PWM1, (uint32_t)PWM1_MODULE, PWM1_P0_IRQn, 1, 500, {70, SYS_GPC_MFPL_PC4MFP_PWM1_CH1  }}, //0  PC4
    {PWM1, (uint32_t)PWM1_MODULE, PWM1_P0_IRQn, 0, 500, {69, SYS_GPC_MFPL_PC5MFP_PWM1_CH0  }}, //1  PC5
    {PWM1, (uint32_t)PWM1_MODULE, PWM1_P1_IRQn, 2, 500, {71, SYS_GPC_MFPL_PC3MFP_PWM1_CH2  }}, //2  PC3
    {PWM1, (uint32_t)PWM1_MODULE, PWM1_P1_IRQn, 3, 500, {72, SYS_GPC_MFPL_PC2MFP_PWM1_CH3  }}, //3  PC2
    {PWM1, (uint32_t)PWM1_MODULE, PWM1_P2_IRQn, 4, 500, {50, SYS_GPA_MFPL_PA7MFP_PWM1_CH4  }}, //4  PA7
    {PWM1, (uint32_t)PWM1_MODULE, PWM1_P2_IRQn, 5, 500, {51, SYS_GPA_MFPL_PA6MFP_PWM1_CH5  }}, //5  PA6
    {PWM0, (uint32_t)PWM0_MODULE, PWM0_P0_IRQn, 0, 500, {55, SYS_GPA_MFPL_PA5MFP_PWM0_CH0  }}, //6  PA5
    {PWM0, (uint32_t)PWM0_MODULE, PWM0_P0_IRQn, 1, 500, {56, SYS_GPA_MFPL_PA4MFP_PWM0_CH1  }} //7  PA4
};

ADCPinDescription ADC_Desc[] =
{
    {EADC, EADC_MODULE, 7, { 127, SYS_GPB_MFPL_PB7MFP_EADC0_CH7 }}, //0 PB7
    {EADC, EADC_MODULE, 6, { 128, SYS_GPB_MFPL_PB6MFP_EADC0_CH6 }}, //1 PB6
    {EADC, EADC_MODULE, 5, {   1, SYS_GPB_MFPL_PB5MFP_EADC0_CH5 }}, //2 PB5
    {EADC, EADC_MODULE, 4, {   2, SYS_GPB_MFPL_PB4MFP_EADC0_CH4 }}, //3 PB4
    {EADC, EADC_MODULE, 0, {  10, SYS_GPB_MFPL_PB0MFP_EADC0_CH0 }}, //4 PB0
    {EADC, EADC_MODULE, 1, {   9, SYS_GPB_MFPL_PB1MFP_EADC0_CH1 }}, //5 PB1
};

SPIPinDescription SPI_Desc[] =
{
    {
        SPI0, (uint32_t)SPI0_MODULE, SPI0_IRQn, CLK_CLKSEL2_SPI0SEL_PCLK1,
        {   {58, SYS_GPA_MFPL_PA2MFP_SPI0_CLK }, {60, SYS_GPA_MFPL_PA0MFP_SPI0_MOSI},
            {59, SYS_GPA_MFPL_PA1MFP_SPI0_MISO}, {57, SYS_GPA_MFPL_PA3MFP_SPI0_SS  }
        }
    },
    {
        USPI0, (uint32_t)USCI0_MODULE, USCI0_IRQn, (0x1ul << CLK_APBCLK1_USCI0CKEN_Pos),
        {   {13, SYS_GPA_MFPH_PA11MFP_USCI0_CLK }, {14, SYS_GPA_MFPH_PA10MFP_USCI0_DAT0},
            {15, SYS_GPA_MFPH_PA9MFP_USCI0_DAT1}, {16, SYS_GPA_MFPH_PA8MFP_USCI0_CTL1  }
        }
    },

};

UARTPinDescription UART_Desc[] =
{
    {UART1, (uint32_t)UART1_MODULE, UART1_IRQn, {{  4, SYS_GPB_MFPL_PB2MFP_UART1_RXD }, {3, SYS_GPB_MFPL_PB3MFP_UART1_TXD }}}, //UNO uart pin

    {UART0, (uint32_t)UART0_MODULE, UART0_IRQn, {{119, SYS_GPB_MFPH_PB12MFP_UART0_RXD }, {118, SYS_GPB_MFPH_PB13MFP_UART0_TXD }}}, //ICE uart pin
};

I2CPinDescription I2C_Desc[] =
{
    {I2C0, I2C0_MODULE, {{73, SYS_GPC_MFPL_PC1MFP_I2C0_SCL}, {74, SYS_GPC_MFPL_PC0MFP_I2C0_SDA }}},
    //{I2C1, I2C1_MODULE, {{ 9, SYS_GPB_MFPL_PB1MFP_I2C1_SCL}, {10, SYS_GPB_MFPL_PB0MFP_I2C1_SDA }}},
};

#endif
