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

#ifdef __cplusplus
extern "C" {
#endif

static int _readResolution = 10;
static int _writeResolution = 8;

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
	if (from == to)
		return value;
	if (from > to)
		return (value >> (from-to));
	else
		return (value << (to-from));
}

eAnalogReference analog_reference = AR_DEFAULT;

void analogReference(eAnalogReference ulMode)
{
	analog_reference = ulMode;
}

uint32_t analogRead(uint32_t ulPin)
{
#if defined(__M460__)
    volatile uint32_t ulValue = 0;  
#if 0//def USE_BoardToPin
    if (ulPin > BoardToPin_MAX_COUNT) return;
	if(BoardToPinInfo[ulPin].type!=ADC_TYPE) return;
    //if (BoardToPinInfo[ulPin].pin == -1) return;
    ulPin = BoardToPinInfo[ulPin].num;
#else
    if(ulPin>ADC_MAX_COUNT || ADC_Desc[ulPin].A==NULL) return 0;  	  
#endif  	  	
	/* Disable the digital input path to avoid the leakage current. */
    GPIO_DISABLE_DIGITAL_PATH(GPIO_Desc[ADC_Desc[ulPin].pintype.num].P,GPIO_Desc[ADC_Desc[ulPin].pintype.num].bit);
   
    ADC_Config(ADC_Desc[ulPin]);
  
    /* Set the ADC internal sampling time, input mode as single-end and enable the A/D converter */
#if defined(__M460__)
	EADC_Open(ADC_Desc[ulPin].A, 0);
	EADC_SetExtendSampleTime(EADC, ADC_Desc[ulPin].ch, 6);
#else

#endif
	
	/* Configure the sample 0 module for analog input channel 0 and enable ADINT0 trigger source */
	EADC_ConfigSampleModule(EADC,ADC_Desc[ulPin].ch, EADC_ADINT0_TRIGGER, ADC_Desc[ulPin].ch);
	
	/* Clear the A/D ADINT0 interrupt flag for safe */
	EADC_CLR_INT_FLAG(EADC, 0x1);
	
	/* Enable sample module interrupt */
	EADC_ENABLE_SAMPLE_MODULE_INT(EADC, 0, (0x1 << ADC_Desc[ulPin].ch));
	
	/* Trigger sample module to start A/D conversion */
	EADC_START_CONV(EADC, (0x1 << ADC_Desc[ulPin].ch));
	
	while(!(EADC_GET_INT_FLAG(EADC,0x1)));
  
	ulValue=EADC_GET_CONV_DATA(EADC, ADC_Desc[ulPin].ch);
  
 	/* Wait conversion done */
 	while(EADC_GET_DATA_VALID_FLAG(EADC, (0x1 << ADC_Desc[ulPin].ch)) != ( 0x1U << ADC_Desc[ulPin].ch));
	
	/* Disable the sample module interrupt */
	EADC_DISABLE_SAMPLE_MODULE_INT(EADC, 0, (1<<ADC_Desc[ulPin].ch));
	
	ulValue=EADC_GET_CONV_DATA(EADC, ADC_Desc[ulPin].ch);
	ulValue = mapResolution(ulValue, 12, _readResolution);

    // Close ADC
    EADC_Close(ADC_Desc[ulPin].A);

#endif
  return ulValue;	
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
static uint8_t pinEnabled[PWM_MAX_COUNT]={0};
static uint32_t fixValue[PWM_MAX_COUNT]={0};
void analogWrite(uint32_t ulPin, uint32_t ulValue) {
	
#ifdef USE_BoardToPin
	if(ulPin > BoardToPin_MAX_COUNT) return;
	if(BoardToPinInfo[ulPin].type!=PWM_TYPE) return;
	ulPin=BoardToPinInfo[ulPin].num;
#else
	if(ulPin>PWM_MAX_COUNT || PWM_Desc[ulPin].P==NULL) return;
#endif
	
	ulValue = mapResolution(ulValue, _writeResolution, 8);
	
	ulValue=((ulValue+1)*100)/(1<<_writeResolution);

	if(ulValue==0)
	{  
		int32_t pin=PWM_Desc[ulPin].pintype.num;
		GPIO_Config(GPIO_Desc[pin]);
		GPIO_SetMode(GPIO_Desc[pin].P, GPIO_Desc[pin].bit, GPIO_MODE_OUTPUT);
		(GPIO_Desc[pin].P)->DOUT &= ~GPIO_Desc[pin].bit;
		pinEnabled[ulPin]= 0;
		fixValue[ulPin]=ulValue;
		return;
	}


	if (!pinEnabled[ulPin]){
		
		if(PWM_Desc[ulPin].moduletype == PWM_USE_EPWM )
		{	
		    //Set Mutifunction pins, module type independent
		    PWM_Config(PWM_Desc[ulPin]);		

            //Config PWMs
		    EPWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
		
		    //Enable PWM output
		    EPWM_EnableOutput(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));
		
		    //Start PWM
		    EPWM_Start(PWM_Desc[ulPin].P,(1<<PWM_Desc[ulPin].ch));  

		    pinEnabled[ulPin] = 1;
		}
		else if(PWM_Desc[ulPin].moduletype == PWM_USE_BPWM )
		{
			//Set Mutifunction pins, module type independent
		    PWM_Config(PWM_Desc[ulPin]);		

            //Config PWMs
		    BPWM_ConfigOutputChannel((BPWM_T  *)(PWM_Desc[ulPin].P),PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
		
		    //Enable PWM output
		    BPWM_EnableOutput((BPWM_T  *)(PWM_Desc[ulPin].P),(1<<PWM_Desc[ulPin].ch));
		
		    //Start PWM
		    BPWM_Start((BPWM_T  *)(PWM_Desc[ulPin].P),(1<<PWM_Desc[ulPin].ch));  
			
			pinEnabled[ulPin] = 1;
		}
		else
		{
			/*To do*/
		}
	}
	
	//Config PWMs		
	if(fixValue[ulPin]!=ulValue)
	{
		if(PWM_Desc[ulPin].moduletype == PWM_USE_EPWM )
		{	
		    EPWM_ConfigOutputChannel(PWM_Desc[ulPin].P,PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
	    }
		else if(PWM_Desc[ulPin].moduletype == PWM_USE_BPWM )
		{
			BPWM_ConfigOutputChannel((BPWM_T  *)(PWM_Desc[ulPin].P),PWM_Desc[ulPin].ch,PWM_Desc[ulPin].freq,ulValue);
		}
		else
		{
		    /*To do*/	
		}
    	fixValue[ulPin]=ulValue;
	}

	return;
}

#ifdef __cplusplus
}
#endif
