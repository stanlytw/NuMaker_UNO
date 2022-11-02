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
#if defined(__NUC240__) | defined(__NANO100__)
#define SPI_WRITE_TX(spi, u32TxData) 	SPI_WRITE_TX0(spi, u32TxData);
#define SPI_READ_RX(spi)                SPI_READ_RX0(spi)
#endif							

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
	USPI_Open(uspi, USPI_MASTER, USPI_MODE_0, 8, 4000000); 
#elif defined(__M252__) || defined(__M480__) || defined(__M460__) 
	if (_vspi == SPI0 )
	{
		SPI_Open(spi, SPI_MASTER, SPI_MODE_0, 8, 4000000); 
	}
	else if(_vspi == USPI0)
	{
		USPI_Open(uspi, USPI_MASTER, USPI_MODE_0, 8, 4000000); 
	}
#else
	SPI_Open(spi, SPI_MASTER, SPI_MODE_0, 8, 4000000); 
#endif

#if defined(__NUC240__)	|| defined(__NANO100__) || defined(__NUC131__)
	SPI_EnableFIFO(spi, 12, 12);
#elif defined(__M252__)
	if (_vspi == SPI0 )
	{
		SPI_SetFIFO(spi, 12, 12); 
	}
#endif
	setBitOrder(SS, MSBFIRST);  

#if defined(__M451__) 
	SPI_ClearRxFIFO(spi);
	SPI_TRIGGER(spi);
#elif defined(__M032BT__)
	USPI_ClearRxBuf(uspi);
	USPI_ClearTxBuf(uspi);
#elif defined(__M252__) || defined(__M480__)
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
	/* Enable IP clock */       
	CLK_EnableModuleClock(module);    	
   	
	/* Select IP clock source and clock divider */
	CLK_SetModuleClock(module,clksel,0);

	/* Configure as a master, clock idle low, falling clock edge Tx, rising edge Rx and 8-bit transaction */
	/* Set IP clock divider. SPI clock rate = 4MHz */  	
	SPI_Open(spi, SPI_MASTER, SPI_MODE_0, 8, 4000000);	
	//SPI_EnableFIFO(spi,12,12);	
	setBitOrder(SS, MSBFIRST);
	
	SPI_ClearRxFIFO(spi);
	SPI_TRIGGER(spi);

#elif defined(__M032BT__)	
	/* Enable IP clock */       
	CLK_EnableModuleClock(module);    	
   	
	/* Select IP clock source and clock divider */
	CLK_SetModuleClock(module,clksel,0);

	/* Configure as a master, clock idle low, falling clock edge Tx, rising edge Rx and 8-bit transaction */
	/* Set IP clock divider. SPI clock rate = 4MHz */  	
	USPI_Open(uspi, USPI_MASTER, USPI_MODE_0, 8, 4000000);	
	
	setBitOrder(SS, MSBFIRST);
	
	USPI_ClearRxBuf(uspi);
	USPI_ClearTxBuf(uspi);
	

#elif defined(__NUC240__) || defined(__NUC131__)
	/* Enable IP clock */       
	CLK_EnableModuleClock(module);    	
   	
	/* Select IP clock source and clock divider */
	CLK_SetModuleClock(module,clksel,0);

	/* Configure as a master, clock idle low, falling clock edge Tx, rising edge Rx and 8-bit transaction */
	/* Set IP clock divider. SPI clock rate = 4MHz */  	
	SPI_Open(spi, SPI_MASTER, SPI_MODE_0, 8, 4000000);	
	SPI_EnableFIFO(spi,12,12);	
	setBitOrder(SS, MSBFIRST);

#elif defined(__NANO100__)
	/* Enable IP clock */       
	CLK_EnableModuleClock(module);    	
   	
	/* Select IP clock source and clock divider */
	CLK_SetModuleClock(module,clksel,0);

	/* Configure as a master, clock idle low, falling clock edge Tx, rising edge Rx and 8-bit transaction */
	/* Set IP clock divider. SPI clock rate = 4MHz */  	
	SPI_Open(spi, SPI_MASTER, SPI_MODE_0, 8, 4000000);
    setBitOrder(SS, MSBFIRST);
    SPI_EnableFIFO(spi,12,12); 
#elif defined(__M252__) || defined(__M480__)|| defined(__M460__) 
	 /* Unlock protected registers */
	SYS_UnlockReg();	

	/* Enable IP clock */       
	CLK_EnableModuleClock(module);    	
   	
	/* Select IP clock source and clock divider */
	CLK_SetModuleClock(module,clksel,0);
	
	/* Lock protected registers */
	SYS_LockReg();

	if (_vspi == SPI0 )
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
		USPI_Close(uspi);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
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
	USPI_Close(uspi);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
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
	/* Configure as a master, clock idle low, falling clock edge Tx, rising edge Rx and 8-bit transaction */
	/* Set IP clock divider. SPI clock rate = 4MHz */  	
	USPI_Open(uspi, USPI_MASTER, settings.datmode, 8, settings.clock);	
	//SPI_EnableFIFO(spi,12,12);	
	setBitOrder(SS, settings.border);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 

	if (_vspi == SPI0 )
	{
		SPI_Open(spi, SPI_MASTER, settings.datmode, 8, settings.clock); 
	}
	else if(_vspi == USPI0)
	{
		USPI_Open(uspi, USPI_MASTER, settings.datmode, 8, settings.clock); 
	}

	setBitOrder(SS, settings.border);
#else
	/* Configure as a master, clock idle low, falling clock edge Tx, rising edge Rx and 8-bit transaction */
	/* Set IP clock divider. SPI clock rate = 4MHz */  	
	SPI_Open(spi, SPI_MASTER, settings.datmode, 8, settings.clock);	
	//SPI_EnableFIFO(spi,12,12);	
	setBitOrder(SS, settings.border);

#endif

#if defined(__M032BT__)
	USPI_ClearRxBuf(uspi);
	USPI_ClearTxBuf(uspi);
#elif defined(__M451__) || defined(__NANO100__)	
	SPI_ClearRxFIFO(spi);
	SPI_TRIGGER(spi);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
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

void SPIClass::endTransaction(void)
{
	
}

void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder) {	
	
#if defined(__M032BT__)
	if(_bitOrder==LSBFIRST)
		USPI_SET_LSB_FIRST(uspi);
	else
		USPI_SET_MSB_FIRST(uspi);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
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
	spi->CTL = (spi->CTL & ~SPI_MODE_Msk) | _mode;
#elif defined(__M032BT__)
	uspi->PROTCTL = (uspi->PROTCTL & ~USPI_PROTCTL_SCLKMODE_Msk)|_mode;
#elif defined(__NUC240__) || defined(__NUC131__)
    spi->CNTRL = (spi->CNTRL & ~SPI_MODE_Msk) | _mode;
#elif defined(__NANO100__)
    spi->CTL = (spi->CTL & ~SPI_MODE_Msk) | _mode;
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
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
	uspi->BRGEN &= ~USPI_BRGEN_CLKDIV_Msk;
    uspi->BRGEN |= (_divider << USPI_BRGEN_CLKDIV_Pos);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
	{
		spi->CLKDIV = (spi->CLKDIV & ~0xffff) | _divider;
	}
	else if(_vspi == USPI0)
	{
		uspi->BRGEN &= ~USPI_BRGEN_CLKDIV_Msk;
    	uspi->BRGEN |= (_divider << USPI_BRGEN_CLKDIV_Pos);
	}
#else
	spi->DIVIDER = (spi->DIVIDER & ~0xffff) | _divider;
#endif
}

byte SPIClass::transfer(byte _pin, uint8_t _data, SPITransferMode _mode) {	
	uint32_t rdata;
	
#if defined(__M032BT__)
	USPI_WRITE_TX(uspi, _data);
	//SPI_TRIGGER(uspi);
	while(USPI_IS_BUSY(uspi));
	return (USPI_READ_RX(uspi) & 0xff);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
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
	USPI_WRITE_TX(uspi, _data&0xff);
	// USPI_TRIGGER(uspi);
	while(USPI_IS_BUSY(uspi));
	rdata=(USPI_READ_RX(uspi) & 0xff);
	USPI_WRITE_TX(uspi, _data>>8);
	// USPI_TRIGGER(uspi);
	while(USPI_IS_BUSY(uspi));
	rdata=rdata | ((USPI_READ_RX(uspi) & 0xff)<<8);
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
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
	SPI_WRITE_TX(spi, _data&0xff);
	SPI_TRIGGER(spi);
	while(SPI_IS_BUSY(spi));
	rdata=(SPI_READ_RX(spi) & 0xff);
	SPI_WRITE_TX(spi, _data>>8);
	SPI_TRIGGER(spi);
	while(SPI_IS_BUSY(spi));
	rdata=rdata | ((SPI_READ_RX(spi) & 0xff)<<8);
#endif

	return rdata;
}

void SPIClass::transfer(byte _pin, void *_buf, size_t _count, SPITransferMode _mode) {
	uint32_t i;
	uint8_t *buf;
	buf=(uint8_t *)_buf;
#if defined(__M032BT__)
	for(i=0;i<_count;i++)
	{	
		USPI_WRITE_TX(uspi, buf[i]);
		//SPI_TRIGGER(spi);
		while(USPI_IS_BUSY(uspi));
	}
#elif defined(__M252__)|| defined(__M480__)|| defined(__M460__) 
	if (_vspi == SPI0 )
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
	for(i=0;i<_count;i++)
	{	
		SPI_WRITE_TX(spi, buf[i]);
		SPI_TRIGGER(spi);
		while(SPI_IS_BUSY(spi));
	}
#endif
}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
}

#if SPI_MAX_COUNT > 0
#define SPI_ID0 1
static void SPI_0_Init(void) {	
	SPI_Config(SPI_Desc[SPI_ID0]);	
}
SPIClass SPI(SPI_Desc[SPI_ID0].S,SPI_Desc[SPI_ID0].module,SPI_Desc[SPI_ID0].clksel,SPI_Desc[SPI_ID0].irq,SPI_0_Init);


#endif

#if SPI_MAX_COUNT >1
#define SPI_ID1 0
static void SPI_1_Init(void) {	
	SPI_Config(SPI_Desc[SPI_ID1]);	
}
SPIClass SPI1(SPI_Desc[SPI_ID1].S,SPI_Desc[SPI_ID1].module,SPI_Desc[SPI_ID1].clksel,SPI_Desc[SPI_ID1].irq,SPI_1_Init);
#endif