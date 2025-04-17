/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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

#include "WInterrupts.h"
#if defined (__M460__)
typedef void (*interruptCB)(void);
#define GNUM 16
static interruptCB callbacksGPA[GNUM];
static interruptCB callbacksGPB[GNUM];
static interruptCB callbacksGPC[GNUM];
static interruptCB callbacksGPD[GNUM];
static interruptCB callbacksGPE[GNUM];
static interruptCB callbacksGPF[GNUM];

/* for software serial */
uint8_t g_u8Softserail_enable=0;
uint8_t g_u8Softserail_port_num;
uint8_t g_u8Softserail_pin_num;
static interruptCB pfn_software_uart_handler = NULL;
GPIO_T* g_u8Softserail_port_base;

/* Configure GPIO interrupt sources */
static void __initialize() 
{
	int i;
	for (i=0; i<GNUM; i++) 
	{
		callbacksGPA[i] = NULL;
		callbacksGPB[i] = NULL;
		callbacksGPC[i] = NULL;
		callbacksGPD[i] = NULL;
		callbacksGPE[i] = NULL;
		callbacksGPF[i] = NULL;
	}
	
	NVIC_EnableIRQ(GPA_IRQn);
	NVIC_EnableIRQ(GPB_IRQn);
	NVIC_EnableIRQ(GPC_IRQn);
	NVIC_EnableIRQ(GPD_IRQn);
	NVIC_EnableIRQ(GPE_IRQn);
	NVIC_EnableIRQ(GPF_IRQn);	
}

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	static int enabled = 0;
	
#ifdef USE_BoardToPin	
	if(pin > BoardToPin_MAX_COUNT) return;
	pin=BoardToPinInfo[pin].pin;
#else
    if(pin>GPIO_MAX_COUNT || GPIO_Desc[pin].P==NULL) return;
#endif
  		
	if (!enabled) {
		__initialize();
		enabled = 1;
	}
	// Retrieve pin information
	GPIO_T *pio = GPIO_Desc[pin].P;
	uint32_t mask = GPIO_Desc[pin].bit;
	uint32_t pos = 0;
	uint32_t t;
	
	for (t = mask; t>1; t>>=1, pos++);


    /*special mode for Softserial*/
    if(   (g_u8Softserail_enable)
        &&(pio == g_u8Softserail_port_base) 
        &&(pos == g_u8Softserail_pin_num) 
    ) { //check if the softserial rx pin
        pfn_software_uart_handler = callback;
        
    } else { //origin path


	// Set callback function
	
	if (pio == PA )
		callbacksGPA[pos] = callback;
	else if (pio == PB )
		callbacksGPB[pos] = callback;
	else if (pio == PC )
		callbacksGPC[pos] = callback;
	else if (pio == PD )
		callbacksGPD[pos] = callback;
	else if (pio == PE )
		callbacksGPE[pos] = callback;
	else if (pio == PF )
		callbacksGPF[pos] = callback;
	

	}


	// Enable interrupt
	if(mode == FALLING)
	{
		GPIO_EnableInt(pio,pos,GPIO_INT_FALLING);
	}
	else if(mode == RISING)
	{
		GPIO_EnableInt(pio,pos,GPIO_INT_RISING);	
	}		
	else if(mode == CHANGE)
	{
		GPIO_EnableInt(pio,pos,GPIO_INT_BOTH_EDGE);		
	}
		
}

void detachInterrupt(uint32_t pin)
{

#ifdef USE_BoardToPin	
	if(pin > BoardToPin_MAX_COUNT) return;
	pin=BoardToPinInfo[pin].pin;
#else
   if(pin>GPIO_MAX_COUNT || GPIO_Desc[pin].P==NULL) return;
#endif
  	
	// Retrieve pin information
	GPIO_T *pio = GPIO_Desc[pin].P;
	uint32_t mask = GPIO_Desc[pin].bit;
	uint32_t pos = 0;
	uint32_t t;
	for (t = mask; t>1; t>>=1, pos++);	

	// Disable interrupt
	GPIO_DisableInt(pio,pos);
	

    /*special mode for Softserial*/
    if( (g_u8Softserail_enable) && (pio == g_u8Softserail_port_base) && (pos == g_u8Softserail_pin_num))
	{ //check if the softserial rx pin
        g_u8Softserail_enable=0;
        pfn_software_uart_handler = NULL;
        
    } 
	else 
	{ //orign path

	    if (pio == PA )
		    callbacksGPA[pos] = NULL;
	    else if (pio == PB )
		    callbacksGPB[pos] = NULL;
	    else if (pio == PC )
		    callbacksGPC[pos] = NULL;
	    else if (pio == PD )
		    callbacksGPD[pos] = NULL;
	    else if (pio == PE )
		    callbacksGPE[pos] = NULL;
	    else if (pio == PF )
		    callbacksGPF[pos] = NULL;			

	}


}

#ifdef __cplusplus
extern "C" {
#endif

void GPA_IRQHandler(void)
{

	uint32_t i;		
	/* For SoftSerial patch */  
    /* put handler here for better responding time for higher baudrate. */
    if( pfn_software_uart_handler) 
	{ 
        pfn_software_uart_handler();
    } 

	
	for (i=0; i<GNUM; i++) 
	{
	  	if(PA->INTSRC & (1<<i))
	  	{ 
	  		if (callbacksGPA[i]) callbacksGPA[i]();	  		
	  		PA->INTSRC = (1<<i);
	  	}
	}  
}
void GPB_IRQHandler(void)
{

	uint32_t i;		
	/* For SoftSerial patch */  
    /* put handler here for better responding time for higher baudrate. */
    if( pfn_software_uart_handler) 
	{ 
        pfn_software_uart_handler();
    } 


	for (i=0; i<GNUM; i++) 
	{
	  	if(PB->INTSRC & (1<<i))
	  	{ 
	  		if (callbacksGPB[i]) callbacksGPB[i]();	  		
	  		PB->INTSRC = (1<<i);
	  	}
	}  
}

void GPC_IRQHandler(void)
{

	uint32_t i;		
	/* For SoftSerial patch */  
    /* put handler here for better responding time for higher baudrate. */
    if( pfn_software_uart_handler) 
	{ 
        pfn_software_uart_handler();
    } 
	
	for (i=0; i<GNUM; i++) 
	{
	  	if(PC->INTSRC & (1<<i))
	  	{ 
	  		if (callbacksGPC[i]) callbacksGPC[i]();	  		
	  		PC->INTSRC = (1<<i);
	  	}
	}  
}

void GPD_IRQHandler(void)
{

	uint32_t i;		
	/* For SoftSerial patch */  
    /* put handler here for better responding time for higher baudrate. */
    if( pfn_software_uart_handler) 
	{ 
        pfn_software_uart_handler();
    } 

	for (i=0; i<GNUM; i++) 
	{
	  	if(PD->INTSRC & (1<<i))
	  	{ 
	  		if (callbacksGPD[i]) callbacksGPD[i]();	  		
	  		PD->INTSRC = (1<<i);
	  	}
	}  
}


void GPE_IRQHandler(void)
{

	uint32_t i;		
	/* For SoftSerial patch */  
    /* put handler here for better responding time for higher baudrate. */
    if( pfn_software_uart_handler) 
	{ 
        pfn_software_uart_handler();
    } 

	for (i=0; i<GNUM; i++) 
	{
	  	if(PE->INTSRC & (1<<i))
	  	{ 
	  		if (callbacksGPE[i]) callbacksGPE[i]();	  		
	  		PE->INTSRC = (1<<i);
	  	}
	}  
}

void GPF_IRQHandler(void)
{

	uint32_t i;		
	/* For SoftSerial patch */  
    /* put handler here for better responding time for higher baudrate. */
    if( pfn_software_uart_handler) 
	{ 
        pfn_software_uart_handler();
    } 

	for (i=0; i<GNUM; i++) 
	{
	  	if(PF->INTSRC & (1<<i))
	  	{ 
	  		if (callbacksGPF[i]) callbacksGPF[i]();	  		
	  		PF->INTSRC = (1<<i);
	  	}
	}  
}

#ifdef __cplusplus
}
#endif



#endif
