
#include "Arduino.h"
extern void CLK_EnableModuleClock(uint32_t);
#ifdef __cplusplus
extern "C" {
#endif

/** Tick Counter united by ms */
static volatile uint32_t _dwTickCount = 0 ;

/**
 * \brief SysTick_Handler.
 */
void SysTick_Handler(void)
{
    /* Increment counter necessary in delay(). */
    _dwTickCount++;
}

/**
 *  \brief Get current Tick Count, in ms.
 */
uint32_t GetTickCount(void)
{
    return _dwTickCount ;
}

#ifdef __cplusplus
}
#endif
//---------------------------------------------------------------------------

void Enable_All_IPs(void)
{
#if defined(__M460__) 
    //Enable ADC module
    CLK_EnableModuleClock(EADC0_MODULE);
    /* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
    //CLK_SetModuleClock(EADC0_MODULE, 0, CLK_CLKDIV0_EADC0(8));
    CLK_SetModuleClock(EADC0_MODULE, CLK_CLKSEL0_EADC0SEL_HCLK, CLK_CLKDIV0_EADC0(8));

    //Enable PWM0 ~PWM1 module
    CLK_EnableModuleClock(EPWM0_MODULE);
    CLK_EnableModuleClock(EPWM1_MODULE);

#if defined(__M467SJHAN__)
    /*M467SJ: PA, PB, PC, PF*/
    CLK_EnableModuleClock(GPA_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);
    CLK_EnableModuleClock(GPC_MODULE);
    CLK_EnableModuleClock(GPF_MODULE);
#else
    /*M467HJ*/
    CLK_EnableModuleClock(GPA_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);
    CLK_EnableModuleClock(GPC_MODULE);
    CLK_EnableModuleClock(GPD_MODULE);//[2024-11-06]For SDH
    CLK_EnableModuleClock(GPE_MODULE);//[2024-11-06]For SDH
    CLK_EnableModuleClock(GPG_MODULE);
	CLK_EnableModuleClock(GPJ_MODULE);//[2025-03-17]For CANFD on Numaker-IOT-M467, PJ10, PJ11
#endif

    CLK_SetModuleClock(EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0);
    CLK_SetModuleClock(EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0);

#if defined(__M460MINIMA__)
    /* Enable RTC module clock */
    CLK_EnableModuleClock(RTC_MODULE);
#endif
    /* Enable SDH0 module clock source as HCLK and SDH0 module clock divider as 4 */
    //[2024-11-06]Since no wrap for sdh, enable ip clock here.
	//[2025-03-25]Change to SD1
#if defined(__M467SJHAN__)    
    CLK_EnableModuleClock(SDH1_MODULE);
    CLK_SetModuleClock(SDH1_MODULE, CLK_CLKSEL0_SDH1SEL_HCLK, CLK_CLKDIV3_SDH1(4));
#else
    CLK_EnableModuleClock(SDH0_MODULE);
    CLK_SetModuleClock(SDH0_MODULE, CLK_CLKSEL0_SDH0SEL_HCLK, CLK_CLKDIV0_SDH0(4));
#endif	
	
	/* Enable CANFD module clock source and proper divide by 1, ref Std sample code */
	/* CANFD Pin Configuarion left in NVT CAN library*/
    //[2024-11-06]Since to compatible to mcp_can class, enable ip clock here.
	//Default use CAN FD0
	//To do: If multi-CANFD mocule are required, add them here.
	 /* Select CAN FD0 clock source is HCLK */
    CLK_SetModuleClock(CANFD0_MODULE, CLK_CLKSEL0_CANFD0SEL_HCLK, CLK_CLKDIV5_CANFD0(1));
    /* Enable CAN FD0 peripheral clock */
    CLK_EnableModuleClock(CANFD0_MODULE);
	/* CANFD_Config is done in nvtCAN library .begin method*/
    

    /* SD0 Pin define for M467SJ */
#if defined(__M467SJHAN__)
    /*M467SJ:*/
	//SET_SD1_nCD_PA6();
    SET_SD1_CLK_PA4();
    SET_SD1_CMD_PA5();
    SET_SD1_DAT0_PA8();
    SET_SD1_DAT1_PA9();
    SET_SD1_DAT2_PA10();
    SET_SD1_DAT3_PA11();

#if 0	
	/* Enable UART0 module clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Select UART0 module clock source as HIRC and UART0 module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
	
	/* Set multi-function pins for UART0 RXD and TXD */
    SET_UART0_RXD_PA6();
    SET_UART0_TXD_PA7();
		
	UART_Open(UART0, 115200);
#endif
#else
    /*M467HJ*/
    SET_SD0_nCD_PD13();
    SET_SD0_CLK_PE6();
    SET_SD0_CMD_PE7();
    SET_SD0_DAT0_PE2();
    SET_SD0_DAT1_PE3();
    SET_SD0_DAT2_PE4();
    SET_SD0_DAT3_PE5();
#endif//(__M467SJHAN__)

#endif

}
//F_CPU
void init(void)
{
    init_flag = 1;
    /* Unlock protected registers */
    SYS_UnlockReg();
#if defined(__M460__)
#if defined(__M460MINIMA_PB12LDR__)
    /* Enable HIRC and HXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk | CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HIRC and HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk | CLK_STATUS_HXTSTB_Msk);
#elif defined(__M460MINIMA__)
    /* Enable HIRC and HXT clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk | CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HIRC and HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk | CLK_STATUS_HXTSTB_Msk);

    /* Enable LXT and Waiting for clock ready */
    CLK_EnableXtalRC(CLK_PWRCTL_LXTEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);
   
#else

   /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
#endif
    /* Set PCLK0 and PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);
    
    /* Set core clock to 192MHz */
    CLK_SetCoreClock(F_CPU);


#endif
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set Systick to 1ms interval */
    if (SysTick_Config(F_CPU / 1000))
    {
        // Capture error
        while (true);
    }

    /* To update the variable SystemCoreClock */
    SystemCoreClockUpdate();
    /* Enable All of IP */
    Enable_All_IPs();
#if defined(__M460MINIMA__)    
    /* Do not Lock protected register for code jumping*/
    // Not to Lock
#else
    /* Lock protected registers */
    SYS_LockReg();
#endif    
}
