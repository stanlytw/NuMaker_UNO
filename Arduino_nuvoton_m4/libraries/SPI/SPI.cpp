/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"
					

SPIClass::SPIClass(void*_spi,uint32_t _module,uint32_t _clksel, IRQn_Type _id, void(*_initCb)(void)) :
	_vspi(_spi), id(_id), initCb(_initCb)
{		
	  initCb();		  
	  _vspi=_spi;
	  clksel=_clksel;
	  module=_module;
	  id=_id;

}

void SPIClass::begin() {		
	
	if(init_flag==0) init();
	
	 /* Unlock protected registers */
	SYS_UnlockReg();	

	/* Enable IP clock */       
	CLK_EnableModuleClock(module);    	
   	
	/* Select IP clock source and clock divider */
	CLK_SetModuleClock(module,clksel,0);
	
	/* Lock protected registers */
	SYS_LockReg();
			  
	/* Configure as a master, clock idle low, falling clock edge Tx, rising edge Rx and 8-bit transaction */
	/* Set IP clock divider. SPI clock rate = 4MHz */  
#if defined(__M032BT__) 
	
#elif defined(__M252__) || defined(__M480__) || defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ))
	{
		SPI_Open(spi, SPI_MASTER, SPI_MODE_0, 8, 4000000); 
		
		/*
		    For Arduino application, skip auuto-SS. If required, umcomment following code.
		*/
		//SPI_EnableAutoSS(spi, SPI_SS, SPI_SS_ACTIVE_LOW);
		
	}
	else if(_vspi == USPI0)
	{
		USPI_Open(uspi, USPI_MASTER, USPI_MODE_0, 8, 4000000); 
	}
#endif

#if defined(__NUC240__)	|| defined(__NANO100__) || defined(__NUC131__)
	
#elif defined(__M252__)
	
#endif
	setBitOrder(SS, MSBFIRST);  

#if defined(__M451__) 
	
#elif defined(__M032BT__)
	
#elif defined(__M252__) || defined(__M480__)|| defined(__M032KG__)
	if (_vspi == SPI0 )
	{
		SPI_ClearRxFIFO(spi);
	}
	else if(_vspi == USPI0)
	{
		USPI_ClearRxBuf(uspi);
		USPI_ClearTxBuf(uspi);
	}
#endif
}

void SPIClass::begin(uint8_t _pin) {
if(init_flag==0) init();
#if defined(__M451__)	
	

#elif defined(__M032BT__)
	

#elif defined(__NUC240__) || defined(__NUC131__)
	

#elif defined(__NANO100__)
	
#elif defined(__M252__) || defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	 /* Unlock protected registers */
	SYS_UnlockReg();	

	/* Enable IP clock */       
	CLK_EnableModuleClock(module);    	
   	
	/* Select IP clock source and clock divider */
	CLK_SetModuleClock(module,clksel,0);
	
	/* Lock protected registers */
	SYS_LockReg();

	if( (_vspi == SPI0 ) || (_vspi == SPI2 ))
	{
		SPI_Open(spi, SPI_MASTER, SPI_MODE_0, 8, 4000000);
				
		SPI_SetFIFO(spi, 12, 12); 
		SPI_ClearRxFIFO(spi);
	}
	else if(_vspi == USPI0)
	{
		USPI_Open(uspi, USPI_MASTER, USPI_MODE_0, 8, 4000000); 
		USPI_ClearRxBuf(uspi);
		USPI_ClearTxBuf(uspi);
	}
    
    setBitOrder(SS, MSBFIRST);

#endif
}

void SPIClass::end(uint8_t _pin) {
		
#if defined(__M032BT__)
		
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		SPI_Close(spi);
	}
	else if(_vspi == USPI0)
	{
		USPI_Close(uspi);
	}
#else
		SPI_Close(spi);
#endif
}

void SPIClass::end() {

#if defined(__M032BT__)
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ))
	{
		SPI_Close(spi);
	}
	else if(_vspi == USPI0)
	{
		USPI_Close(uspi);
	}
#else
	SPI_Close(spi);
#endif

}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
}

void SPIClass::beginTransaction(uint8_t pin, SPISettings settings)
{
	if(init_flag==0) init();

#if defined(__M032BT__)
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)

	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		SPI_Open(spi, SPI_MASTER, settings.datmode, 8, settings.clock); 
	}
	else if(_vspi == USPI0)
	{
		USPI_Open(uspi, USPI_MASTER, settings.datmode, 8, settings.clock); 
	}

	setBitOrder(SS, settings.border);
#else
	

#endif

#if defined(__M032BT__)
	
#elif defined(__M451__) || defined(__NANO100__)	
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		SPI_ClearRxFIFO(spi);
	}
	else if(_vspi == USPI0)
	{
		USPI_ClearRxBuf(uspi);
		USPI_ClearTxBuf(uspi);
	}
#endif
}

void SPIClass::endTransaction(void)
{
	
}

void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder) {	
	
#if defined(__M032BT__)
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		if(_bitOrder==LSBFIRST)
			SPI_SET_LSB_FIRST(spi);
		else
			SPI_SET_MSB_FIRST(spi);
	}
	else if(_vspi == USPI0)
	{

		if(_bitOrder==LSBFIRST)
			USPI_SET_LSB_FIRST(uspi);
		else
			USPI_SET_MSB_FIRST(uspi);		
	}
#else
	if(_bitOrder==LSBFIRST)
		SPI_SET_LSB_FIRST(spi);
	else
		SPI_SET_MSB_FIRST(spi);
#endif
}

void SPIClass::setDataMode(uint8_t _pin, uint8_t _mode) {	
#if defined(__M451__)
	
#elif defined(__M032BT__)
	
#elif defined(__NUC240__) || defined(__NUC131__)
    
#elif defined(__NANO100__)
    
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ))
	{
	    spi->CTL = (spi->CTL & ~SPI_MODE_Msk) | _mode;
	}
	else if(_vspi == USPI0)
	{
		uspi->PROTCTL = (uspi->PROTCTL & ~USPI_PROTCTL_SCLKMODE_Msk)|_mode;
	}
#endif
}

void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider) {	
#if defined(__M032BT__)
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		spi->CLKDIV = (spi->CLKDIV & ~0xffff) | _divider;
	}
	else if(_vspi == USPI0)
	{
		uspi->BRGEN &= ~USPI_BRGEN_CLKDIV_Msk;
    	uspi->BRGEN |= (_divider << USPI_BRGEN_CLKDIV_Pos);
	}
#else
	
#endif
}

byte SPIClass::transfer(byte _pin, uint8_t _data, SPITransferMode _mode) {	
	uint32_t rdata;
	
#if defined(__M032BT__)
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		SPI_WRITE_TX(spi, _data);
		while(SPI_IS_BUSY(spi));
		return (SPI_READ_RX(spi) & 0xff);
	}
	else if(_vspi == USPI0)
	{
		USPI_WRITE_TX(uspi, _data);
		while(USPI_IS_BUSY(uspi));
		return (USPI_READ_RX(uspi) & 0xff);
	}
#else
	SPI_WRITE_TX(spi, _data);
	SPI_TRIGGER(spi);
	while(SPI_IS_BUSY(spi));
	return (SPI_READ_RX(spi) & 0xff);
#endif

}			

uint16_t SPIClass::transfer16(byte _pin, uint16_t _data, SPITransferMode _mode) {
	uint32_t rdata;

#if defined(__M032BT__)
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		SPI_WRITE_TX(spi, _data&0xff);
		while(SPI_IS_BUSY(spi));
		rdata=(SPI_READ_RX(spi) & 0xff);
		SPI_WRITE_TX(spi, _data>>8);
		while(SPI_IS_BUSY(spi));
		rdata=rdata | ((SPI_READ_RX(spi) & 0xff)<<8);
	}
	else if(_vspi == USPI0)
	{
		USPI_WRITE_TX(uspi, _data&0xff);
		while(USPI_IS_BUSY(uspi));
		rdata=(USPI_READ_RX(uspi) & 0xff);
		USPI_WRITE_TX(uspi, _data>>8);
		while(USPI_IS_BUSY(uspi));
		rdata=rdata | ((USPI_READ_RX(uspi) & 0xff)<<8);
	}
#else
	
#endif

	return rdata;
}

void SPIClass::transfer(byte _pin, void *_buf, size_t _count, SPITransferMode _mode) {
	uint32_t i;
	uint8_t *buf;
	buf=(uint8_t *)_buf;
#if defined(__M032BT__)
	
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) || defined(__M032KG__)
	if ((_vspi == SPI0 ) || (_vspi == SPI2 ) )
	{
		for(i=0;i<_count;i++)
		{	
			SPI_WRITE_TX(spi, buf[i]);
			while(SPI_IS_BUSY(spi));
		}
	}
	else if(_vspi == USPI0)
	{
		for(i=0;i<_count;i++)
		{	
			USPI_WRITE_TX(uspi, buf[i]);
			while(USPI_IS_BUSY(uspi));
		}

	}
#else
	
#endif
}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
}

#if SPI_MAX_COUNT > 0
#if defined(__M467SJHAN__)
#define SPI_ID0 0
#else
#define SPI_ID0 1
#endif	


static void SPI_0_Init(void) {	
	SPI_Config(SPI_Desc[SPI_ID0]);	
}
SPIClass SPI(SPI_Desc[SPI_ID0].S,SPI_Desc[SPI_ID0].module,SPI_Desc[SPI_ID0].clksel,SPI_Desc[SPI_ID0].irq,SPI_0_Init);
#endif

#if SPI_MAX_COUNT >1
#if defined(__M467SJHAN__)
#define SPI_ID1 1
#else
#define SPI_ID1 0
#endif

static void SPI_1_Init(void) {	
	SPI_Config(SPI_Desc[SPI_ID1]);	
}
SPIClass SPI1(SPI_Desc[SPI_ID1].S,SPI_Desc[SPI_ID1].module,SPI_Desc[SPI_ID1].clksel,SPI_Desc[SPI_ID1].irq,SPI_1_Init);
#endif