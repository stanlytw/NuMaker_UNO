/*----------------------------------------------------------------------------*/
/* This file implement MCU peripherals like: UART, GPIO, etc for BLE          */
/*----------------------------------------------------------------------------*/
#include "Arduino.h"

// check if defined BLE_DTM_ENABLE
#ifndef BLE_DTM_ENABLE
#define BLE_DTM_ENABLE  DISABLE_DEF
#endif


/**************************************************************************
* Private Functions
**************************************************************************/
/** MCU Initial RF Reset Pin
 */
static void setGpioResetInit(void)
{
    //Assign RF Reset pin
#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))
    GPIO_SetMode(PA, BIT4, GPIO_MODE_OUTPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M487JIDAE_B3_
    GPIO_SetMode(PC, BIT10, GPIO_MODE_OUTPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031TD2AE_QFN33_
    GPIO_SetMode(PC, BIT1, GPIO_MODE_OUTPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031_SIP_
    GPIO_SetMode(PA, BIT12, GPIO_MODE_OUTPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M032_SIP_
    GPIO_SetMode(PH, BIT4, GPIO_MODE_OUTPUT);
#endif

    RESET_RF = 1;  //RESET_RF defined in porting_misc.h
}


/** MCU Set RF Reset to Idle Mode to Avoid Leakage Current.
 *
 */
static void setGpioResetIdle(void)
{
    //Assign RF Reset pin
#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))
    GPIO_SetMode(PA, BIT4, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M487JIDAE_B3_
    GPIO_SetMode(PC, BIT10, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031TD2AE_QFN33_
    GPIO_SetMode(PC, BIT1, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031_SIP_
    GPIO_SetMode(PA, BIT12, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M032_SIP_
    GPIO_SetMode(PH, BIT4, GPIO_MODE_INPUT);
#endif
}


/** MCU Set General GPIO Pin.
 *
 * @note Here defines RF interrupt pin and LED pin.
 */
static void setGpioPinInit(void)
{
    //Configure Interrupt pin as Input mode
#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))
    GPIO_SetMode(PA, BIT5, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M487JIDAE_B3_
    GPIO_SetMode(PC, BIT9, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031TD2AE_QFN33_
    GPIO_SetMode(PF, BIT15, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031_SIP_
    GPIO_SetMode(PC, BIT2, GPIO_MODE_INPUT);
#elif _BOARD_SELECTION_ ==  _BOARD_NUVOTON_M032_SIP_
    GPIO_SetMode(PD, BIT12, GPIO_MODE_INPUT);
#endif

    //LED pin assign
#if (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031_SIP_ || _BOARD_SELECTION_ == _BOARD_NUVOTON_M032_SIP_)
    GPIO_SetMode(PF, BIT15, GPIO_MODE_OUTPUT); //LED
    PF15 = 1;                                  //initial off
#else
    GPIO_SetMode(PB, BIT15, GPIO_MODE_OUTPUT); //LED
    PB15 = 0;                                  //initial off
#endif
}


/** spiWriteReg.
 *
 * @note Writes to an 8-bit register with the SPI port.
 */
static void spiGpioWriteReg(const unsigned char regAddr, const unsigned char regData)
{
    unsigned char SPICount;                               // Counter used to clock out the data
    unsigned char SPIData;                                // Define a data structure for the SPI data.

    SPI_CS = 1;                                           // Make sure we start with /CS high
    SPI_CK = 0;                                           // and CK low
    SPI_CS = 0;                                           // Set /CS low to start the SPI cycle 25nS


    setMCU_TinyDelay(1);

    //Address 1th byte
    SPIData = regAddr & 0x7F;
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    {
        if (SPIData & 0x80)                               // Check for a 1
        {
            SPI_MOSI = 1;    // and set the MOSI line appropriately
        }
        else
        {
            SPI_MOSI = 0;
        }

        setMCU_TinyDelay(1);                              // Delay half clk cycle
        SPI_CK = 1;                                       // Toggle the clock line
        setMCU_TinyDelay(1);
        SPI_CK = 0;
        SPIData <<= 1;                                    // Rotate to get the next bit
    }                                                     // and loop back to send the next bit
    // Repeat for the Data byte
    //Address 2nd byte
    SPIData = (regAddr & 0x80) >> 7;
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    {
        if (SPIData & 0x80)                               // Check for a 1
        {
            SPI_MOSI = 1;    // and set the MOSI line appropriately
        }
        else
        {
            SPI_MOSI = 0;
        }

        setMCU_TinyDelay(1);                              // Delay half clk cycle
        SPI_CK = 1;                                       // Toggle the clock line
        setMCU_TinyDelay(1);
        SPI_CK = 0;
        SPIData <<= 1;                                    // Rotate to get the next bit
    }

    //Data
    SPIData = regData;                                    // Preload the data to be sent with Data
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Data
    {
        if (SPIData & 0x80)
        {
            SPI_MOSI = 1;
        }
        else
        {
            SPI_MOSI = 0;
        }

        setMCU_TinyDelay(1);
        SPI_CK = 1;
        setMCU_TinyDelay(1);
        SPI_CK = 0;
        SPIData <<= 1;
    }

    setMCU_TinyDelay(1);

    SPI_CS = 1;
    SPI_MOSI = 0;
}


/** SPI_GPIO_Init.
 *
 */
static void SPI_GPIO_Init(void)
{
    //For SPI I/O remapping, set all GPIO pin as output
#if (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)
    GPIO_SetMode(PA, BIT2, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT0, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT1, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT5, GPIO_MODE_OUTPUT);  //PA5
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_)
    GPIO_SetMode(PA, BIT2, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT0, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT1, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT5, GPIO_MODE_OUTPUT);  //PA5
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M487JIDAE_B3_)
    GPIO_SetMode(PA, BIT2, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT0, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT1, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT9, GPIO_MODE_OUTPUT);  //PC9
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031TD2AE_QFN33_)
    GPIO_SetMode(PA, BIT2, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT0, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT1, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PF, BIT15, GPIO_MODE_OUTPUT); //PF15
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031_SIP_)
    GPIO_SetMode(PD, BIT2, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PD, BIT0, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PD, BIT1, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PD, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT2, GPIO_MODE_OUTPUT);  //PC2
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M032_SIP_)
    GPIO_SetMode(PF, BIT6, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PF, BIT7, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PF, BIT8, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PF, BIT9, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PD, BIT12, GPIO_MODE_OUTPUT); //PD12
#endif
}


/** SYS_Disable_AnalogPORCircuit.
 *
 */
static void SYS_Disable_AnalogPORCircuit(void)
{
    SYS->PORDISAN = 0x5AA5;
}



/******************************************************************************
 * Public Functions
 ******************************************************************************/
/** MCU Initial RF Reset Pin
 *
 */
void seBLE_GpioReset(void)
{
    //Init reset gpio pin
    setGpioResetInit();

    //Do Reset: pulse low
    RESET_RF = 1;
    setMCU_TinyDelay(1000);       //1ms

    RESET_RF = 0;
    setMCU_TinyDelay(1000);       //1ms

    RESET_RF = 1;

    //Set reset pin to input mode to avoid leakage current in sleep/ deep sleep mode
    setGpioResetIdle();
}



/** MCU Set Enable RF Interrupt Pin.
 *
 */
void setBLE_GpioIntEnable(void)
{
#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))

    GPIO_SetMode(PA, BIT5, GPIO_MODE_INPUT);

    if (getBLE_ChipId() == MP_A1)
    {
        GPIO_EnableInt(PA, 5, GPIO_INT_RISING);
    }
    else
    {
#if 1
        GPIO_EnableInt(PA, 5, GPIO_INT_HIGH);
#else   //(0)
        GPIO_EnableInt(PA, 5, GPIO_INT_RISING);
#endif  //(0)
    }
    NVIC_EnableIRQ(GPIO_PAPB_IRQn);

#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M487JIDAE_B3_)

    GPIO_SetMode(PC, BIT9, GPIO_MODE_INPUT);

    if (getBLE_ChipId() == MP_A1)
    {
        GPIO_EnableInt(PC, 9, GPIO_INT_RISING);
    }
    else
    {
        GPIO_EnableInt(PC, 9, GPIO_INT_HIGH);
    }
    NVIC_EnableIRQ(GPC_IRQn);

#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031TD2AE_QFN33_)

    GPIO_SetMode(PF, BIT15, GPIO_MODE_INPUT);

    if (getBLE_ChipId() == MP_A1)
    {
        GPIO_EnableInt(PF, 15, GPIO_INT_RISING);
    }
    else
    {
        GPIO_EnableInt(PF, 15, GPIO_INT_HIGH);
    }
    NVIC_EnableIRQ(GPIO_PCPDPEPF_IRQn);

#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031_SIP_)

    GPIO_SetMode(PC, BIT2, GPIO_MODE_INPUT);

    if (getBLE_ChipId() == MP_A1)
    {
        GPIO_EnableInt(PC, 2, GPIO_INT_RISING);
    }
    else
    {
        GPIO_EnableInt(PC, 2, GPIO_INT_HIGH);
    }
    NVIC_EnableIRQ(GPIO_PCPDPEPF_IRQn);

#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M032_SIP_)
    SYS_UnlockReg();
    // Set PD.12 to EINT5 mode ...
    SYS->GPD_MFPH = (SYS->GPD_MFPH & ~SYS_GPD_MFPH_PD12MFP_Msk) | SYS_GPD_MFPH_PD12MFP_INT5;
    SYS_LockReg();

    // Configure PD.12 as Input mode and enable interrupt by rising edge trigger
    GPIO_SetMode(PD, BIT12, GPIO_MODE_INPUT);

    if (getBLE_ChipId() == MP_A1)
    {
        GPIO_EnableInt(PD, 12, GPIO_INT_RISING);
    }
    else
    {
        GPIO_EnableInt(PD, 12, GPIO_INT_HIGH);
    }
    NVIC_EnableIRQ(EINT135_IRQn);

#endif //(_BOARD_SELECTION_==_BOARD_NUVOTON_M031SE_)

}


/** MCU Set Disable RF Interrupt Pin.
 */
void setBLE_GpioIntDisable(void)
{
#if (_BOARD_SELECTION_   == _BOARD_NUVOTON_M031SE_)
    GPIO_DisableInt(PA, 5);
    NVIC_DisableIRQ(GPIO_PAPB_IRQn);
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_)
    GPIO_DisableInt(PA, 5);
    NVIC_DisableIRQ(GPIO_PAPB_IRQn);
#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M487JIDAE_B3_)
    GPIO_DisableInt(PC, 9);
    NVIC_DisableIRQ(GPC_IRQn);
#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031TD2AE_QFN33_)
    GPIO_DisableInt(PF, 15);
    NVIC_DisableIRQ(GPIO_PCPDPEPF_IRQn);
#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M031_SIP_)
    GPIO_DisableInt(PC, 2);
    NVIC_DisableIRQ(GPIO_PCPDPEPF_IRQn);
#elif (_BOARD_SELECTION_ ==  _BOARD_NUVOTON_M032_SIP_)
    GPIO_DisableInt(PD, 12);
    NVIC_DisableIRQ(EINT135_IRQn);
#endif  //(_BOARD_SELECTION_)
}



/** MCU SPI IO mapping.
 *  Must do this after Power ON and MCU GPIO initialed.
 *
 */
void setRF_SpiIoMapping(void)
{
    //(1) Set all SPI GPIO pin as output. Be careful not to set 5 pins as HIGH, it will trigger HW mapping SPI pin
    SPI_GPIO_Init();    //Here set all CLK,CS,MOSI,NISO,INT as output
    SPI_CS = 1;
    SPI_CK = 0;
    SPI_MOSI = 0;
    SPI_MISO = 0;
    DEFAULT_INT = 0;

    //(2) Write R248, R249 GPIO select
#if (SPI_IO_ORDER==1)
    //Write R248=8'b10,001,000, R249=8'b0,100,011,0
    //GPIO0[2:0]=0 - CS
    //GPIO1[2:0]=1 - CLK
    //GPIO2[2:0]=2 - MOSI
    //GPIO3[2:0]=3 - MISO
    //GPIO4[2:0]=4 - INT
    setMCU_TinyDelay(50);
    spiGpioWriteReg(248, 0x88);
    setMCU_TinyDelay(50);
    spiGpioWriteReg(249, 0x46);
    setMCU_TinyDelay(50);
#elif (SPI_IO_ORDER==2)
    //Write R248=8'b01,000,100, R249=8'b0,011,010,0
    //GPIO0[2:0]=4 - INT
    //GPIO1[2:0]=0 - CS
    //GPIO2[2:0]=1 - CK
    //GPIO3[2:0]=2 - MOSI
    //GPIO4[2:0]=3 - MISO
    setMCU_TinyDelay(50);
    spiGpioWriteReg(248, 4 | (0 << 3) | ((1 & 0x03) << 6));
    setMCU_TinyDelay(50);
    spiGpioWriteReg(249, ((1 & 0x04) >> 2) | (2 << 1) | (3 << 4));
    setMCU_TinyDelay(50);
#elif (SPI_IO_ORDER==3)
    //Write R248=8'b00,100,011, R249=8'b0,010,001,0
    //GPIO0[2:0]=3 - MISO
    //GPIO1[2:0]=4 - INT
    //GPIO2[2:0]=0 - CS
    //GPIO3[2:0]=1 - CK
    //GPIO4[2:0]=2 - MOSI
    setMCU_TinyDelay(50);
    spiGpioWriteReg(248, 3 | (4 << 3) | ((0 & 0x03) << 6));
    setMCU_TinyDelay(50);
    spiGpioWriteReg(249, ((0 & 0x04) >> 2) | (1 << 1) | (2 << 4));
    setMCU_TinyDelay(50);
#elif (SPI_IO_ORDER==4)
    //Write R248=8'b00,100,011, R249=8'b0,010,001,0
    //GPIO0[2:0]=2 - MOSI
    //GPIO1[2:0]=3 - MISO
    //GPIO2[2:0]=4 - INT
    //GPIO3[2:0]=0 - CS
    //GPIO4[2:0]=1 - CK
    setMCU_TinyDelay(50);
    spiGpioWriteReg(248, 2 | (3 << 3) | ((4 & 0x03) << 6));
    setMCU_TinyDelay(50);
    spiGpioWriteReg(249, ((4 & 0x04) >> 2) | (0 << 1) | (1 << 4));
    setMCU_TinyDelay(50);
#elif (SPI_IO_ORDER==5)
    //Write R248=8'b00,100,011, R249=8'b0,010,001,0
    //GPIO0[2:0]=1 - CK
    //GPIO1[2:0]=2 - MOSI
    //GPIO2[2:0]=3 - MISO
    //GPIO3[2:0]=4 - INT
    //GPIO4[2:0]=0 - CS
    setMCU_TinyDelay(50);
    spiGpioWriteReg(248, 1 | (2 << 3) | ((3 & 0x03) << 6));
    setMCU_TinyDelay(50);
    spiGpioWriteReg(249, ((3 & 0x04) >> 2) | (4 << 1) | (0 << 4));
    setMCU_TinyDelay(50);
#elif (SPI_IO_ORDER==6)
    //Write R248=8'b01,000,100, R249=8'b0,010,011,0
    //GPIO0[2:0]=4 - INT
    //GPIO1[2:0]=0 - CS
    //GPIO2[2:0]=1 - CK
    //GPIO3[2:0]=3 - MISO
    //GPIO4[2:0]=2 - MOSI
    setMCU_TinyDelay(50);
    spiGpioWriteReg(248, 4 | (0 << 3) | ((1 & 0x03) << 6));
    setMCU_TinyDelay(50);
    spiGpioWriteReg(249, ((1 & 0x04) >> 2) | (3 << 1) | (2 << 4));
    setMCU_TinyDelay(50);
#elif (SPI_IO_ORDER==7)
    //Write R248=8'b10,001,000, R249=8'b0,100,011,0
    //GPIO0[2:0]=0 - CS
    //GPIO1[2:0]=1 - CLK
    //GPIO2[2:0]=3 - MISO
    //GPIO3[2:0]=2 - MOSI
    //GPIO4[2:0]=4 - INT
    setMCU_TinyDelay(50);
    spiGpioWriteReg(248, 0 | (1 << 3) | ((3 & 0x03) << 6));
    setMCU_TinyDelay(50);
    spiGpioWriteReg(249, ((3 & 0x04) >> 2) | (2 << 1) | (4 << 4));
    setMCU_TinyDelay(50);
#endif


    //(3) Output all pin as HIGH, last 10ms(>1ms). trigger HW take effect
    SPI_GPIO_Init();          //Set all as output
    SPI_CS = 1;
    SPI_CK = 1;
    SPI_MOSI = 1;
    SPI_MISO = 1;
    DEFAULT_INT = 1;
    setMCU_TinyDelay(10000);  //Delay 10ms

    //(4) Init GPIO & SPI
    setGpioPinInit();        //Set GPIO interrupt pin as input
    setBLE_SpiInit();        //Intial SPI pin, change MISO direction as INPUT

#if (SPI_IO_ORDER==1)
    setBLE_SpiOneByteTx(249, (0x46 | 0x80));                                      //set RF MISO, INT as output
#elif (SPI_IO_ORDER==2)
    setBLE_SpiOneByteTx(249, ((1 & 0x04) >> 2) | (2 << 1) | (3 << 4) | (1 << 7)); //set RF MISO, INT as output
#elif (SPI_IO_ORDER==3)
    setBLE_SpiOneByteTx(249, ((0 & 0x04) >> 2) | (1 << 1) | (2 << 4) | (1 << 7)); //set RF MISO, INT as output
#elif (SPI_IO_ORDER==4)
    setBLE_SpiOneByteTx(249, ((4 & 0x04) >> 2) | (0 << 1) | (1 << 4) | (1 << 7)); //set RF MISO, INT as output
#elif (SPI_IO_ORDER==5)
    setBLE_SpiOneByteTx(249, ((3 & 0x04) >> 2) | (4 << 1) | (0 << 4) | (1 << 7)); //set RF MISO, INT as output
#elif (SPI_IO_ORDER==6)  //for SIP
    setBLE_SpiOneByteTx(249, ((1 & 0x04) >> 2) | (3 << 1) | (2 << 4) | (1 << 7)); //set RF MISO, INT as output
#elif (SPI_IO_ORDER==7)  //for daughter board
    setBLE_SpiOneByteTx(249, ((3 & 0x04) >> 2) | (2 << 1) | (4 << 4) | (1 << 7)); //set RF MISO, INT as output
#endif

    //(5) Set RF init values
    //manual control
    setBLE_SpiOneByteTx(53, 0xC0);

    //enable LDO
    setBLE_SpiOneByteTx(40, 0xC0);
    setBLE_SpiWaitAndSSHigh();

    setMCU_TinyDelay(25000);          //Put delay after LDO_enable, or set register may have strange behavior!

    //enable chip
    setBLE_SpiOneByteTx(53, 0x80);    //Enable chip
    setBLE_SpiWaitAndSSHigh();
    setMCU_TinyDelay(10000);          //Put delay after chip_enable, or set register may have strange behavior!
}



/** MCU Enter System Power Down Mode.
 *
 */
void setMCU_SystemPowerDown(void)
{
    if (getRF_Mode() != BLERFMODE_ACTIVE) //RF is idle or sleep mode
    {
        /* Check if all the debug messages are finished */
        UART_WAIT_TX_EMPTY(UART0);

        /* Unlock protected registers before entering Power-down mode */
        SYS_UnlockReg();

        /* LVR must be enabled and the POR will be enabled automatically */
        SYS_ENABLE_LVR();

        /* Turn off internal analog POR circuit */
        SYS_Disable_AnalogPORCircuit();

        /* Disable Power-on Reset */
        SYS_DISABLE_POR();

        /* Enter to Power-down mode */
        CLK_PowerDown();

        /* Lock protected registers */
        SYS_LockReg();
    }
    else
    {
        CLK_Idle();
    }
}



/** MCU Implemented Tiny Delay Function.
 *
 * @param[in] u32Usec   : delay time in microseconds.
 */
void setMCU_TinyDelay(uint32_t u32Usec)
{
    CLK_SysTickDelay(u32Usec);
}

void MCU_WDTmr_En(void)
{
    extern void WDT_Open(uint32_t u32TimeoutInterval, uint32_t u32ResetDelay, uint32_t u32EnableReset, uint32_t u32EnableWakeup);

    InterruptDisable();
    SYS_UnlockReg();

    CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDTSEL_LIRC, 0);
    CLK_EnableModuleClock(WDT_MODULE);
    SystemCoreClockUpdate();

    WDT_Open(WDT_TIMEOUT_2POW4, WDT_RESET_DELAY_130CLK, TRUE, FALSE);
    NVIC_EnableIRQ(WDT_IRQn);
    WDT_EnableInt();

    SYS_LockReg();
    InterruptEnable();
}



/* GPIO Interrupt Handler */
#pragma push
#pragma Otime
#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))
void GPABGH_IRQHandler(void)
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M487JIDAE_B3_)
void GPC_IRQHandler(void)
#elif ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031TD2AE_QFN33_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M031_SIP_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032_SIP_))
void EINT135_IRQHandler(void) // use external Interrupt for Arduino framework
#endif
{
    volatile uint32_t temp;
    //Clear MCU GPIO Int status
#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))
    if (GPIO_GET_INT_FLAG(PA, BIT5))
    {
        GPIO_CLR_INT_FLAG(PA, BIT5);
#if (BLE_DTM_ENABLE == ENABLE_DEF )
        BleDTM_Isr();
#else
        setBLE_BleStackGpio_Isr();    // Put this on the top following the clearing MCU GPIO Int status process.
#endif
    }
    else
    {
        // Un-expected interrupt. Just clear all PA interrupts
        temp = PA->INTSRC;
        PA->INTSRC = temp;
    }
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M487JIDAE_B3_)
    if (GPIO_GET_INT_FLAG(PC, BIT9))
    {
        GPIO_CLR_INT_FLAG(PC, BIT9);
#if (BLE_DTM_ENABLE == ENABLE_DEF )
        BleDTM_Isr();
#else
        setBLE_BleStackGpio_Isr();    // Put this on the top following the clearing MCU GPIO Int status process.
#endif
    }
    else
    {
        // Un-expected interrupt. Just clear all PC interrupts
        temp = PC->INTSRC;
        PC->INTSRC = temp;
    }
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031TD2AE_QFN33_)
    if (GPIO_GET_INT_FLAG(PF, BIT15))
    {
        GPIO_CLR_INT_FLAG(PF, BIT15);
#if (BLE_DTM_ENABLE == ENABLE_DEF )
        BleDTM_Isr();
#else
        setBLE_BleStackGpio_Isr();    // Put this on the top following the clearing MCU GPIO Int status process.
#endif
    }
    else
    {
        // Un-expected interrupt. Just clear all PF interrupts
        temp = PF->INTSRC;
        PF->INTSRC = temp;
    }
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031_SIP_)
    if (GPIO_GET_INT_FLAG(PC, BIT2))
    {
        GPIO_CLR_INT_FLAG(PC, BIT2);
#if (BLE_DTM_ENABLE == ENABLE_DEF )
        BleDTM_Isr();
#else
        setBLE_BleStackGpio_Isr();    // Put this on the top following the clearing MCU GPIO Int status process.
#endif
    }
    else
    {
        /* Un-expected interrupt. Just clear all PC interrupts */
        temp = PC->INTSRC;
        PC->INTSRC = temp;
    }
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M032_SIP_)
    if (GPIO_GET_INT_FLAG(PD, BIT12))
    {
        GPIO_CLR_INT_FLAG(PD, BIT12);
#if (BLE_DTM_ENABLE == ENABLE_DEF )
        BleDTM_Isr();
#else
        setBLE_BleStackGpio_Isr();    // Put this on the top following the clearing MCU GPIO Int status process.
#endif
    }
    else
    {
        /* Un-expected interrupt. Just clear all PC interrupts */
        temp = PD->INTSRC;
        PD->INTSRC = temp;
    }
#endif // (_BOARD_SELECTION_ == _BOARD_NUVOTON_xxxx)
}  //end of ISR

#pragma pop





