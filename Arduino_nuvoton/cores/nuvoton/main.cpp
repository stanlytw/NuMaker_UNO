/*
  Copyright (c) 2011 Arduino.  All right reserved.

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

#include "Arduino.h"
// #include "HardwareSerial.h"
void  __attribute__((weak)) loop(void)
{


}

void __attribute__((weak)) setup(void)
{
  // _TEST_POINT();
}

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{

#if ENABLE_C_PRINTF && defined(__M032BT__)
  SYS_UnlockReg();
  
  CLK_EnableModuleClock(UART0_MODULE);
  CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));
  SYS->GPA_MFPL = (SYS->GPA_MFPL & ~(SYS_GPA_MFPL_PA0MFP_Msk | SYS_GPA_MFPL_PA1MFP_Msk))    |       \
                    (SYS_GPA_MFPL_PA0MFP_UART0_RXD | SYS_GPA_MFPL_PA1MFP_UART0_TXD);
  SYS_LockReg();
  UART_Open(UART0, 115200);
#endif

	init();	//nuvoton board init

	#if defined(__M451__) || defined(__NUC240__) || defined(__NANO100__)
    USBDevice.attach();
	#endif

	setup();

	for (;;)
	{
      loop();
      if(serialEventRun) serialEventRun();
	}

	return 0;
}
