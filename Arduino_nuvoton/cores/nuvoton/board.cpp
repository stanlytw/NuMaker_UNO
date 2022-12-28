
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
#if defined(__M451__)
    //Enable ADC module
    CLK_EnableModuleClock(EADC_MODULE);
    /* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
    CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));

    //Enable PWM0 ~PWM1 module
    CLK_EnableModuleClock(PWM0_MODULE);
    CLK_EnableModuleClock(PWM1_MODULE);

    CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL2_PWM0SEL_PCLK0, 0);
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK1, 0);
#elif defined(__NUC240__)
    //Enable ADC module
    CLK_EnableModuleClock(ADC_MODULE);

    //Enable PWM0 ~PWM7 module
    CLK_EnableModuleClock(PWM01_MODULE);
    CLK_EnableModuleClock(PWM23_MODULE);
    CLK_EnableModuleClock(PWM45_MODULE);
    CLK_EnableModuleClock(PWM67_MODULE);

    CLK_SetModuleClock(PWM01_MODULE, CLK_CLKSEL2_PWM45_S_HXT, 0);
    CLK_SetModuleClock(PWM23_MODULE, CLK_CLKSEL2_PWM45_S_HXT, 0);
    CLK_SetModuleClock(PWM45_MODULE, CLK_CLKSEL2_PWM45_S_HXT, 0);
    CLK_SetModuleClock(PWM67_MODULE, CLK_CLKSEL2_PWM45_S_HXT, 0);
#elif defined(__NANO100__)
    CLK_EnableModuleClock(ADC_MODULE);

    //Enable PWM0 ~PWM7 module
    CLK_EnableModuleClock(PWM0_CH01_MODULE);
    CLK_EnableModuleClock(PWM0_CH23_MODULE);
    CLK_EnableModuleClock(PWM1_CH01_MODULE);
    CLK_EnableModuleClock(PWM1_CH23_MODULE);

    CLK_SetModuleClock(PWM0_CH01_MODULE, CLK_CLKSEL1_PWM0_CH01_S_HXT, 0);
    CLK_SetModuleClock(PWM0_CH23_MODULE, CLK_CLKSEL1_PWM0_CH23_S_HXT, 0);
    CLK_SetModuleClock(PWM1_CH01_MODULE, CLK_CLKSEL2_PWM1_CH01_S_HXT, 0);
    CLK_SetModuleClock(PWM1_CH23_MODULE, CLK_CLKSEL2_PWM1_CH23_S_HXT, 0);
#elif defined(__NUC131__)
    //Enable ADC module
    CLK_EnableModuleClock(ADC_MODULE);

    //Enable PWM0 ~PWM7 module
    CLK_EnableModuleClock(PWM0_MODULE);
    CLK_EnableModuleClock(PWM1_MODULE);

    CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL3_PWM0_S_PLL, 0);
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL3_PWM0_S_PLL, 0);
#elif defined(__M032BT__)
    //Enable ADC module
    CLK_EnableModuleClock(ADC_MODULE);

    //Enable PWM0 ~PWM7 module
    CLK_EnableModuleClock(PWM0_MODULE);
    CLK_EnableModuleClock(PWM1_MODULE);
    CLK_EnableModuleClock(BPWM0_MODULE);
    CLK_EnableModuleClock(BPWM1_MODULE);
    // CLK_EnableModuleClock(CLKO_MODULE);
    CLK_EnableModuleClock(UART1_MODULE);
    CLK_EnableModuleClock(USCI0_MODULE);
    CLK_EnableModuleClock(USCI1_MODULE);

    CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL2_PWM0SEL_PCLK0, 0);
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK1, 0);
    CLK_SetModuleClock(BPWM0_MODULE, CLK_CLKSEL2_BPWM0SEL_PCLK0, 0);
    CLK_SetModuleClock(BPWM1_MODULE, CLK_CLKSEL2_BPWM1SEL_PCLK1, 0);

#if ENABLE_C_PRINTF
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
#endif

#elif defined(__NANO1X2__)
    //Enable ADC module
    CLK_EnableModuleClock(ADC_MODULE);

    //Enable PWM0 ~PWM3 module
    CLK_EnableModuleClock(PWM0_CH01_MODULE);
    CLK_EnableModuleClock(PWM0_CH23_MODULE);

    CLK_SetModuleClock(PWM0_CH01_MODULE, CLK_CLKSEL1_PWM0_CH01_S_HCLK, 0);
    CLK_SetModuleClock(PWM0_CH23_MODULE, CLK_CLKSEL1_PWM0_CH23_S_HCLK, 0);
#elif defined(__M252__)
    //Enable ADC module
    CLK_EnableModuleClock(EADC_MODULE);
    /* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
    CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));

    //Enable PWM0 ~PWM1 module
    CLK_EnableModuleClock(PWM0_MODULE);
    CLK_EnableModuleClock(PWM1_MODULE);

    CLK_SetModuleClock(PWM0_MODULE, CLK_CLKSEL2_PWM0SEL_PCLK0, 0);
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK1, 0);
	
#elif defined(__M480__) 
    //Enable ADC module
    CLK_EnableModuleClock(EADC_MODULE);
    /* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
    CLK_SetModuleClock(EADC_MODULE, 0, CLK_CLKDIV0_EADC(8));

    //Enable PWM0 ~PWM1 module
    CLK_EnableModuleClock(EPWM0_MODULE);
    CLK_EnableModuleClock(EPWM1_MODULE);

    CLK_SetModuleClock(EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0);
    CLK_SetModuleClock(EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0);

#elif defined(__M460__) 
    //Enable ADC module
    CLK_EnableModuleClock(EADC0_MODULE);
    /* EADC clock source is 72MHz, set divider to 8, ADC clock is 72/8 MHz */
    //CLK_SetModuleClock(EADC0_MODULE, 0, CLK_CLKDIV0_EADC0(8));
    CLK_SetModuleClock(EADC0_MODULE, CLK_CLKSEL0_EADC0SEL_HCLK, CLK_CLKDIV0_EADC0(8));

    //Enable PWM0 ~PWM1 module
    CLK_EnableModuleClock(EPWM0_MODULE);
    CLK_EnableModuleClock(EPWM1_MODULE);

    CLK_EnableModuleClock(GPA_MODULE);
    CLK_EnableModuleClock(GPB_MODULE);
    CLK_EnableModuleClock(GPC_MODULE);
    CLK_EnableModuleClock(GPG_MODULE);

    CLK_SetModuleClock(EPWM0_MODULE, CLK_CLKSEL2_EPWM0SEL_PCLK0, 0);
    CLK_SetModuleClock(EPWM1_MODULE, CLK_CLKSEL2_EPWM1SEL_PCLK1, 0);

#endif

}
//F_CPU
void init(void)
{
    init_flag = 1;

    /* Unlock protected registers */
    SYS_UnlockReg();
#if defined(__M451__)
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock (Internal RC 22.1184MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Switch HCLK clock source to HXT */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT, CLK_CLKDIV0_HCLK(1));

    /* Set PLL to power down mode and PLLSTB bit in CLKSTATUS register will be cleared by hardware.*/
    CLK->PLLCTL |= CLK_PLLCTL_PD_Msk;

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(F_CPU);

    /* STCLK to HXT */
    //CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLKSEL_HCLK_DIV2);

#elif defined(__NUC240__)
    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(F_CPU);

    /* STCLK to HXT */
    //CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HXT);
#elif defined(__NANO100__)
    /* Enable Internal RC 22.1184MHz clock */
    //CLK_EnableXtalRC(CLK_PWRCTL_HIRC_EN_Msk);

    /* Waiting for clock ready */
    //CLK_WaitClockReady(CLK_CLKSTATUS_HIRC_STB_Msk);

    /* Switch HCLK clock source to HIRC */
    //CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC,CLK_HCLK_CLK_DIVIDER(1));

    /* Enable external XTAL 12MHz clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HXT_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_HXT_STB_Msk);

#if 0
    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(F_CPU);
#else
    /* Enable external 12MHz HXT */
    CLK_EnableXtalRC(CLK_PWRCTL_HXT_EN_Msk);
    CLK_EnablePLL(CLK_PLLCTL_PLL_SRC_HXT, F_CPU * 3);
    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_HXT_STB_Msk | CLK_CLKSTATUS_PLL_STB_Msk);
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_PLL, CLK_HCLK_CLK_DIVIDER(3));
#endif


    /* STCLK to HXT */
    //CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HXT);
#elif defined(__NUC131__)

    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external 12MHz XTAL, internal 22.1184MHz */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk );//| CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for external XTAL clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(F_CPU);

    /* Enable CAN module clock */
    CLK_EnableModuleClock(CAN0_MODULE);

    /* STCLK to HXT */
    //CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HXT);
#elif defined(__M032BT__)
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock (Internal RC 48 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_EnableXtalRC(CLK_PWRCTL_LIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
    CLK_WaitClockReady(CLK_STATUS_LIRCSTB_Msk);

    //  CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV0_HCLK(1)); //will stuck...
    CLK->CLKSEL0 = (CLK->CLKSEL0 & ~ CLK_CLKSEL0_HCLKSEL_Msk) | CLK_CLKSEL0_HCLKSEL_HIRC;

#elif defined(__NANO1X2__)
    /* Set HCLK source form HXT and HCLK source divide 1  */
    //CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT,CLK_HCLK_CLK_DIVIDER(1));

    /* Enable 32KHz LXT and HIRC */
    CLK_EnableXtalRC(CLK_PWRCTL_LXT_EN_Msk | CLK_PWRCTL_HIRC_EN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_CLKSTATUS_LXT_STB_Msk | CLK_CLKSTATUS_HIRC_STB_Msk);

    /*  Set HCLK frequency 32MHz */
    CLK_SetCoreClock(F_CPU);

    /* Enable IP clock */
    //CLK_EnableModuleClock(UART0_MODULE);

    /* Select IP clock source */
    //CLK_SetModuleClock(UART0_MODULE,CLK_CLKSEL1_UART_S_HIRC,CLK_UART_CLK_DIVIDER(1));
#elif defined(__M252__)

    /* Enable HIRC clock (Internal RC 48MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Select HCLK clock source as HIRC and and HCLK source divider as 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(F_CPU);

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1;

    /* Disable digital input path of analog pin XT1_OUT to prevent leakage */
    //GPIO_DISABLE_DIGITAL_PATH(PF, (1ul << 2));

    /* Disable digital input path of analog pin XT1_IN to prevent leakage */
    //GPIO_DISABLE_DIGITAL_PATH(PF, (1ul << 3));
	
#elif defined(__M480__)

    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady( CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(F_CPU);

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;

#elif defined(__M460__)
   /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Wait for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

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
    /* Lock protected registers */
    SYS_LockReg();
}
