/******************************************************************************
 * @file     USBCore.cpp
 * @brief    NUC200 series USBD driver Sample file
 * @version  1.0.0
 * @date     10, Sep, 2014
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

/*!<Includes */
//#include "Arduino.h"
#include "USBAPI.h"


#ifdef __cplusplus
extern "C" {
#endif
uint8_t volatile g_u8EPKBReady = 0;


#ifdef __cplusplus
}
#endif

//=======================================================================
//=======================================================================
#if defined(__M460MINIMA__)

USBDevice_ USBDevice;

USBDevice_::USBDevice_()
{
}


bool USBDevice_::attach(void)
{    

	/*
	    To do: check some flag 
	    to make sure attach is done.
	*/

	

	return true;	
}

bool USBDevice_::detach(void)
{

	/*Do nothing. Keep VCOM*/
	//NVIC_DisableIRQ(USBD_IRQn);


	return true;
}

#endif