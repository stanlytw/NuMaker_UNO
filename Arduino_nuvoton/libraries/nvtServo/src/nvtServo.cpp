/*
  Copyright (c) 2013 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "Arduino.h"
#include <nvtServo.h>
volatile uint32_t Servo_MAX=180;
volatile uint32_t Servo_MIN=0;
volatile uint32_t Pwm2Servo_max=13; //For Tower Pro SG90 
volatile uint32_t Pwm2Servo_min=3;
int Servo_Pin=0;
int Servo_Val=0;

// Micro servo must learn to adjust with PWM
uint32_t mul_max=9;  //For GS-9018
uint32_t mul_min=39; //For GS-9018

Servo::Servo()
{

}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, Servo_MIN, Servo_MAX);
}
static uint8_t pinEnabled[PWM_MAX_COUNT];
static uint32_t fixValue[PWM_MAX_COUNT];
uint8_t Servo::attach(int ulPin, int min, int max)
{      
	uint32_t CNR;
	uint32_t ulValue=0;
	#ifdef USE_BoardToPin
	if(ulPin > BoardToPin_MAX_COUNT) return 0;
	if(BoardToPinInfo[ulPin].type!=PWM_TYPE) return 0;
	ulPin=BoardToPinInfo[ulPin].num;
	#else
	if(ulPin>PWM_MAX_COUNT || PWM_Desc[ulPin].P==NULL) return 0;
	#endif	
#if defined(__M451__)
	if(ulValue==100)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT |= GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#elif defined(__M252__) || defined(__M480__) || defined(__M460__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#elif defined(__NUC240__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#elif defined(__NANO100__) | defined(__NANO1X2__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#elif defined(__NUC131__)
	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_PMD_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		//return 0;
	}
#endif	


	if (!pinEnabled[ulPin]){
		//Set Mutifunction pins
		PWM_Config(PWM_Desc[ulPin]);		
 #if defined(__M480__) 
	//Config PWMs
		EPWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,50,0);
		
		//Enable PWM output
		EPWM_EnableOutput(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		//Start PWM
		EPWM_Start(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
 #else  
		//Config PWMs
		PWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,50,0);
		
		//Enable PWM output
		PWM_EnableOutput(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		//Start PWM
		PWM_Start(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
#endif		
		pinEnabled[ulPin] = 1;
	}
	
	//Config PWMs		
	if(fixValue[ulPin]!=ulValue)
	{
 #if defined(__M480__) 
        EPWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,50,ulValue);
 #else		
		PWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,50,ulValue);
		
#endif
		fixValue[ulPin]=ulValue;
	}
	
	Servo_MAX=max;
  Servo_MIN=min;
  Servo_Pin=ulPin;
  
#if defined(__NUC131__)
	CNR=(PWM_Desc[ulPin].P)->PERIOD[(((PWM_Desc[ulPin].ch)>>1<<1))] - 1;
	Pwm2Servo_min= (CNR/mul_min);
	Pwm2Servo_max= (CNR/mul_max);  //1ms
#else

#endif
}

void Servo::detach()
{

}

void Servo::write(int value)
{
    if (value < Servo_MIN)
      value = Servo_MIN;
    else if (value > Servo_MAX)
      value = Servo_MAX;

    Servo_Val=value;
    value = map(value, Servo_MIN, Servo_MAX, Pwm2Servo_min, Pwm2Servo_max);
#if defined(__NUC131__)
	  PWM_SET_CMR(PWM_Desc[Servo_Pin].P,PWM_Desc[Servo_Pin].ch,value);
		(PWM_Desc[Servo_Pin].P)->WGCTL0 &= ~((PWM_WGCTL0_PRDPCTL0_Msk | PWM_WGCTL0_ZPCTL0_Msk) << (PWM_Desc[Servo_Pin].ch * 2));
		(PWM_Desc[Servo_Pin].P)->WGCTL0 |= (PWM_OUTPUT_LOW << (PWM_Desc[Servo_Pin].ch * 2 + PWM_WGCTL0_PRDPCTL0_Pos));
		(PWM_Desc[Servo_Pin].P)->WGCTL1 &= ~((PWM_WGCTL1_CMPDCTL0_Msk | PWM_WGCTL1_CMPUCTL0_Msk) << (PWM_Desc[Servo_Pin].ch * 2));
		(PWM_Desc[Servo_Pin].P)->WGCTL1 |= (PWM_OUTPUT_HIGH << (PWM_Desc[Servo_Pin].ch * 2 + PWM_WGCTL1_CMPDCTL0_Pos));
#elif defined(__M480__)
	EPWM_ConfigOutputChannel(PWM_Desc[Servo_Pin].P,PWM_Desc[Servo_Pin].ch,50,value);	 
#else 
    PWM_ConfigOutputChannel(PWM_Desc[Servo_Pin].P,PWM_Desc[Servo_Pin].ch,50,value);	 
#endif
}

int Servo::read() // return the value as degrees
{
  return Servo_Val;
}
