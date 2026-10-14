/* ----------------------------------------------------------------------------
 *         Nuvoton Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012, Nuvoton Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following condition is met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */
#include "m460.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

// Arduino: we must setup hardware before doing this
void __libc_init_array(void);

/* Default empty handler */
void Dummy_Handler(void);


// Arduino: handlers weak symbols moved into main
/* Cortex-M4 core handlers */
void Reset_Handler      ( void );
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void BOD_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void IRC_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void PWRWU_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void RAMPE_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CLKFAIL_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void ISP_IRQHandler			 ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void RTC_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void TAMPER_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void WDT_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void WWDT_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT4_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT5_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void GPA_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GPB_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GPC_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GPD_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void GPE_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void GPF_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI0_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void QSPI0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void BRAKE0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EPWM0_P0_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EPWM0_P1_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EPWM0_P2_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BRAKE1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EPWM1_P0_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EPWM1_P1_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void EPWM1_P2_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TMR0_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TMR1_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TMR2_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void TMR3_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UART0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void I2C0_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void I2C1_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PDMA0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC00_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC01_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void ACMP01_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void ACMP23_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC02_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC03_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void QSPI1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI1_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI2_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void USBD_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void OHCI_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void USBOTG_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void BMC_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI5_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SC0_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SC1_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SC2_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void GPJ_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI3_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI4_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SDH0_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void USBD20_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EMAC0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void I2S0_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void I2S1_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI6_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CRPT_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void GPG_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT6_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART4_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART5_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void USCI0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI7_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void BPWM0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void BPWM1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPIM_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CCAP_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void I2C2_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void I2C3_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EQEI0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EQEI1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void ECAP0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void ECAP1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void GPH_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EINT7_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SDH1_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void PSIO_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EHCI_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void USBOTG20_IRQHandler     ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void ECAP2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void ECAP3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void KPI_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void HBI_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void PDMA1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART8_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART9_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void TRNG_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART6_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void UART7_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC10_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC11_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC12_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC13_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI8_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void KS_IRQHandler           ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void GPI_IRQHandler          ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI9_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CANFD00_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CANFD01_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CANFD10_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EQEI2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EQEI3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void I2C4_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void SPI10_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CANFD20_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CANFD21_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void CANFD30_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC20_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC21_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC22_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 
void EADC23_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler"))); 

 
/* Exception Table */
__attribute__ ((section(".vectors")))
const void * exception_table[] = {

	/* Configure Initial Stack Pointer, using linker-generated symbols */
	(void*) (&_estack),
	(void*) Reset_Handler,
	(void*) NMI_Handler,
	(void*) HardFault_Handler,
	(void*) MemManage_Handler,
	(void*) BusFault_Handler,
	(void*) UsageFault_Handler,
	(void*) (0UL),                    /* Reserved */
	(void*) (0UL),                    /* Reserved */
	(void*) (0UL),                    /* Reserved */
	(void*) (0UL),                    /* Reserved */
	(void*) SVC_Handler,
	(void*) DebugMon_Handler,
	(void*) (0UL),                    /* Reserved */
	(void*) PendSV_Handler,
	(void*) SysTick_Handler,

	/* Configurable interrupts */
    (void*)	BOD_IRQHandler,        //  0: BOD                        //
	(void*)	IRC_IRQHandler,        //  1: IRC                        //
	(void*)	PWRWU_IRQHandler,      //  2: PWRWU                      //
	(void*)	RAMPE_IRQHandler,      //  3: RAMPE                      //
	(void*)	CLKFAIL_IRQHandler,     //  4: CKFAIL                     //
	(void*)	ISP_IRQHandler,        //  5: ISP                        //
	(void*)	RTC_IRQHandler,        //  6: RTC                        //
	(void*)	TAMPER_IRQHandler,     //  7: TAMPER                     //
	(void*)	WDT_IRQHandler,        //  8: WDT                        //
	(void*)	WWDT_IRQHandler,       //  9: WWDT                       //
	(void*)	EINT0_IRQHandler,      // 10: EINT0                      //
	(void*)	EINT1_IRQHandler,      // 11: EINT1                      //
	(void*)	EINT2_IRQHandler,      // 12: EINT2                      //
	(void*)	EINT3_IRQHandler,      // 13: EINT3                      //
	(void*)	EINT4_IRQHandler,      // 14: EINT4                      //
	(void*)	EINT5_IRQHandler,      // 15: EINT5                      //
	(void*)	GPA_IRQHandler,        // 16: GPA                        //
	(void*)	GPB_IRQHandler,        // 17: GPB                        //
	(void*)	GPC_IRQHandler,        // 18: GPC                        //
	(void*)	GPD_IRQHandler,        // 19: GPD                        //
	(void*)	GPE_IRQHandler,        // 20: GPE                        //
	(void*)	GPF_IRQHandler,        // 21: GPF                        //
	(void*)	QSPI0_IRQHandler,      // 22: QSPI0                      //
	(void*)	SPI0_IRQHandler,       // 23: SPI0                       //
	(void*)	BRAKE0_IRQHandler,     // 24: BRAKE0                     //
	(void*)	EPWM0_P0_IRQHandler,    // 25: EPWM0P0                    //
	(void*)	EPWM0_P1_IRQHandler,    // 26: EPWM0P1                    //
	(void*)	EPWM0_P2_IRQHandler,    // 27: EPWM0P2                    //
	(void*)	BRAKE1_IRQHandler,     // 28: BRAKE1                     //
	(void*)	EPWM1_P0_IRQHandler,    // 29: EPWM1P0                    //
	(void*)	EPWM1_P1_IRQHandler,    // 30: EPWM1P1                    //
	(void*)	EPWM1_P2_IRQHandler,    // 31: EPWM1P2                    //
	(void*)	TMR0_IRQHandler,       // 32: TIMER0                     //
	(void*)	TMR1_IRQHandler,       // 33: TIMER1                     //
	(void*)	TMR2_IRQHandler,       // 34: TIMER2                     //
	(void*)	TMR3_IRQHandler,       // 35: TIMER3                     //
	(void*)	UART0_IRQHandler,      // 36: UART0                      //
	(void*)	UART1_IRQHandler,      // 37: UART1                      //
	(void*)	I2C0_IRQHandler,       // 38: I2C0                       //
	(void*)	I2C1_IRQHandler,       // 39: I2C1                       //
	(void*)	PDMA0_IRQHandler,      // 40: PDMA0                      //
	(void*)	DAC_IRQHandler,        // 41: DAC                        //
	(void*)	EADC00_IRQHandler,     // 42: EADC00                     //
	(void*)	EADC01_IRQHandler,     // 43: EADC01                     //
	(void*)	ACMP01_IRQHandler,     // 44: ACMP01                     //
	(void*)	ACMP23_IRQHandler,     // 45: ACMP23                     //
	(void*)	EADC02_IRQHandler,     // 46: EADC02                     //
	(void*)	EADC03_IRQHandler,     // 47: EADC03                     //
	(void*)	UART2_IRQHandler,      // 48: UART2                      //
	(void*)	UART3_IRQHandler,      // 49: UART3                      //
	(void*)	QSPI1_IRQHandler,      // 50: QSPI1                      //
	(void*)	SPI1_IRQHandler,       // 51: SPI1                       //
	(void*)	SPI2_IRQHandler,       // 52: SPI2                       //
	(void*)	USBD_IRQHandler,       // 53: USBD                       //
	(void*)	OHCI_IRQHandler,       // 54: OHCI                       //
	(void*)	USBOTG_IRQHandler,     // 55: OTG                        //
	(void*)	BMC_IRQHandler,        // 56: BMC                        //
	(void*)	SPI5_IRQHandler,       // 57: SPI5                       //
	(void*)	SC0_IRQHandler,        // 58: SC0                        //
	(void*)	SC1_IRQHandler,        // 59: SC1                        //
	(void*)	SC2_IRQHandler,        // 60: SC2                        //
	(void*)	GPJ_IRQHandler,        // 61: GPJ                        //
	(void*)	SPI3_IRQHandler,       // 62: SPI3                       //
	(void*)	SPI4_IRQHandler,       // 63: SPI4                       //
	(void*)	SDH0_IRQHandler,       // 64: SDH0                       //
	(void*)	USBD20_IRQHandler,     // 65: HSUSBD                     //
	(void*)	EMAC0_IRQHandler,      // 66: EMAC0                      //
	(void*)	0,                     // 67: Reserved                   //
	(void*)	I2S0_IRQHandler,       // 68: I2S0                       //
	(void*)	I2S1_IRQHandler,       // 69: I2S1                       //
	(void*)	SPI6_IRQHandler,       // 70: SPI6                       //
	(void*)	CRPT_IRQHandler,       // 71: CRPT                       //
	(void*)	GPG_IRQHandler,        // 72: GPG                        //
	(void*)	EINT6_IRQHandler,      // 73: EINT6                      //
	(void*)	UART4_IRQHandler,      // 74: UART4                      //
	(void*)	UART5_IRQHandler,      // 75: UART5                      //
	(void*)	USCI0_IRQHandler,      // 76: USCI0                      //
	(void*)	SPI7_IRQHandler,       // 77: SPI7                       //
	(void*)	BPWM0_IRQHandler,      // 78: BPWM0                      //
	(void*)	BPWM1_IRQHandler,      // 79: BPWM1                      //
	(void*)	SPIM_IRQHandler,       // 80: SPIM                       //
	(void*)	CCAP_IRQHandler,       // 81: CCAP                       //
	(void*)	I2C2_IRQHandler,       // 82: I2C2                       //
	(void*)	I2C3_IRQHandler,       // 83: I2C3                       //
	(void*)	EQEI0_IRQHandler,      // 84: EQEI0                      //
	(void*)	EQEI0_IRQHandler,      // 85: EQEI1                      //
	(void*)	ECAP0_IRQHandler,      // 86: ECAP0                      //
	(void*)	ECAP1_IRQHandler,      // 87: ECAP1                      //
	(void*)	GPH_IRQHandler,        // 88: GPH                        //
	(void*)	EINT7_IRQHandler,      // 89: EINT7                      //
	(void*)	SDH1_IRQHandler,       // 90: SDH1                       //
	(void*)	PSIO_IRQHandler,       // 91: PSIO                       //
	(void*)	EHCI_IRQHandler,       // 92: EHCI                       //
	(void*)	USBOTG20_IRQHandler,   // 93: HSOTG                      //
	(void*)	ECAP2_IRQHandler,      // 94: ECAP2                      //
	(void*)	ECAP3_IRQHandler,      // 95: ECAP3                      //
	(void*)	KPI_IRQHandler,        // 96: KPI                        //
	(void*)	HBI_IRQHandler,        // 97: HBI                        //
	(void*)	PDMA1_IRQHandler,      // 98: PDMA1                      //
	(void*)	UART8_IRQHandler,      // 99: UART8                      //
	(void*)	UART9_IRQHandler,      // 100: UART9                     //
	(void*)	TRNG_IRQHandler,      // 101: TRNG                      //
	(void*)	UART6_IRQHandler,      // 102: UART6                     //
	(void*)	UART7_IRQHandler,      // 103: UART7                     //
	(void*)	EADC10_IRQHandler,     // 104: EADC10                    //
	(void*)	EADC11_IRQHandler,     // 105: EADC11                    //
	(void*)	EADC12_IRQHandler,     // 106: EADC12                    //
	(void*)	EADC13_IRQHandler,     // 107: EADC13                    //
	(void*)	SPI8_IRQHandler,       // 108: SPI8                      //
	(void*)	KS_IRQHandler,         // 109: KS                        //
	(void*)	GPI_IRQHandler,        // 110: GPI                       //
	(void*)	SPI9_IRQHandler,       // 111: SPI9                      //
	(void*)	CANFD00_IRQHandler,    // 112: CANFD00                   //
	(void*)	CANFD01_IRQHandler,    // 113: CANFD01                   //
	(void*)	CANFD10_IRQHandler,    // 114: CANFD10                   //
	(void*)	CANFD10_IRQHandler,    // 115: CANFD10                   //
	(void*)	EQEI2_IRQHandler,      // 116: EQEI2                     //
	(void*)	EQEI3_IRQHandler,      // 117: EQEI3                     //
	(void*)	I2C4_IRQHandler,       // 118: I2C4                      //
	(void*)	SPI10_IRQHandler,      // 119: SPI10                     //
	(void*)	CANFD20_IRQHandler,    // 120: CANFD20                   //
	(void*)	CANFD21_IRQHandler,    // 121: CANFD21                   //
	(void*)	CANFD30_IRQHandler,    // 122: CANFD30                   //
	(void*)	CANFD30_IRQHandler,    // 123: CANFD30                   //
	(void*)	EADC20_IRQHandler,     // 124: EADC20                    //
	(void*)	EADC21_IRQHandler,     // 125: EADC21                    //
	(void*)	EADC22_IRQHandler,     // 126: EADC22                    //
	(void*)	EADC23_IRQHandler     // 127: EADC23                    //
};
/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
	uint32_t *pSrc, *pDest;

	/* Initialize the relocate segment */
	pSrc = &_etext;
	pDest = &_srelocate;

	if (pSrc != pDest) {
		for (; pDest < &_erelocate;) {
			*pDest++ = *pSrc++;
		}
	}

	/* Clear the zero segment */
	for (pDest = &_szero; pDest < &_ezero;) {
		*pDest++ = 0;
	}

	/* Initialize the C library */

	// Arduino: we must setup hardware before doing this
	__libc_init_array();

	SYS_UnlockReg();
	SystemInit();
	SYS_LockReg();
	/* Branch to main function */
	main();

	/* Infinite loop */
	while (1);
}

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Dummy_Handler(void)
{
	while (1) {
	}
}
