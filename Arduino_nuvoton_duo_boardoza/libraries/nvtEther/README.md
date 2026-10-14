# Nuvoton Arduino Ethernet Library (nvtEthernet)
This library is an porting for Nuvoton M467 board, based on [STM32Ethernet](https://github.com/stm32duino/Arduino_Core_STM32/wiki/STM32Ethernet) library/API.<br>
It follows the Ethernet API from Arduino.<br>

## Functionality
This library enables an Nuvoton M467 board with built-in Ethernet to connect to the Internet.<br>
Ethernet interface driver are rewritten according to Nuvoton's Ethernet device.

- nvtEthernet.cpp/h<br>
	Override the `Ethernet` class referrenced by Arduino's Ethernet library.

- nvt_eth.cpp/h<br>
	Some interface functions referenced by `Ethernet` and other class.

- ethernetif.cpp/h<br>
	Low level Ethernet interface`(netif)` porting to link Nuvoton M467 board and LwIP stack.

## Configuration
User-define options of LwIP for Nuvoton M467 board is configured in the `lwipopts.h` file.<br> 
Experienced users of LwIP/Nuvoton M467 board can modify this file for application specific purpose.<br>

## Note
The timer to trigger the idle task is implemented by 'TIMER0'.<br> 
This idle task handles timer and data reception. It is required by the LwIP stack.<br>
This idle task is triggered with 1 ms interval by function `nvt_eth_scheduler()`.<br>
For test, just clone all files and locate it in Nuvoton board packege folders.<br>
`Arduino15/packages/nuvoton/hardware/nuvoton/x.y.z/libraries/nvtEthernet`
