/***************************************************************************//**
 * @file     vcom_serial.h
 * @version  V3.00
 * @brief    HSUSBD virtual COM header file.
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#ifndef __VCOM_SERIAL_H__
#define __VCOM_SERIAL_H__
#if defined(__M460MINIMA__)
//#include "HardwareSerial.h"
#ifdef __cplusplus
extern "C" {
#endif
/* Define the vendor id and product id */
#define USBD_VID        0x0416
#define USBD_PID        0x3F00


//=============================================
/*!<USB HID Interface Class protocol */
#define HID_NONE                0x00
#define HID_KEYBOARD            0x01
#define HID_MOUSE               0x02

/*!<USB HID Class Report Type */
#define HID_RPT_TYPE_INPUT      0x01
#define HID_RPT_TYPE_OUTPUT     0x02
#define HID_RPT_TYPE_FEATURE    0x03

/*!<USB HID Protocol Type */
#define HID_BOOT_PROTOCOL       0x00
#define HID_REPORT_PROTOCOL     0x01

/*!<USB HID Keyboard Output Report */
#define HID_LED_NumLock         0x01
#define HID_LED_CapsLock        0x02
#define HID_LED_ScrollLock      0x04
#define HID_LED_Compose         0x08
#define HID_LED_Kana            0x10
#define HID_LED_Constatnt       0xE0
#define HID_LED_ALL             0xFF

/*!<Define HID Class Specific Request */
#define GET_REPORT              0x01
#define GET_IDLE                0x02
#define GET_PROTOCOL            0x03
#define SET_REPORT              0x09
#define SET_IDLE                0x0A
#define SET_PROTOCOL            0x0B



/*!<Define CDC Class Specific Request */
#define SET_LINE_CODE           0x20
#define GET_LINE_CODE           0x21
#define SET_CONTROL_LINE_STATE  0x22

/* Define DMA Maximum Transfer length */
#define USBD_MAX_DMA_LEN    0x1000
/*-------------------------------------------------------------*/
/* Define EP maximum packet size */
//EP for VCOM
#define CEP_MAX_PKT_SIZE        64
#define CEP_OTHER_MAX_PKT_SIZE  64
#define EPA_MAX_PKT_SIZE        512
#define EPA_OTHER_MAX_PKT_SIZE  64
#define EPB_MAX_PKT_SIZE        512
#define EPB_OTHER_MAX_PKT_SIZE  64
#define EPC_MAX_PKT_SIZE        64
#define EPC_OTHER_MAX_PKT_SIZE  64
//EP for HID
#define EPD_MAX_PKT_SIZE        1024
#define EPD_OTHER_MAX_PKT_SIZE  64
#define EPE_MAX_PKT_SIZE        1024
#define EPE_OTHER_MAX_PKT_SIZE  64


#define CEP_BUF_BASE            0
#define CEP_BUF_LEN             CEP_MAX_PKT_SIZE
#define EPA_BUF_BASE            0x200//0x200 offset
#define EPA_BUF_LEN             EPA_MAX_PKT_SIZE
#define EPB_BUF_BASE            0x400//0x200 offset
#define EPB_BUF_LEN             EPB_MAX_PKT_SIZE
#define EPC_BUF_BASE            0x600//0x200 offset
#define EPC_BUF_LEN             EPC_MAX_PKT_SIZE
#define EPD_BUF_BASE            0x800//0x400 offset
#define EPD_BUF_LEN             EPD_MAX_PKT_SIZE
#define EPE_BUF_BASE            0xC00//0x400 offset
#define EPE_BUF_LEN             EPE_MAX_PKT_SIZE


/* Define the interrupt In EP number */
#define BULK_IN_EP_NUM_VCOM      0x01
#define BULK_OUT_EP_NUM_VCOM     0x02
#define INT_IN_EP_NUM_VCOM       0x03
#define INT_IN_EP_NUM_HID        0x04
#define INT_OUT_EP_NUM_HID       0x05


/* Define Descriptor information */
#define HID_DEFAULT_INT_IN_INTERVAL     1
#define USBD_SELF_POWERED               0
#define USBD_REMOTE_WAKEUP              0
#define USBD_MAX_POWER                  50  /* The unit is in 2mA. ex: 50 * 2mA = 100mA */



//EP for Keyboard
#define EPF_MAX_PKT_SIZE        64
#define EPF_OTHER_MAX_PKT_SIZE  64

#define EPF_BUF_BASE    0x700//(CEP_BUF_BASE + CEP_BUF_LEN)
#define EPF_BUF_LEN     EPF_MAX_PKT_SIZE

//EP for Mouse
#define EPG_MAX_PKT_SIZE        64
#define EPG_OTHER_MAX_PKT_SIZE  64

#define EPG_BUF_BASE    0x780//(CEP_BUF_BASE + CEP_BUF_LEN)
#define EPG_BUF_LEN     EPG_MAX_PKT_SIZE

/* Define the interrupt In EP number */
#define INT_IN_EP_NUM_KB    0x06
#define INT_IN_EP_NUM_MOUSE 0x07

/* Define Descriptor information */
//#define HID_DEFAULT_INT_IN_INTERVAL     8   /* (2^8-1) */
#define HID_DEFAULT_INT_IN_INTERVAL_KB  16  /* (2^16-1) */

#define LEN_IAD                           8
#define LEN_VCOM_SPECIFIED_ITERFACE      19
#define LEN_CONFIG_AND_SUBORDINATE      (LEN_CONFIG + 5*LEN_INTERFACE + 3*LEN_HID + 7*LEN_ENDPOINT + 1*LEN_VCOM_SPECIFIED_ITERFACE + 1*LEN_IAD)
//=============================================





#define VHID

/************************************************/
/* for CDC class */
/* Line coding structure
  0-3 dwDTERate    Data terminal rate (baudrate), in bits per second
  4   bCharFormat  Stop bits: 0 - 1 Stop bit, 1 - 1.5 Stop bits, 2 - 2 Stop bits
  5   bParityType  Parity:    0 - None, 1 - Odd, 2 - Even, 3 - Mark, 4 - Space
  6   bDataBits    Data bits: 5, 6, 7, 8, 16  */

typedef struct
{
    uint32_t  u32DTERate;     /* Baud rate    */
    uint8_t   u8CharFormat;   /* stop bit     */
    uint8_t   u8ParityType;   /* parity       */
    uint8_t   u8DataBits;     /* data bits    */
} STR_VCOM_LINE_CODING;

/*-------------------------------------------------------------*/
extern volatile int8_t gi8BulkOutReady;
extern STR_VCOM_LINE_CODING gLineCoding;
extern uint16_t gCtrlSignal;
extern volatile uint16_t comRbytes;
extern volatile uint16_t comRhead;
extern volatile uint16_t comRtail;
extern volatile uint16_t comTbytes;
extern volatile uint16_t comThead;
extern volatile uint16_t comTtail;
extern uint32_t gu32RxSize;
extern uint32_t gu32RxCount;
extern uint32_t gu32TxSize;
extern uint8_t gUsbRxBuf[];

/*-------------------------------------------------------------*/
void VCOM_Init(void);
void VCOM_InitForHighSpeed(void);
void VCOM_InitForFullSpeed(void);
void VCOM_ClassRequest(void);

void HIDKeyboard_InitForHighSpeed(void);
void HIDMouse_InitForHighSpeed(void);

void EPA_Handler(void);
void EPB_Handler(void);
void EPD_Handler(void);
void EPE_Handler(void);
void EPF_Handler(void);
void EPG_Handler(void);

void VCOM_LineCoding(uint8_t port);
void VCOM_TransferData(void);
void VcomBegin(uint32_t baud);
uint32_t VcomGetTxFifoCount(void);
uint32_t VcomRxhandler(uint8_t* pch);

void HID_InitForHighSpeed(void);
void HID_InitForFullSpeed(void);
void HID_Init(void);
void HID_ClassRequest(void);

void HID_SetInReport(void);
void HID_GetOutReport(uint8_t *pu8EpBuf, uint32_t u32Size);
void HID_RebootCmdhandler(void);

void VHID_Init(void);
void HID_UpdateKeyboardData(void);
void HID_UpdateMouseData(void);
void NVT_HID_SendReport(uint8_t id, const void* data, uint32_t len);
   
#ifdef __cplusplus
}
#endif
#endif//defined(__M460MINIMA__)

#endif  /* __VCOM_SERIAL_H__ */
