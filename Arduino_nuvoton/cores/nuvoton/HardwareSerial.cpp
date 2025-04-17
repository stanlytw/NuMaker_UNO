/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file HardwareSerial.cpp
 * @brief Wirish serial port implementation.
 */

#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#if defined(__M451__)
#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer = { { 0 }, 0, 0};
HardwareSerial Serial(UART_Desc[0].U, 0, CLK_CLKSEL1_UARTSEL_HXT, 1, UART_Desc[0].irq, &rx_buffer);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer1 = { { 0 }, 0, 0};
HardwareSerial Serial1(UART_Desc[1].U, 1, CLK_CLKSEL1_UARTSEL_HXT, 1, UART_Desc[1].irq, &rx_buffer1);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>0)
void UART0_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->DAT;
            rx_buffer.head = i;
        }
    }

}
#endif

#if(UART_MAX_COUNT>1)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->DAT;
            rx_buffer1.head = i;
        }
    }

}
#endif

#ifdef __cplusplus
}
#endif

#elif defined(__NUC240__)
#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer = { { 0 }, 0, 0};
HardwareSerial Serial(UART_Desc[0].U, 0, CLK_CLKSEL1_UART_S_HXT, 1, UART_Desc[0].irq, &rx_buffer);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer1 = { { 0 }, 0, 0};
HardwareSerial Serial1(UART_Desc[1].U, 1, CLK_CLKSEL1_UART_S_HXT, 1, UART_Desc[1].irq, &rx_buffer1);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>0)
void UART02_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_IER_RDA_IEN_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->RBR;
            rx_buffer.head = i;
        }
    }

}
#endif

#if(UART_MAX_COUNT>1)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_IER_RDA_IEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->RBR;
            rx_buffer1.head = i;
        }
    }

}
#endif

#ifdef __cplusplus
}
#endif
#elif defined(__M252__) || defined(__M480__) || defined(__M460__)
#if defined(__M460MINIMA__)
    #define RXBUFSIZE           512 /* RX buffer size */
    #define TXBUFSIZE           512 /* TX buffer size */
    uint16_t gCtrlSignal = 0;     /* BIT0: DTR(Data Terminal Ready) , BIT1: RTS(Request To Send) */
    STR_VCOM_LINE_CODING gLineCoding = {115200, 0, 0, 8};   /* Baud rate : 115200    */
    volatile uint16_t comRbytes = 0;
    volatile uint16_t comRhead = 0;
    volatile uint16_t comRtail = 0;
    volatile uint16_t comTbytes = 0;
    volatile uint16_t comThead = 0;
    volatile uint16_t comTtail = 0;
    uint32_t gu32RxSize = 0;
    uint32_t gu32RxCount = 0;
    uint32_t gu32TxSize = 0;
    volatile uint8_t comRbuf[RXBUFSIZE] __attribute__((aligned(4)));
    volatile uint8_t comTbuf[TXBUFSIZE]__attribute__((aligned(4)));
    uint8_t gRxBuf[RXBUFSIZE] __attribute__((aligned(4))) = {0};
    uint8_t gUsbRxBuf[RXBUFSIZE] __attribute__((aligned(4))) = {0};
    volatile int8_t gi8BulkOutReady = 0;
#endif
//For M460Minima board, map Serial1 to VCOM
#if defined(__M460MINIMA__)//VCOM, UART0, UART1
    #if(UART_MAX_COUNT>0)
    ring_buffer rx_buffer= { { 0 }, 0, 0};
    HardwareSerial Serial(HSUSBD, &rx_buffer);//VCOM
    #endif
	#if(UART_MAX_COUNT>1)
    ring_buffer rx_buffer1 = { { 0 }, 0, 0};
    HardwareSerial Serial1(UART_Desc[0].U, 0, UART_Desc[0].clksrcsel, 1, UART_Desc[0].irq, &rx_buffer1);//UART0
    #endif
	#if(UART_MAX_COUNT>2)
    ring_buffer rx_buffer2 = { { 0 }, 0, 0};
    HardwareSerial Serial2(UART_Desc[1].U, 1, UART_Desc[1].clksrcsel, 1, UART_Desc[1].irq, &rx_buffer2);//UART1
    #endif
#else//UART0, UART1, UART4(dummy)
    #if(UART_MAX_COUNT>0)
    ring_buffer rx_buffer = { { 0 }, 0, 0};
    HardwareSerial Serial(UART_Desc[0].U, 0, UART_Desc[0].clksrcsel, 1, UART_Desc[0].irq, &rx_buffer);//UART0
    #endif
	#if(UART_MAX_COUNT>1)
    ring_buffer rx_buffer1 = { { 0 }, 0, 0};
    HardwareSerial Serial1(UART_Desc[1].U, 1, UART_Desc[1].clksrcsel, 1, UART_Desc[1].irq, &rx_buffer1);//UART1
    #endif
	#if(UART_MAX_COUNT>2)
    ring_buffer rx_buffer2 = { { 0 }, 0, 0};
    HardwareSerial Serial2(UART_Desc[2].U, 4, UART_Desc[2].clksrcsel, 1, UART_Desc[2].irq, &rx_buffer2);//UART4,dummy
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if defined(__M460MINIMA__)//VCOM, UART0, UART1
#if(UART_MAX_COUNT>1)
void UART0_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART0->DAT;
            rx_buffer1.head = i;
        }
    }

}
#endif  

#if(UART_MAX_COUNT>2)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer2.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer2.tail)
        {
            rx_buffer2.buffer[rx_buffer2.head] = UART1->DAT;
            rx_buffer2.head = i;
        }
    }

}

#endif

 
#else//defined(__M460MINIMA__), UART0, UART1, UART4(dummy)
#if(UART_MAX_COUNT>0)
void UART0_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->DAT;
            rx_buffer.head = i;
        }
    }

}
#endif

#if(UART_MAX_COUNT>1)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->DAT;
            rx_buffer1.head = i;
        }
    }

}
#endif

#if(UART_MAX_COUNT>2)
/*UART4 is not work on M467SJ, dummy so far.*/
void UART4_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART4, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer2.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer2.tail)
        {
            rx_buffer2.buffer[rx_buffer2.head] = UART4->DAT;
            rx_buffer2.head = i;
        }
    }

}
#endif

#endif










#ifdef __cplusplus
}
#endif

#elif defined(__NANO100__)
#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer1 = { { 0 }, 0, 0};  /* for UAR1_IRQ */
HardwareSerial Serial(UART_Desc[0].U, 0, CLK_CLKSEL1_UART_S_HXT, 1, UART_Desc[0].irq, &rx_buffer1);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer = { { 0 }, 0, 0}; /* for UAR0_IRQ */
HardwareSerial Serial1(UART_Desc[1].U, 1, CLK_CLKSEL1_UART_S_HXT, 1, UART_Desc[1].irq, &rx_buffer);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>1)
void UART0_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_IER_RDA_IE_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->RBR;
            rx_buffer.head = i;
        }
    }

}
#endif

#if(UART_MAX_COUNT>0)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_IER_RDA_IE_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->RBR;
            rx_buffer1.head = i;
        }
    }

}
#endif

#ifdef __cplusplus
}
#endif

#elif defined(__NUC131__)

#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer = { { 0 }, 0, 0};
HardwareSerial Serial(UART_Desc[0].U, 0, CLK_CLKSEL1_UART_S_PLL, 1, UART_Desc[0].irq, &rx_buffer);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer1 = { { 0 }, 0, 0};
HardwareSerial Serial1(UART_Desc[1].U, 1, CLK_CLKSEL1_UART_S_PLL, 1, UART_Desc[1].irq, &rx_buffer1);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>0)
void UART02_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_IER_RDA_IEN_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->RBR;
            rx_buffer.head = i;
        }
    }

}
#endif

#if(UART_MAX_COUNT>1)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_IER_RDA_IEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->RBR;
            rx_buffer1.head = i;
        }
    }

}
#endif

#ifdef __cplusplus
}
#endif
#elif defined(__M032BT__)

#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer = { { 0 }, 0, 0};
HardwareSerial Serial(UART_Desc[0].U, 0, CLK_CLKSEL1_UART0SEL_HIRC, 1, UART_Desc[0].irq, &rx_buffer);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer1 = { { 0 }, 0, 0};
HardwareSerial Serial1(UART_Desc[1].U, 1, CLK_CLKSEL1_UART_S_HXT, 1, UART_Desc[1].irq, &rx_buffer1);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>0)
void UART02_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->DAT;
            rx_buffer.head = i;
        }
    }
}
#endif

#if(UART_MAX_COUNT>1)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_IER_RDA_IEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->RBR;
            rx_buffer1.head = i;
        }
    }

}
#endif

#ifdef __cplusplus
}
#endif

#elif defined(__M032KG__)
#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer1 = { { 0 }, 0, 0};
HardwareSerial Serial(UART_Desc[0].U, 0, CLK_CLKSEL1_UART1SEL_HIRC, 1, UART_Desc[0].irq, &rx_buffer1);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer = { { 0 }, 0, 0};
HardwareSerial Serial1(UART_Desc[1].U, 1, CLK_CLKSEL1_UART0SEL_HIRC, 1, UART_Desc[1].irq, &rx_buffer);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>0)
void UART13_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->DAT;
            rx_buffer1.head = i;
        }
    }

}
#endif


#if(UART_MAX_COUNT>1)
void UART02_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_INTEN_RDAIEN_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->DAT;
            rx_buffer.head = i;
        }
    }
}
#endif






#ifdef __cplusplus
}
#endif

#elif defined(__NANO1X2__)

#if(UART_MAX_COUNT>0)
ring_buffer rx_buffer = { { 0 }, 0, 0};  /* for UART0_IRQ */
HardwareSerial Serial(UART_Desc[0].U, 0, CLK_CLKSEL1_UART_S_HIRC, 1, UART_Desc[0].irq, &rx_buffer);
#endif

#if(UART_MAX_COUNT>1)
ring_buffer rx_buffer1 = { { 0 }, 0, 0}; /* for UART1_IRQ */
HardwareSerial Serial1(UART_Desc[1].U, 1, CLK_CLKSEL1_UART_S_HIRC, 1, UART_Desc[1].irq, &rx_buffer1);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if(UART_MAX_COUNT>0)
void UART0_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART0, UART_IER_RDA_IE_Msk))
    {
        int i = (unsigned int)(rx_buffer.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer.tail)
        {
            rx_buffer.buffer[rx_buffer.head] = UART0->RBR;
            rx_buffer.head = i;
        }
    }

}
#endif

#if(UART_MAX_COUNT>1)
void UART1_IRQHandler(void)
{
    while (UART_GET_INT_FLAG(UART1, UART_IER_RDA_IE_Msk))
    {
        int i = (unsigned int)(rx_buffer1.head + 1) % SERIAL_BUFFER_SIZE;
        if (i != rx_buffer1.tail)
        {
            rx_buffer1.buffer[rx_buffer1.head] = UART1->RBR;
            rx_buffer1.head = i;
        }
    }

}
#endif

#ifdef __cplusplus
}
#endif

#endif

#if(UART_MAX_COUNT>0)
    void serialEvent() __attribute__((weak));
#endif

#if(UART_MAX_COUNT>1)
    void serial1Event() __attribute__((weak));
#endif

void serialEventRun(void)
{
#if(UART_MAX_COUNT>0)
    if (Serial.available()) serialEvent();
#endif

#if(UART_MAX_COUNT>1)
    if (Serial1.available()) serial1Event();
#endif
}

HardwareSerial::HardwareSerial(UART_T *uart_device,
                               uint32_t u32Idx,
                               uint32_t u32ClkSrc,
                               uint32_t u32ClkDiv,
                               IRQn_Type u32IrqId,
                               ring_buffer *rx_buffer)
{
    this->use_vcom = 0;
    this->uart_device = uart_device;
    this->u32Idx = u32Idx;
    this->u32ClkSrc = u32ClkSrc;
    this->u32ClkDiv = u32ClkDiv;
    this->u32IrqId = u32IrqId;
    this->_rx_buffer = rx_buffer;
    
}

#if defined(__M460MINIMA__)
HardwareSerial::HardwareSerial(VCOM_T *vcom_device,ring_buffer *rx_buffer)
{
    this->use_vcom = 1;
    this->vcom_device = vcom_device;
    this->_rx_buffer = rx_buffer;
    this->vcom_init_done = 0;
}
#endif
/*
 * Set up/tear down
 */
void HardwareSerial::begin(uint32_t baud)
{
#if defined(__M460MINIMA__)
    if( use_vcom == 1)//Vcom object
    {
        if( vcom_init_done == 0 ) 
        {
            VcomBegin(baud);
            vcom_init_done = 1;//not use vcom
        }
    }
	else//UART object
	{
		UART_Config(UART_Desc[u32Idx]);
        SYS_UnlockReg();
		 /* Enable IP clock */
        CLK_EnableModuleClock(UART_Desc[u32Idx].module);

        /* Select IP clock source and clock divider */
        if(uart_device == UART0)
            CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART0(u32ClkDiv));
        else if(uart_device == UART1)
            CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART1(u32ClkDiv));
	    else if(uart_device == UART3)
            CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV4_UART3(u32ClkDiv));
	    else if(uart_device == UART4)
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV4_UART4(u32ClkDiv));
        
		/* Reset IP */
        //SYS_ResetModule(UART_Desc[u32Idx].module);

        /* Enable Interrupt */
        UART_ENABLE_INT(uart_device, UART_INTEN_RDAIEN_Msk);
        NVIC_EnableIRQ(u32IrqId);

        /* Configure UART and set UART Baudrate */
        UART_Open(uart_device, baud);
	}
	
#else//NOT  defined(__M460MINIMA__)
    UART_Config(UART_Desc[u32Idx]);
    SYS_UnlockReg();
#if defined(__M451__)
    /* Enable IP clock */
    CLK_EnableModuleClock(UART_Desc[u32Idx].module);

    /* Select IP clock source and clock divider */
    CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART(u32ClkDiv));

    /* Reset IP */
    //SYS_ResetModule(UART_Desc[u32Idx].module);

    /* Enable Interrupt */
    UART_ENABLE_INT(uart_device, UART_INTEN_RDAIEN_Msk);
    NVIC_EnableIRQ(u32IrqId);

    /* Configure UART and set UART Baudrate */
    UART_Open(uart_device, baud);

#elif defined(__M252__) || defined(__M480__) || defined(__M460__)

	/* Enable IP clock */
    CLK_EnableModuleClock(UART_Desc[u32Idx].module);

    /* Select IP clock source and clock divider */
    if(uart_device == UART0)
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART0(u32ClkDiv));
    else if(uart_device == UART1)
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART1(u32ClkDiv));
	else if(uart_device == UART3)
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV4_UART3(u32ClkDiv));
	else if(uart_device == UART4)
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV4_UART4(u32ClkDiv));
        
	/* Reset IP */
    //SYS_ResetModule(UART_Desc[u32Idx].module);

    /* Enable Interrupt */
    UART_ENABLE_INT(uart_device, UART_INTEN_RDAIEN_Msk);
    NVIC_EnableIRQ(u32IrqId);

    /* Configure UART and set UART Baudrate */
    UART_Open(uart_device, baud);
		
#elif defined(__M032BT__)|| defined(__M032KG__)
    /* Enable IP clock */
    CLK_EnableModuleClock(UART_Desc[u32Idx].module);

    /* Select IP clock source and clock divider */
    if(uart_device == UART0)
    {
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART0(u32ClkDiv));
        /* Reset IP */
        //SYS_ResetModule(UART0_RST);
    }
    else if(uart_device == UART1)
    {
        CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV0_UART1(u32ClkDiv));
        /* Reset IP */
        //SYS_ResetModule(UART1_RST);
    }
        
    

    /* Enable Interrupt */
    UART_ENABLE_INT(uart_device, UART_INTEN_RDAIEN_Msk);
    NVIC_EnableIRQ(u32IrqId);

    /* Configure UART and set UART Baudrate */
    UART_Open(uart_device, baud);
#elif defined(__NUC240__) || defined(__NUC131__)
    /* Enable IP clock */
    CLK_EnableModuleClock(UART_Desc[u32Idx].module);

    /* Select IP clock source and clock divider */
    CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_CLKDIV_UART(u32ClkDiv));

    /* Reset IP */
    //SYS_ResetModule(UART_Desc[u32Idx].module);

    /* Enable Interrupt */
    UART_ENABLE_INT(uart_device, UART_IER_RDA_IEN_Msk);
    NVIC_EnableIRQ(u32IrqId);

    /* Configure UART and set UART Baudrate */
    UART_Open(uart_device, baud);
#elif defined(__NANO100__) || defined(__NANO1X2__)

    CLK_EnableModuleClock(UART_Desc[u32Idx].module);

    /* Select IP clock source and clock divider */
    CLK_SetModuleClock(UART_Desc[u32Idx].module, u32ClkSrc, CLK_UART_CLK_DIVIDER(u32ClkDiv));

    /* Reset IP */
    //SYS_ResetModule(UART_Desc[u32Idx].module);

    /* Enable Interrupt */
    UART_ENABLE_INT(uart_device, UART_IER_RDA_IE_Msk);

    NVIC_EnableIRQ(u32IrqId);

    /* Configure UART and set UART Baudrate */
    UART_Open(uart_device, baud);
#endif//#if defined(__M451__)
    SYS_LockReg();
#endif//#if defined(__M460MINIMA__)
}

void HardwareSerial::end(void)
{
#if defined(__M460MINIMA__)
    if( use_vcom == 1)
    {
    /*Do nothing*/

    }
#else    
    UART_Close(uart_device);
#endif    
}

/*
 * I/O
 */

int HardwareSerial::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer->head == _rx_buffer->tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
        _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % SERIAL_BUFFER_SIZE;
#if defined(__M460MINIMA__)    
        gu32RxCount--;
#endif        
        return c;
    }
}

int HardwareSerial::availableForWrite(void)
{
    /* For NUC131 */
#if defined(__NUC131__)
    if (UART_IS_TX_FULL(uart_device))
    {
        return 0;
    }
    else
    {
        return (UART0_FIFO_SIZE - ((uart_device->FSR & UART_FSR_TX_POINTER_Msk) >> UART_FSR_TX_POINTER_Pos));
    }
#elif defined(__M460__)
#if defined (__M460MINIMA__)
    if( use_vcom == 1 )
    {
        /*
            To do VCOM availability
        */
        return ((int)(EPA_MAX_PKT_SIZE) - (int)(vcom_device->EP[EPA].EPDATCNT & 0xffff));
    }
#else        
    if (UART_IS_TX_FULL(uart_device))
    {
        return 0;
    }
    else
    {
        if(uart_device == UART0)
            return (UART0_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
        else if(uart_device == UART1)
            return (UART1_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
		else if(uart_device == UART3)
            return (UART3_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
        else if(uart_device == UART4)
            return (UART4_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
    }
#endif//(__M460MINIMA__)    
#elif defined(__M032BT__) || defined(__M032KG__)|| defined(__M252__) || defined(__M480__)
    if (UART_IS_TX_FULL(uart_device))
    {
        return 0;
    }
    else
    {
        if(uart_device == UART0)
            return (UART0_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
        else if(uart_device == UART1)
            return (UART1_FIFO_SIZE - ((uart_device->FIFOSTS & UART_FIFOSTS_TXPTR_Msk) >> UART_FIFOSTS_TXPTR_Pos));
    }
#endif

}

int HardwareSerial::available(void)
{
#if defined(__M460MINIMA__)    
    return (int)(gu32RxCount);
#else
    return (unsigned int)(SERIAL_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % SERIAL_BUFFER_SIZE;
#endif    
}

size_t HardwareSerial::write(const uint8_t ch)
{
#if defined(__M451__) || defined(__M032BT__) || defined(__M032KG__) || defined(__M252__)|| defined(__M480__)
    while (uart_device->FIFOSTS & UART_FIFOSTS_TXFULL_Msk);
    uart_device->DAT = ch;
    return 1;
#elif defined(__M460__)
    #if defined(__M460MINIMA__)
        /*To do:VCOM Tx, single byte TX*/
        vcom_device->EP[EPA].EPDAT_BYTE = ch;
        vcom_device->EP[EPA].EPRSPCTL = HSUSBD_EP_RSPCTL_SHORTTXEN;    // packet end
        vcom_device->EP[EPA].EPTXCNT = 1;
        HSUSBD_ENABLE_EP_INT(EPA, HSUSBD_EPINTEN_INTKIEN_Msk);
    #else
        while (uart_device->FIFOSTS & UART_FIFOSTS_TXFULL_Msk);
        uart_device->DAT = ch;
    #endif
    return 1;
#elif defined(__NUC240__) || defined(__NUC131__)
    while (uart_device->FSR & UART_FSR_TX_FULL_Msk);
    uart_device->THR = ch;
    return 1;
#elif defined(__NANO100__)
    while (uart_device->FSR & UART_FSR_TX_FULL_F_Msk);
    uart_device->THR = ch;
    return 1;
#elif defined(__NANO1X2__)
    while (UART_IS_TX_FULL(uart_device));
    UART_WRITE(uart_device, ch);
    return 1;
#endif
}

void HardwareSerial::flush(void)
{
#if defined(__M451__)
    while (!(uart_device->FIFOSTS & UART_FIFOSTS_TXEMPTY_Msk));
#endif
    /* No need to implement because we use hardware UART and it is with hardware FIFO*/
}


int HardwareSerial::peek(void)
{
    if (_rx_buffer->head == _rx_buffer->tail)
    {
        return -1;
    }
    else
    {
        return _rx_buffer->buffer[_rx_buffer->tail];
    }
}
