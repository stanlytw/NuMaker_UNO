/***************************************************************************//**
 * @file     vcom_serial.c
 * @version  V3.00
 * @brief    HSUSBD virtual COM sample file.
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
/*!<Includes */
#if defined(__M460MINIMA__)
#include <string.h>
#include "NuMicro.h"
#include "HardwareSerial.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "vcom_serial.h"
uint8_t volatile g_u8EPAReady = 0;
//uint8_t volatile g_u8GoLDR = 0;
uint32_t g_u32EpAMaxPacketSize;
uint32_t g_u32EpBMaxPacketSize;
uint32_t g_u32EpDMaxPacketSize;
uint32_t g_u32EpEMaxPacketSize;
uint8_t volatile g_u8UsbDataReady = 0;
__attribute__((aligned(4))) uint8_t g_u8UsbRcvBuff[64];

/***************************************************************/
#define HID_CMD_SIGNATURE   0x43444948

/* HID Transfer Commands */
#define HID_CMD_NONE     0x00
#define HID_CMD_ERASE    0x71
#define HID_CMD_READ     0xD2
#define HID_CMD_WRITE    0xC3
#define HID_CMD_TEST     0xB4

#if (EP_MAX_PKT_SIZE == 1024)
#define PAGE_SIZE        1024
#else
#define PAGE_SIZE        512
#endif

#ifdef __ICCARM__
typedef __packed struct
{
    uint8_t u8Cmd;
    uint8_t u8Size;
    uint32_t u32Arg1;
    uint32_t u32Arg2;
    uint32_t u32Signature;
    uint32_t u32Checksum;
} CMD_T;

#else
typedef struct __attribute__((__packed__))
{
    uint8_t u8Cmd;
    uint8_t u8Size;
    uint32_t u32Arg1;
    uint32_t u32Arg2;
    uint32_t u32Signature;
    uint32_t u32Checksum;
}
CMD_T;
#endif

CMD_T gCmd;

#ifdef __ICCARM__
#pragma data_alignment=4
static uint8_t  g_u8PageBuff[PAGE_SIZE] = {0};    /* Page buffer to upload/download through HID report */
static uint32_t g_u32BytesInPageBuf = 0;          /* The bytes of data in g_u8PageBuff */
uint8_t  g_u8OutBuff[EPB_MAX_PKT_SIZE] = {0};
#else
static uint8_t  g_u8PageBuff[PAGE_SIZE] __attribute__((aligned(4))) = {0};    /* Page buffer to upload/download through HID report */
static uint32_t g_u32BytesInPageBuf __attribute__((aligned(4))) = 0;          /* The bytes of data in g_u8PageBuff */
uint8_t  g_u8OutBuff[EPB_MAX_PKT_SIZE] __attribute__((aligned(4))) = {0};
#endif
void TIM_scheduler_Config(void);
/*--------------------------------------------------------------------------*/
void USBD20_IRQHandler(void)
{
    __IO uint32_t IrqStL, IrqSt;

    IrqStL = HSUSBD->GINTSTS & HSUSBD->GINTEN;    /* get interrupt status */

    if(!IrqStL)    return;

    /* USB interrupt */
    if(IrqStL & HSUSBD_GINTSTS_USBIF_Msk)
    {
        IrqSt = HSUSBD->BUSINTSTS & HSUSBD->BUSINTEN;

        if(IrqSt & HSUSBD_BUSINTSTS_SOFIF_Msk)
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SOFIF_Msk);

        if(IrqSt & HSUSBD_BUSINTSTS_RSTIF_Msk)
        {
            HSUSBD_SwReset();

            HSUSBD_ResetDMA();
            HSUSBD->EP[EPA].EPRSPCTL = HSUSBD_EPRSPCTL_FLUSH_Msk;
            HSUSBD->EP[EPB].EPRSPCTL = HSUSBD_EPRSPCTL_FLUSH_Msk;
            HSUSBD->EP[EPD].EPRSPCTL = HSUSBD_EPRSPCTL_FLUSH_Msk;
            HSUSBD->EP[EPE].EPRSPCTL = HSUSBD_EPRSPCTL_FLUSH_Msk;

            if(HSUSBD->OPER & 0x04)   /* high speed */
            {
                VCOM_InitForHighSpeed();
                HID_InitForHighSpeed();
            }
            else                    /* full speed */
            {
                //VCOM_InitForFullSpeed();
            }
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
            HSUSBD_SET_ADDR(0);
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk | HSUSBD_BUSINTEN_SUSPENDIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RSTIF_Msk);
            HSUSBD_CLR_CEP_INT_FLAG(0x1ffc);
        }

        if(IrqSt & HSUSBD_BUSINTSTS_RESUMEIF_Msk)
        {
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_SUSPENDIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_RESUMEIF_Msk);
        }

        if(IrqSt & HSUSBD_BUSINTSTS_SUSPENDIF_Msk)
        {
            HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_SUSPENDIF_Msk);
        }

        if(IrqSt & HSUSBD_BUSINTSTS_HISPDIF_Msk)
        {
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_HISPDIF_Msk);
        }

        if(IrqSt & HSUSBD_BUSINTSTS_DMADONEIF_Msk)
        {
            g_hsusbd_DmaDone = 1;
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_DMADONEIF_Msk);

            if(!(HSUSBD->DMACTL & HSUSBD_DMACTL_DMARD_Msk))
            {
                HSUSBD_ENABLE_EP_INT(EPB, HSUSBD_EPINTEN_RXPKIEN_Msk);
                HSUSBD_ENABLE_EP_INT(EPE, HSUSBD_EPINTEN_RXPKIEN_Msk);
            }

            if(HSUSBD->DMACTL & HSUSBD_DMACTL_DMARD_Msk)
            {
                if(g_hsusbd_ShortPacket == 1)
                {
                    HSUSBD->EP[EPA].EPRSPCTL = (HSUSBD->EP[EPA].EPRSPCTL & 0x10) | HSUSBD_EP_RSPCTL_SHORTTXEN;    // packet end
                    HSUSBD->EP[EPD].EPRSPCTL = (HSUSBD->EP[EPD].EPRSPCTL & 0x10) | HSUSBD_EP_RSPCTL_SHORTTXEN;    // packet end
                    g_hsusbd_ShortPacket = 0;
                }
            }
        }

        if(IrqSt & HSUSBD_BUSINTSTS_PHYCLKVLDIF_Msk)
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_PHYCLKVLDIF_Msk);

        if(IrqSt & HSUSBD_BUSINTSTS_VBUSDETIF_Msk)
        {
            if(HSUSBD_IS_ATTACHED())
            {
                /* USB Plug In */
                HSUSBD_ENABLE_USB();
            }
            else
            {
                /* USB Un-plug */
                HSUSBD_DISABLE_USB();
            }
            HSUSBD_CLR_BUS_INT_FLAG(HSUSBD_BUSINTSTS_VBUSDETIF_Msk);
        }
    }

    if(IrqStL & HSUSBD_GINTSTS_CEPIF_Msk)
    {
        IrqSt = HSUSBD->CEPINTSTS & HSUSBD->CEPINTEN;

        if(IrqSt & HSUSBD_CEPINTSTS_SETUPTKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPTKIF_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_SETUPPKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_SETUPPKIF_Msk);
            HSUSBD_ProcessSetupPacket();
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_OUTTKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_OUTTKIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_INTKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
            if(!(IrqSt & HSUSBD_CEPINTSTS_STSDONEIF_Msk))
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk);
                HSUSBD_CtrlIn();
            }
            else
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_TXPKIEN_Msk | HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            }
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_PINGIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_PINGIF_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_TXPKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
            if(g_hsusbd_CtrlInSize)
            {
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
            }
            else
            {
                if(g_hsusbd_CtrlZero == 1)
                    HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_ZEROLEN);
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            }
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_TXPKIF_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_RXPKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_RXPKIF_Msk);
            HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_STSDONEIEN_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_NAKIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_NAKIF_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_STALLIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STALLIF_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_ERRIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_ERRIF_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_STSDONEIF_Msk)
        {
            HSUSBD_UpdateDeviceState();
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
            HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_BUFFULLIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_BUFFULLIF_Msk);
            return;
        }

        if(IrqSt & HSUSBD_CEPINTSTS_BUFEMPTYIF_Msk)
        {
            HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_BUFEMPTYIF_Msk);
            return;
        }
    }

    /* bulk in */
    if(IrqStL & HSUSBD_GINTSTS_EPAIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPA].EPINTSTS & HSUSBD->EP[EPA].EPINTEN;

        gu32TxSize = 0;
        HSUSBD_ENABLE_EP_INT(EPA, 0);
        HSUSBD_CLR_EP_INT_FLAG(EPA, IrqSt);
    }
    /* bulk out */
    if(IrqStL & HSUSBD_GINTSTS_EPBIF_Msk)
    {
        int volatile i;

        IrqSt = HSUSBD->EP[EPB].EPINTSTS & HSUSBD->EP[EPB].EPINTEN;
        gu32RxSize = HSUSBD->EP[EPB].EPDATCNT & 0xffff;
        
        for(i = 0; i < gu32RxSize; i++)
            gUsbRxBuf[i] = HSUSBD->EP[EPB].EPDAT_BYTE;//[2024-12-03]byte accessible

        /* Set a flag to indicate bulk out ready */
        //gi8BulkOutReady = 1;
        HSUSBD_ENABLE_EP_INT(EPB, 0);
        HSUSBD_CLR_EP_INT_FLAG(EPB, IrqSt);

        VcomRxhandler((uint8_t*)(gUsbRxBuf));
        
    }

#if 1 //To do:check which EP is used for HID in this composite device
    /* interrupt in */
    if(IrqStL & HSUSBD_GINTSTS_EPDIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPD].EPINTSTS & HSUSBD->EP[EPD].EPINTEN;
        //if(HSUSBD->EP[EPD].EPINTSTS & 0x02)
        //    EPD_Handler();
        HSUSBD_CLR_EP_INT_FLAG(EPD, IrqSt);
				//printf("EPD, INT IN\n");
				
    }
    /* interrupt out */
    if(IrqStL & HSUSBD_GINTSTS_EPEIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPE].EPINTSTS & HSUSBD->EP[EPE].EPINTEN;
        //printf("EPE, INT IN, IrqSr=0x%x\n", IrqSt);
			  #if 0
			  if(HSUSBD->EP[EPE].EPINTSTS & 0x01)
			  #else
			  if(HSUSBD->EP[EPE].EPINTSTS & 0x11)//receive or buffer full
        #endif			
        {
				    EPE_Handler();
				}
        HSUSBD_CLR_EP_INT_FLAG(EPE, IrqSt);
				
				//g_u8GoLDR = 1;
    }
#endif

    if(IrqStL & HSUSBD_GINTSTS_EPCIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPC].EPINTSTS & HSUSBD->EP[EPC].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPC, IrqSt);
    }

#if 0
    if(IrqStL & HSUSBD_GINTSTS_EPDIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPD].EPINTSTS & HSUSBD->EP[EPD].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPD, IrqSt);
    }

    if(IrqStL & HSUSBD_GINTSTS_EPEIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPE].EPINTSTS & HSUSBD->EP[EPE].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPE, IrqSt);
    }
#endif
    if(IrqStL & HSUSBD_GINTSTS_EPFIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPF].EPINTSTS & HSUSBD->EP[EPF].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPF, IrqSt);
    }

    if(IrqStL & HSUSBD_GINTSTS_EPGIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPG].EPINTSTS & HSUSBD->EP[EPG].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPG, IrqSt);
    }

    if(IrqStL & HSUSBD_GINTSTS_EPHIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPH].EPINTSTS & HSUSBD->EP[EPH].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPH, IrqSt);
    }

    if(IrqStL & HSUSBD_GINTSTS_EPIIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPI].EPINTSTS & HSUSBD->EP[EPI].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPI, IrqSt);
    }

    if(IrqStL & HSUSBD_GINTSTS_EPJIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPJ].EPINTSTS & HSUSBD->EP[EPJ].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPJ, IrqSt);
    }

    if(IrqStL & HSUSBD_GINTSTS_EPKIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPK].EPINTSTS & HSUSBD->EP[EPK].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPK, IrqSt);
    }

    if(IrqStL & HSUSBD_GINTSTS_EPLIF_Msk)
    {
        IrqSt = HSUSBD->EP[EPL].EPINTSTS & HSUSBD->EP[EPL].EPINTEN;
        HSUSBD_CLR_EP_INT_FLAG(EPL, IrqSt);
    }
}

extern __attribute__((aligned(4))) uint8_t g_au8ResponseBuff[64];
void EPD_Handler(void)  /* Interrupt IN handler */
{
//    HID_SetInReport();
	  uint32_t i;

    /* Trigger HID IN */
    for(i = 0; i < 64; i++)
    {
        HSUSBD->EP[EPD].EPDAT_BYTE = g_au8ResponseBuff[i];
    }

    HSUSBD->EP[EPD].EPTXCNT = 64;
    HSUSBD_ENABLE_EP_INT(EPD, HSUSBD_EPINTEN_INTKIEN_Msk);
}

void EPE_Handler(void)  /* Interrupt OUT handler */
{
    uint32_t len, i;
    len = HSUSBD->EP[EPE].EPDATCNT & 0xffff;
    //printf("Finish EPE_Handler\n");  
	  for(i = 0; i < len; i++)
	  {
	      g_u8OutBuff[i] = HSUSBD->EP[EPE].EPDAT_BYTE;  
			  //printf("0x%x\n",g_u8OutBuff[i]);
        //HID_GetOutReport(g_u8OutBuff, len);
	  }
		
	  g_u8UsbDataReady = TRUE;
     //[2025-02-21]move here to speed up ISP cmd response      
      HID_RebootCmdhandler();

    
}

/*--------------------------------------------------------------------------*/
/**
  * @brief  USBD Endpoint Config.
  * @param  None.
  * @retval None.
  */
void HID_InitForHighSpeed(void)
{
    /*****************************************************/
    /* EPD ==> Interrupt IN endpoint, address 1 */
    HSUSBD_SetEpBufAddr(EPD, EPD_BUF_BASE, EPD_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPD, EPD_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPD, INT_IN_EP_NUM_HID, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_IN);
    //HSUSBD_ENABLE_EP_INT(EPA, HSUSBD_EPINTEN_TXPKIEN_Msk);
    g_u32EpDMaxPacketSize = EPD_MAX_PKT_SIZE;

    /* EPE ==> Interrupt OUT endpoint, address 2 */
    HSUSBD_SetEpBufAddr(EPE, EPE_BUF_BASE, EPE_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPE, EPE_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPE, INT_OUT_EP_NUM_HID, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_OUT);
    HSUSBD_ENABLE_EP_INT(EPE, HSUSBD_EPINTEN_RXPKIEN_Msk | HSUSBD_EPINTEN_BUFFULLIEN_Msk);
    g_u32EpEMaxPacketSize = EPE_MAX_PKT_SIZE;
}

void HID_InitForFullSpeed(void)
{
    /*****************************************************/
    /* EPA ==> Interrupt IN endpoint, address 1 */
    HSUSBD_SetEpBufAddr(EPA, EPA_BUF_BASE, EPA_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPA, EPA_OTHER_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPA, INT_IN_EP_NUM_HID, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_IN);
    //HSUSBD_ENABLE_EP_INT(EPA, HSUSBD_EPINTEN_TXPKIEN_Msk);
    g_u32EpAMaxPacketSize = EPA_OTHER_MAX_PKT_SIZE;

    /* EPB ==> Interrupt OUT endpoint, address 2 */
    HSUSBD_SetEpBufAddr(EPB, EPB_BUF_BASE, EPB_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPB, EPB_OTHER_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPB, INT_OUT_EP_NUM_HID, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_OUT);
    HSUSBD_ENABLE_EP_INT(EPB, HSUSBD_EPINTEN_RXPKIEN_Msk | HSUSBD_EPINTEN_BUFFULLIEN_Msk);
    g_u32EpBMaxPacketSize = EPB_OTHER_MAX_PKT_SIZE;
}

void VHID_Init(void)
{
    /* Configure USB controller */
    /* Enable USB BUS, CEP and EPA , EPB global interrupt */
    HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk | HSUSBD_GINTEN_CEPIEN_Msk | HSUSBD_GINTEN_EPAIEN_Msk | HSUSBD_GINTEN_EPBIEN_Msk | HSUSBD_GINTEN_EPCIEN_Msk | HSUSBD_GINTEN_EPDIEN_Msk |HSUSBD_GINTEN_EPEIEN_Msk);
    /* Enable BUS interrupt */
    HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_DMADONEIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk | HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_VBUSDETIEN_Msk);
    /* Reset Address to 0 */
    HSUSBD_SET_ADDR(0);

    /*****************************************************/
    /* Control endpoint */
    HSUSBD_SetEpBufAddr(CEP, CEP_BUF_BASE, CEP_BUF_LEN);
    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_STSDONEIEN_Msk);

    HID_InitForHighSpeed();
	  VCOM_InitForHighSpeed();
}

void VCOM_InitForHighSpeed(void)
{
    /*****************************************************/
    /* EPA ==> Bulk IN endpoint, address 1 */
    HSUSBD_SetEpBufAddr(EPA, EPA_BUF_BASE, EPA_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPA, EPA_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPA, BULK_IN_EP_NUM_VCOM, HSUSBD_EP_CFG_TYPE_BULK, HSUSBD_EP_CFG_DIR_IN);

    /* EPB ==> Bulk OUT endpoint, address 2 */
    HSUSBD_SetEpBufAddr(EPB, EPB_BUF_BASE, EPB_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPB, EPB_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPB, BULK_OUT_EP_NUM_VCOM, HSUSBD_EP_CFG_TYPE_BULK, HSUSBD_EP_CFG_DIR_OUT);
    HSUSBD_ENABLE_EP_INT(EPB, HSUSBD_EPINTEN_RXPKIEN_Msk | HSUSBD_EPINTEN_SHORTRXIEN_Msk);

    /* EPC ==> Interrupt IN endpoint, address 3 */
    HSUSBD_SetEpBufAddr(EPC, EPC_BUF_BASE, EPC_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPC, EPC_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPC, INT_IN_EP_NUM_VCOM, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_IN);
}

void VCOM_InitForFullSpeed(void)
{
    /*****************************************************/
    /* EPA ==> Bulk IN endpoint, address 1 */
    HSUSBD_SetEpBufAddr(EPA, EPA_BUF_BASE, EPA_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPA, EPA_OTHER_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPA, BULK_IN_EP_NUM_VCOM, HSUSBD_EP_CFG_TYPE_BULK, HSUSBD_EP_CFG_DIR_IN);

    /* EPB ==> Bulk OUT endpoint, address 2 */
    HSUSBD_SetEpBufAddr(EPB, EPB_BUF_BASE, EPB_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPB, EPB_OTHER_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPB, BULK_OUT_EP_NUM_VCOM, HSUSBD_EP_CFG_TYPE_BULK, HSUSBD_EP_CFG_DIR_OUT);
    HSUSBD_ENABLE_EP_INT(EPB, HSUSBD_EPINTEN_RXPKIEN_Msk | HSUSBD_EPINTEN_SHORTRXIEN_Msk);

    /* EPC ==> Interrupt IN endpoint, address 3 */
    HSUSBD_SetEpBufAddr(EPC, EPC_BUF_BASE, EPC_BUF_LEN);
    HSUSBD_SET_MAX_PAYLOAD(EPC, EPC_OTHER_MAX_PKT_SIZE);
    HSUSBD_ConfigEp(EPC, INT_IN_EP_NUM_VCOM, HSUSBD_EP_CFG_TYPE_INT, HSUSBD_EP_CFG_DIR_IN);
}

/*--------------------------------------------------------------------------*/
/**
  * @brief  USBD Endpoint Config.
  * @param  None.
  * @retval None.
  */
void VCOM_Init(void)
{
    /* Configure USB controller */
    /* Enable USB BUS, CEP and EPA , EPB, EPD, EPE global interrupt */
    HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk | HSUSBD_GINTEN_CEPIEN_Msk | HSUSBD_GINTEN_EPAIEN_Msk | HSUSBD_GINTEN_EPBIEN_Msk | HSUSBD_GINTEN_EPCIEN_Msk);
    /* Enable BUS interrupt */
    HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_DMADONEIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk | HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_VBUSDETIEN_Msk);
    /* Reset Address to 0 */
    HSUSBD_SET_ADDR(0);

    /*****************************************************/
    /* Control endpoint */
    HSUSBD_SetEpBufAddr(CEP, CEP_BUF_BASE, CEP_BUF_LEN);
    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_STSDONEIEN_Msk);

    VCOM_InitForHighSpeed();
}


void VCOM_ClassRequest(void)
{
    if(gUsbCmd.bmRequestType & 0x80)    /* request data transfer direction */
    {
        // Device to host
        switch(gUsbCmd.bRequest)
        {
            
					  case GET_LINE_CODE:
            {
                if((gUsbCmd.wIndex & 0xff) == 0)   /* VCOM-1 */
                    HSUSBD_PrepareCtrlIn((uint8_t *)&gLineCoding, 7);
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_INTKIF_Msk);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_INTKIEN_Msk);
                break;
            }
  		 		  case GET_REPORT://hid
            case GET_IDLE://hid
            case GET_PROTOCOL://hid
            default:
            {
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
                break;
            }
        }
    }
    else
    {
        // Host to device
        switch(gUsbCmd.bRequest)
        {
            case SET_REPORT:
            {
                if(((gUsbCmd.wValue >> 8) & 0xff) == 3)
                {
                    /* Request Type = Feature */
                    HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                    HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
                }
                break;
            }
            case SET_IDLE:
            {
                /* Status stage */
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
                break;
            }
            
					  case SET_CONTROL_LINE_STATE:
            {
                if((gUsbCmd.wIndex & 0xff) == 0)    /* VCOM-1 */
                {
                    gCtrlSignal = gUsbCmd.wValue;
                    //printf("RTS=%d  DTR=%d\n", (gCtrlSignal0 >> 1) & 1, gCtrlSignal0 & 1);
                }
                // DATA IN for end of setup
                /* Status stage */
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
                break;
            }
            case SET_LINE_CODE:
            {
                if((gUsbCmd.wIndex & 0xff) == 0)  /* VCOM-1 */
                    HSUSBD_CtrlOut((uint8_t *)&gLineCoding, 7);

                /* Status stage */
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);

                /* UART setting */
                if((gUsbCmd.wIndex & 0xff) == 0)  /* VCOM-1 */
                    VCOM_LineCoding(0);
                break;
            }
            case SET_PROTOCOL://hid
						default:
            {
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
                break;
            }
        }
    }
}


void VCOM_LineCoding(uint8_t port)
{
    uint32_t u32Reg;
    uint32_t u32Baud_Div;

    if(port == 0)
    {
        NVIC_DisableIRQ(UART0_IRQn);
        // Reset software fifo
        comRbytes = 0;
        comRhead = 0;
        comRtail = 0;

        comTbytes = 0;
        comThead = 0;
        comTtail = 0;

        // Reset hardware fifo
        UART0->FIFO = 0x3;

        // Set baudrate
        u32Baud_Div = UART_BAUD_MODE2_DIVIDER(__HIRC, gLineCoding.u32DTERate);

        if(u32Baud_Div > 0xFFFF)
            UART0->BAUD = (UART_BAUD_MODE0 | UART_BAUD_MODE0_DIVIDER(__HIRC, gLineCoding.u32DTERate));
        else
            UART0->BAUD = (UART_BAUD_MODE2 | u32Baud_Div);

        // Set parity
        if(gLineCoding.u8ParityType == 0)
            u32Reg = 0; // none parity
        else if(gLineCoding.u8ParityType == 1)
            u32Reg = 0x08; // odd parity
        else if(gLineCoding.u8ParityType == 2)
            u32Reg = 0x18; // even parity
        else
            u32Reg = 0;

        // bit width
        switch(gLineCoding.u8DataBits)
        {
            case 5:
                u32Reg |= 0;
                break;
            case 6:
                u32Reg |= 1;
                break;
            case 7:
                u32Reg |= 2;
                break;
            case 8:
                u32Reg |= 3;
                break;
            default:
                break;
        }

        // stop bit
        if(gLineCoding.u8CharFormat > 0)
            u32Reg |= 0x4; // 2 or 1.5 bits

        UART0->LINE = u32Reg;

        // Re-enable UART interrupt
        //NVIC_EnableIRQ(UART0_IRQn);
    }
}

void VcomBegin(uint32_t baud)
{
    S_RTC_TIME_DATA_T sInitTime;
    uint32_t u32SPRData;
    //SYS_UnlockReg();//For ISP, it is unlock status now

#if defined(__M460__)
    uint32_t volatile i;
    /* Select HSUSBD */
    SYS->USBPHY &= ~SYS_USBPHY_HSUSBROLE_Msk;
    /* Enable USB PHY */
    SYS->USBPHY = (SYS->USBPHY & ~(SYS_USBPHY_HSUSBROLE_Msk | SYS_USBPHY_HSUSBACT_Msk)) | SYS_USBPHY_HSUSBEN_Msk;
    for(i = 0; i < 0x1000; i++);   // delay > 10 us
    SYS->USBPHY |= SYS_USBPHY_HSUSBACT_Msk;

    /* Enable HSUSBD module clock */
    CLK_EnableModuleClock(HSUSBD_MODULE);

#endif

    //SYS_LockReg();//For ISP, keep it in unlock status

    HSUSBD_Open(&gsHSInfo, VCOM_ClassRequest, NULL);

    /* Start transaction */
    HSUSBD->OPER = HSUSBD_OPER_HISPDEN_Msk;   /* high-speed */
    HSUSBD_CLR_SE0();

    VHID_Init();
    /* Enable HSUSBD interrupt */
    NVIC_EnableIRQ(USBD20_IRQn);

    /* Start transaction */
    while(1)
    {
        if(HSUSBD_IS_ATTACHED())
        {
            HSUSBD_Start();
            break;
        }
    }

     /* Set Vector Table Offset Register */
    SCB->VTOR = FMC_APROM_BASE;        

    /* FMC Enable */
    CLK->AHBCLK0 |= CLK_AHBCLK0_ISPCKEN_Msk;
		
	/* FMC Open */
    FMC->ISPCTL |= FMC_ISPCTL_ISPEN_Msk;

    /* Open RTC and start counting */
    sInitTime.u32Year       = 2021;
    sInitTime.u32Month      = 10;
    sInitTime.u32Day        = 10;
    sInitTime.u32Hour       = 12;
    sInitTime.u32Minute     = 12;
    sInitTime.u32Second     = 0;
    sInitTime.u32DayOfWeek  = RTC_SUNDAY;
    sInitTime.u32TimeScale  = RTC_CLOCK_24;
    if(RTC_Open(&sInitTime) != 0)
    {
        while(1);
    }
    /* Enable spare register */
    RTC_EnableSpareAccess();  

    /* Check spare register data index 0 */
	u32SPRData = RTC_READ_SPARE_REGISTER(0);
	if(u32SPRData!=0)
	{
	    RTC_WRITE_SPARE_REGISTER(0, 0x00);
	}
	else
    {
	    //Do nothing
	}
   
}

uint32_t VcomGetTxFifoCount(void)
{
    //HSUSBD_EPDATCNT_DATCNT_Msk  is  "0xfffful << 0".
    return (HSUSBD->EP[EPA].EPDATCNT & 0xffff);
}

extern uint8_t volatile g_u8GoLDR;
void HID_RebootCmdhandler(void)
{
    if(g_u8UsbDataReady == TRUE)
    {
        ParseCmd((uint8_t *)g_u8OutBuff, 64);
        /* Response ISP command */
        EPD_Handler();
	    g_u8UsbDataReady = 0;
    }				  
        
	if(g_u8GoLDR)
    {
	    /* Write magic number to spare register */
        RTC_WRITE_SPARE_REGISTER(0, 0x55);
				  
        FMC_SetVectorPageAddr(FMC_LDROM_BASE);
        SYS_LockReg(); 
        /* Software reset to boot to LDROM */
        NVIC_SystemReset();

        /* Trap the CPU, */
        //[2025-02-21]to guarntee ISP cmd response done and trap.      
        while(1);
	}

}
#if 1
#define SERIAL_BUFFER_SIZE 16
uint32_t VcomRxhandler(uint8_t* pch)
{
    int32_t i, i32Len; 
    int k; 
    gu32RxCount += gu32RxSize;
	/* Process the Bulk out data when bulk out data is ready. */
	
    /*To fit serial buffer size: SERIAL_BUFFER_SIZE*/
    if ( gu32RxSize > SERIAL_BUFFER_SIZE)
        gu32RxSize = SERIAL_BUFFER_SIZE;
	
    //Since the USB EP bulk in data are get in batch. Always set tail to 0, 
    //and feed from tail(0) 
    for (k = 0; k < gu32RxSize; k++)
    {
       Serial._rx_buffer->buffer[k] = *pch++;
    }
    Serial._rx_buffer->head = k;
    Serial._rx_buffer->tail = 0;
    
    gu32RxSize = 0;
    HSUSBD_ENABLE_EP_INT(EPB, HSUSBD_EPINTEN_RXPKIEN_Msk | HSUSBD_EPINTEN_SHORTRXIEN_Msk);
    
}
#endif
#if  1
void HID_Init(void)
{
    //HSUSBD->OPER = 0;
    /* Configure USB controller */
    /* Enable USB BUS, CEP and EPA global interrupt */
    HSUSBD_ENABLE_USB_INT(HSUSBD_GINTEN_USBIEN_Msk | HSUSBD_GINTEN_CEPIEN_Msk | HSUSBD_GINTEN_EPAIEN_Msk | HSUSBD_GINTEN_EPBIEN_Msk);
    /* Enable BUS interrupt */
    HSUSBD_ENABLE_BUS_INT(HSUSBD_BUSINTEN_DMADONEIEN_Msk | HSUSBD_BUSINTEN_RESUMEIEN_Msk | HSUSBD_BUSINTEN_RSTIEN_Msk | HSUSBD_BUSINTEN_VBUSDETIEN_Msk);
    /* Reset Address to 0 */
    HSUSBD_SET_ADDR(0);

    /*****************************************************/
    /* Control endpoint */
    HSUSBD_SetEpBufAddr(CEP, CEP_BUF_BASE, CEP_BUF_LEN);
    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_SETUPPKIEN_Msk | HSUSBD_CEPINTEN_STSDONEIEN_Msk);

    HID_InitForHighSpeed();
}

void HID_ClassRequest(void)
{
    if(gUsbCmd.bmRequestType & 0x80)    /* request data transfer direction */
    {
        // Device to host
        switch(gUsbCmd.bRequest)
        {
            case GET_REPORT:
//             {
//                 break;
//             }
            case GET_IDLE:
//             {
//                 break;
//             }
            case GET_PROTOCOL:
//            {
//                break;
//            }
            default:
            {
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
                break;
            }
        }
    }
    else
    {
        // Host to device
        switch(gUsbCmd.bRequest)
        {
            case SET_REPORT:
            {
                if(((gUsbCmd.wValue >> 8) & 0xff) == 3)
                {
                    /* Request Type = Feature */
                    HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                    HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                    HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
                }
                break;
            }
            case SET_IDLE:
            {
                /* Status stage */
                HSUSBD_CLR_CEP_INT_FLAG(HSUSBD_CEPINTSTS_STSDONEIF_Msk);
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_NAKCLR);
                HSUSBD_ENABLE_CEP_INT(HSUSBD_CEPINTEN_STSDONEIEN_Msk);
                break;
            }
            case SET_PROTOCOL:
//             {
//                 break;
//             }
            default:
            {
                // Stall
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
                break;
            }
        }
    }
}

void HID_VendorRequest(void)
{
    if(gUsbCmd.bmRequestType & 0x80)    /* request data transfer direction */
    {
        // Device to host
        switch(gUsbCmd.bRequest)
        {
            default:
            {
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
                break;
            }
        }
    }
    else
    {
        // Host to device
        switch(gUsbCmd.bRequest)
        {
            default:
            {
                // Stall
                /* Setup error, stall the device */
                HSUSBD_SET_CEP_STATE(HSUSBD_CEPCTL_STALLEN_Msk);
                break;
            }
        }
    }
}


int32_t HID_CmdEraseSectors(CMD_T *pCmd)
{
    uint32_t u32StartSector;
    uint32_t u32Sectors;
    uint32_t i;

    u32StartSector = pCmd->u32Arg1;
    u32Sectors = pCmd->u32Arg2;

    //printf("Erase command - Sector: %d   Sector Cnt: %d\n", u32StartSector, u32Sectors);

    for(i = 0; i < u32Sectors; i++)
    {
        //printf("Erase sector - %d\n",u32StartSector + i);
        /* TODO: To erase the sector of storage */

    }
    /* To note the command has been done */
    pCmd->u8Cmd = HID_CMD_NONE;

    return 0;
}


int32_t HID_CmdReadPages(CMD_T *pCmd)
{
    uint32_t u32StartPage;
    uint32_t u32Pages;
    uint32_t len;
    int32_t i;

    u32StartPage = pCmd->u32Arg1;
    u32Pages     = pCmd->u32Arg2;

    //printf("Read command - Start page: %d    Pages Numbers: %d\n", u32StartPage, u32Pages);

    if(u32Pages)
    {
        /* Update data to page buffer to upload */
        /* TODO: We need to update the page data if got a page read command. (0xFF is used in this sample code) */
        for(i = 0; i < PAGE_SIZE; i++)
            g_u8PageBuff[i] = 0xFF;
        g_u32BytesInPageBuf = PAGE_SIZE;

        /* The signature word is used as page counter */
        pCmd->u32Signature = 1;

        /* Trigger HID IN */
        len = Minimum(g_u32EpDMaxPacketSize, g_u32BytesInPageBuf);
        for(i = 0; i < len; i++)
            HSUSBD->EP[EPD].EPDAT_BYTE = g_u8PageBuff[i];
        HSUSBD->EP[EPD].EPTXCNT = len;
        HSUSBD_ENABLE_EP_INT(EPD, HSUSBD_EPINTEN_INTKIEN_Msk);
        g_u32BytesInPageBuf -= len;
    }

    return 0;
}


int32_t HID_CmdWritePages(CMD_T *pCmd)
{
    uint32_t u32StartPage;
    uint32_t u32Pages;

    u32StartPage = pCmd->u32Arg1;
    u32Pages     = pCmd->u32Arg2;

    //printf("Write command - Start page: %d    Pages Numbers: %d\n", u32StartPage, u32Pages);
    g_u32BytesInPageBuf = 0;

    /* The signature is used to page counter */
    pCmd->u32Signature = 0;
    return 0;
}


int32_t gi32CmdTestCnt = 0;
int32_t HID_CmdTest(CMD_T *pCmd)
{
    int32_t i;
    uint8_t *pu8;

    pu8 = (uint8_t *)pCmd;
    //printf("Get test command #%d (%d bytes)\n", gi32CmdTestCnt++, pCmd->u8Size);
    for(i = 0; i < pCmd->u8Size; i++)
    {
        if((i & 0xF) == 0)
        {
            //printf("\n");
        }
        //printf(" %02x", pu8[i]);
    }

    //printf("\n");


    /* To note the command has been done */
    pCmd->u8Cmd = HID_CMD_NONE;

    return 0;
}


uint32_t CalCheckSum(uint8_t *buf, uint32_t size)
{
    uint32_t sum;
    int32_t i;

    i = 0;
    sum = 0;
    while(size--)
    {
        sum += buf[i++];
    }

    return sum;

}


int32_t ProcessCommand(uint8_t *pu8Buffer, uint32_t u32BufferLen)
{
    uint32_t u32sum;


    HSUSBD_MemCopy((uint8_t *)&gCmd, pu8Buffer, u32BufferLen);

    /* Check size */
    if((gCmd.u8Size > sizeof(gCmd)) || (gCmd.u8Size > u32BufferLen))
        return -1;

    /* Check signature */
    if(gCmd.u32Signature != HID_CMD_SIGNATURE)
        return -1;

    /* Calculate checksum & check it*/
    u32sum = CalCheckSum((uint8_t *)&gCmd, gCmd.u8Size);
    if(u32sum != gCmd.u32Checksum)
        return -1;

    switch(gCmd.u8Cmd)
    {
        case HID_CMD_ERASE:
        {
            HID_CmdEraseSectors(&gCmd);
            break;
        }
        case HID_CMD_READ:
        {
            HID_CmdReadPages(&gCmd);
            break;
        }
        case HID_CMD_WRITE:
        {
            HID_CmdWritePages(&gCmd);
            break;
        }
        case HID_CMD_TEST:
        {
            HID_CmdTest(&gCmd);
            break;
        }
        default:
            return -1;
    }

    return 0;
}

void HID_GetOutReport(uint8_t *pu8EpBuf, uint32_t u32Size)
{
    uint32_t volatile i;
    uint8_t  u8Cmd;
    uint32_t u32StartPage;
    uint32_t u32Pages;
    uint32_t u32PageCnt;

    /* Get command information */
    u8Cmd        = gCmd.u8Cmd;
    u32StartPage = gCmd.u32Arg1;
    u32Pages     = gCmd.u32Arg2;
    u32PageCnt   = gCmd.u32Signature; /* The signature word is used to count pages */

    /* Check if it is in the data phase of write command */
    if((u8Cmd == HID_CMD_WRITE) && (u32PageCnt < u32Pages))
    {
        /* Process the data phase of write command */

        /* Get data from HID OUT */
        HSUSBD_MemCopy(&g_u8PageBuff[g_u32BytesInPageBuf], pu8EpBuf, u32Size);
        g_u32BytesInPageBuf += u32Size;

        /* The HOST must make sure the data is PAGE_SIZE alignment */
        if(g_u32BytesInPageBuf >= PAGE_SIZE)
        {
            //printf("Writing page %d\n", u32StartPage + u32PageCnt);
            /* TODO: We should program received data to storage here */
            u32PageCnt++;

            /* Write command complete! */
            if(u32PageCnt >= u32Pages)
            {
                u8Cmd = HID_CMD_NONE;

                //printf("Write command complete.\n");
            }

            g_u32BytesInPageBuf = 0;

        }

        /* Update command status */
        gCmd.u8Cmd        = u8Cmd;
        gCmd.u32Signature = u32PageCnt;
    }
    else
    {
        /* Check and process the command packet */
        if(ProcessCommand(pu8EpBuf, u32Size))
        {
            //printf("Unknown HID command!\n");
        }
    }
}


//[2024-12-19]Add from HSUSB HID transfer Example
void HID_SetInReport(void)
{
    uint32_t u32StartPage;
    uint32_t u32TotalPages;
    uint32_t u32PageCnt;
    uint32_t len;
    int32_t i;
    uint8_t u8Cmd;

    u8Cmd        = gCmd.u8Cmd;
    u32StartPage = gCmd.u32Arg1;
    u32TotalPages = gCmd.u32Arg2;
    u32PageCnt   = gCmd.u32Signature;

    /* Check if it is in data phase of read command */
    if(u8Cmd == HID_CMD_READ)
    {
        /* Process the data phase of read command */
        if((u32PageCnt >= u32TotalPages) && (g_u32BytesInPageBuf == 0))
        {
            /* The data transfer is complete. */
            u8Cmd = HID_CMD_NONE;
            //printf("Read command complete!\n");
        }
        else
        {
            if(g_u32BytesInPageBuf == 0)
            {
                /* The previous page has sent out. Read new page to page buffer */
                /* TODO: We should update new page data here. (0xFF is used in this sample code) */
                //printf("Reading page %d\n", u32StartPage + u32PageCnt);
                for(i = 0; i < PAGE_SIZE; i++)
                    g_u8PageBuff[i] = 0xFF;

                g_u32BytesInPageBuf = PAGE_SIZE;

                /* Update the page counter */
                u32PageCnt++;
            }

            /* Prepare the data for next HID IN transfer */
            len = Minimum(g_u32EpDMaxPacketSize, g_u32BytesInPageBuf);
            for(i = 0; i < len; i++)
                HSUSBD->EP[EPD].EPDAT_BYTE = g_u8PageBuff[PAGE_SIZE - g_u32BytesInPageBuf];
            HSUSBD->EP[EPD].EPTXCNT = len;
            HSUSBD_ENABLE_EP_INT(EPD, HSUSBD_EPINTEN_INTKIEN_Msk);
            g_u32BytesInPageBuf -= len;
        }
    }

    gCmd.u8Cmd        = u8Cmd;
    gCmd.u32Signature = u32PageCnt;

}


#endif
#ifdef __cplusplus
}
#endif
#endif//#if defined(__M460MINIMA__)