/*
  This file is part of the ArduinoBLE library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#if defined(__M032BT__)
#include "HCINuvotonTransport.h"

HCINuvotonTransportClass::HCINuvotonTransportClass(bool dataReadyDefault = 0) :
    _bleDataReady(dataReadyDefault),
    _begun(false),
    _u32seed(0)
{
}

HCINuvotonTransportClass::~HCINuvotonTransportClass()
{
}

static void tmr_ISR(uint8_t num)
{
    HCINuvotonTransport.setRandomSeed(SysTick->VAL);
}

/*
 * use cross clock domain jitter to get random seed
 */
static void getSeed(void)
{
    HardwareTimer *tmr = Timer[3]; //use timer3 for seed sampling

    SYS_UnlockReg();
    CLK_EnableModuleClock(TMR3_MODULE);
    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_LIRC, 0);
    SYS_LockReg();

    tmr->open(ONESHOT, tmr->getModuleClock());
    tmr->setCompare(tmr->getModuleClock() / 1000);  /* milliseconds */
    tmr->attachInterrupt(tmr_ISR);
    tmr->start();
    while (!HCINuvotonTransport.getRandomNumberSeed());

    /* recover initial timer setting*/
    SYS_UnlockReg();
    tmr->detachInterrupt();
    tmr->close();

    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_PCLK1, 0);
    SYS_LockReg();
}

int HCINuvotonTransportClass::begin()
{
    if (false == _begun)
    {
        BleStackStatus status;
        BLE_Addr_Param bleAddrParam;

        _nuvoton_BLE_LL_init();
        /* Set BD ADDR */
        bleAddrParam.addrType = RANDOM_ADDR;
        getSeed();

        randomSeed(_u32seed);
        bleAddrParam.addr[0] = random(256);
        bleAddrParam.addr[1] = random(256);
        bleAddrParam.addr[2] = random(256);
        bleAddrParam.addr[3] = random(256);
        bleAddrParam.addr[4] = random(256);
        bleAddrParam.addr[5] = 0xC0 | random(256);
        status = setBLE_BleDeviceAddr(&bleAddrParam);

        if (status != BLESTACK_STATUS_SUCCESS)
        {
            printf("setBLE_BleDeviceAddr() returns fail %d\n", status);
            return eNUVOTON_TRANSPORT_FAIL;
        }

        _begun = true;
    }

    return eNUVOTON_TRANSPORT_SUCCESS;
}

void HCINuvotonTransportClass::end()
{
    _begun = false;
    _u32seed = 0;
}

void HCINuvotonTransportClass::wait(unsigned long timeout)
{
    for (unsigned long start = millis(); (millis() - start) < timeout;)
    {
        if (available())
        {
            break;
        }
    }
}

int HCINuvotonTransportClass::available()
{
    setBLE_KernelStateHandle();
    return _bleDataReady;
}

int HCINuvotonTransportClass::peek()
{
    return 0;
}

int HCINuvotonTransportClass::read()
{
    /*the function not used in Nuvoton LL Transport*/
    return 1;
}

HCINuvotonTransportErrorCode HCINuvotonTransportClass::readRawData2ParserBuf(uint8_t *parserBufAddr, uint16_t parser_buf_len)
{
    if (_u16rcv_length > parser_buf_len)
    {
        return eNUVOTON_TRANSPORT_FAIL;
    }

    memcpy(parserBufAddr, _pu8_rawdataBuf, _u16rcv_length);

    return eNUVOTON_TRANSPORT_SUCCESS;
}

size_t HCINuvotonTransportClass::write(const uint8_t *data, size_t length)
{

    BleStackStatus status = setBLE_HCICommand((uint8_t *)data, length);
    if (status != BLESTACK_STATUS_SUCCESS)
    {
        printf("Tx to LL error: %d\n", status);
        return 0;
    }
    return length;
}

void HCINuvotonTransportClass::_cb_setDataReady(uint8_t *pHCIbufAddr, uint16_t length)
{
    _bleDataReady    = true; //set data ready
    _pu8_rawdataBuf  = pHCIbufAddr;
    _u16rcv_length   = length;

}

void HCINuvotonTransportClass::_nuvoton_BLE_LL_init(void)
{

    _init_peripheral_for_LL();
    _BLE_StackInit();

    /*
      For Arduino framework timing counting, recover the
      systick setting to 1ms interval after LL lib setting.
    */
    if (SysTick_Config(F_CPU / 1000))
    {
        // Capture error
        while (true);
    }

    while (setBLE_KernelStateHandle() != BLESTACK_STATUS_FREE);
}

void HCINuvotonTransportClass::_init_peripheral_for_LL(void)
{
    int8_t irqno;
    /* Unlock protected registers */
    SYS_UnlockReg();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/

#if (_BOARD_SELECTION_==_BOARD_NUVOTON_M031TD2AE_QFN33_)   //HIRC
    //GPIO_SetMode(PF, (BIT2|BIT3), GPIO_MODE_INPUT);
    // Enable HIRC
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    // Waiting for HIRC clock ready
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

#elif (_BOARD_SELECTION_==_BOARD_NUVOTON_M031_SIP_ || _BOARD_SELECTION_==_BOARD_NUVOTON_M032_SIP_)
#if (_USE_MCU_CLK_==MCU_CLK_SOURCE_HXT) //HXT
    GPIO_SetMode(PF, (BIT2 | BIT3), GPIO_MODE_INPUT);
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk | CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk | CLK_STATUS_HIRCSTB_Msk);
#else                                   //HIRC   
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
#endif
#else
    //GPIO_SetMode(PF, (BIT2|BIT3), GPIO_MODE_INPUT);
    // Enable HXT
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    // Waiting for HXT clock ready
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
#endif

    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

#if (_BOARD_SELECTION_==_BOARD_NUVOTON_M487JIDAE_B3_)  //M487
    /* Set both PCLK0 and PCLK1 as HCLK/PCLK_DIV */
#if (PCLK_DIV==4)
    CLK->PCLKDIV = CLK_PCLKDIV_PCLK0DIV4 | CLK_PCLKDIV_PCLK1DIV4;  //96/4=24MHz
#elif (PCLK_DIV==2)
    CLK->PCLKDIV = CLK_PCLKDIV_PCLK0DIV2 | CLK_PCLKDIV_PCLK1DIV2;  //64/2=32MHz
#elif (PCLK_DIV==1)
    CLK->PCLKDIV = CLK_PCLKDIV_PCLK0DIV1 | CLK_PCLKDIV_PCLK1DIV1;
#endif //(PCLK_DIV==4)

#else //M031 series
    /* Set both PCLK0 and PCLK1 as HCLK/PCLK_DIV */
#if (PCLK_DIV==4)
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV4 | CLK_PCLKDIV_APB1DIV_DIV4;
#elif (PCLK_DIV==2)
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;  //48/2=24MHz
#elif (PCLK_DIV==1)
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1;
#endif //(PCLK_DIV==4)

#endif //(_BOARD_SELECTION_==_BOARD_NUVOTON_M487JIDAE_B3_))

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
#if (_BOARD_SELECTION_==_BOARD_NUVOTON_M031_SIP_ || _BOARD_SELECTION_==_BOARD_NUVOTON_M032_SIP_)   //Nuvoton SIP EVK
    /* Set PA multi-function pins for UART0 RXD=PA.0 and TXD=PA.1 */
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~(SYS_GPA_MFPL_PA0MFP_Msk | SYS_GPA_MFPL_PA1MFP_Msk)) |
                    (SYS_GPA_MFPL_PA0MFP_UART0_RXD | SYS_GPA_MFPL_PA1MFP_UART0_TXD);
#else
    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk)) |     //It is also VCOM TX/RX port
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);
#endif

    /* Set only BLE interrupt with the highest priority to mkae sure RF can handle event in time */
    for (irqno = BOD_IRQn; irqno <= RTC_IRQn; irqno++)
    {
        NVIC_SetPriority((IRQn_Type)irqno, 1);
    }
#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)||(_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))
    NVIC_SetPriority(GPIO_PAPB_IRQn, 0);
#else
    NVIC_SetPriority(EINT135_IRQn, 0);
#endif

    /* Change HIRC stable count; 0x00 = 1024, 0x01 = 512 (default), 0x02 = 256, 0x03 = 16 */
    CLK->PWRCTL = (CLK->PWRCTL & ~(0x3 << 16)) | (0x02 << 16);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Update System Core Clock                                                                               */
    /*---------------------------------------------------------------------------------------------------------*/
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

}

BleStackStatus HCINuvotonTransportClass::_BLE_StackInit(void)
{
    BleStackStatus status;

    /* Wait RF PHY stable */
    CLK_SysTickDelay(200000);    // MUST wait for stable voltage( >= 1.8V) after power-up

    /* Do Gpio Reset */
    seBLE_GpioReset();
    CLK_SysTickDelay(50000);     // HW 32K clk count 15ms, but need to consider 32K deviation & MCU HIRC deviation

    /* SPI IO remapping */
    setRF_SpiIoMapping();

    /* Init SPI PDMA */
    setBLE_SpiPDMAInit();

    /* Init RF PHY */
    status = setRF_Init(DCDC_REGULATOR, XTAL_16M);   //EnableGpioInterrupt in the end of this function
    BLESTACK_STATUS_CHECK(status);

    /* Init BLE Stack */
    status = setBLE_BleStackInit();
    BLESTACK_STATUS_CHECK(status);

    return BLESTACK_STATUS_SUCCESS;
}

HCINuvotonTransportClass HCINuvotonTransport_instance(0);
HCINuvotonTransportClass &HCINuvotonTransport = HCINuvotonTransport_instance;
HCITransportInterface &HCITransport = HCINuvotonTransport;

void HCI_EVENT_CALLBACK(uint8_t *pHCIbufAddr, uint16_t length)
{
    /*** analysis HCI Evenet***/

    /*************************/
    HCINuvotonTransport._cb_setDataReady(pHCIbufAddr, length);
}

#else
#error "Unsupported board selected!"
#endif
