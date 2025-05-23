#include "RTClib.h"


typedef void (*interruptRTC)(void);

static interruptRTC callbacksRTC;

uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }
uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
/**************************************************************************/
/*!
    @brief  Start RTC for the RTC in M467
    @return Always True for compatibility.
*/
/**************************************************************************/
bool RTC_M467::begin(void) {
  /*
      RTC hooks to VCOM_Serial, so it must be always true.
  */
  callbacksRTC = NULL;
  return true;
}

/**************************************************************************/
/*!
    @brief  Is the RTC_M467 RTC running. Yes, it hooks to Vcom.
    @return 1 for compatibility
*/
/**************************************************************************/
uint8_t RTC_M467::isrunning(void) 
{ 
  return true;//!(read_register(0) >> 7); 
}

/**************************************************************************/
/*!
    @brief  Set the date and time
    @param dt DateTime object containing the desired date/time
*/
/**************************************************************************/
void RTC_M467::adjust(const DateTime &dt) {
  uint8_t buffer[8] = {0,
                       bin2bcd(dt.second()),
                       bin2bcd(dt.minute()),
                       bin2bcd(dt.hour()),
                       0,
                       bin2bcd(dt.day()),
                       bin2bcd(dt.month()),
                       bin2bcd(dt.year() - 2000U)};
  //i2c_dev->write(buffer, 8);
  S_RTC_TIME_DATA_T srtcData;
  srtcData.u32Year = (uint32_t)(dt.year()); 
  srtcData.u32Month = (uint32_t)(dt.month()); 
  srtcData.u32Day = (uint32_t)(dt.day());
  srtcData.u32Hour  = (uint32_t)(dt.hour());
  srtcData.u32Minute = (uint32_t)(dt.minute());
  srtcData.u32Second = (uint32_t)(dt.second());


  RTC_SetDateAndTime(&srtcData);
}

/**************************************************************************/
/*!
    @brief  Get the current date and time
    @return DateTime object containing the current date and time
*/
/**************************************************************************/
DateTime RTC_M467::now() {
  uint8_t buffer[7];
  
  S_RTC_TIME_DATA_T srtcData;
  //i2c_dev->write_then_read(buffer, 1, buffer, 7);

  RTC_GetDateAndTime(&srtcData);
  
  buffer[6] = (uint8_t)(srtcData.u32Year - 2000);
  buffer[5] = (uint8_t)(srtcData.u32Month);
  buffer[4] = (uint8_t)(srtcData.u32Day);
  buffer[2] = (uint8_t)(srtcData.u32Hour);
  buffer[1] = (uint8_t)(srtcData.u32Minute);
  buffer[0] = (uint8_t)(srtcData.u32Second);

  return DateTime((buffer[6]) + 2000U, (buffer[5]),
                  (buffer[4]), (buffer[2]), (buffer[1]),
                  (buffer[0] & 0x7F));
}

void RTC_M467::setAlarm(const DateTime &dt) {
  uint8_t buffer[8] = {0,
                       bin2bcd(dt.second()),
                       bin2bcd(dt.minute()),
                       bin2bcd(dt.hour()),
                       0,
                       bin2bcd(dt.day()),
                       bin2bcd(dt.month()),
                       bin2bcd(dt.year() - 2000U)};
  //i2c_dev->write(buffer, 8);
  S_RTC_TIME_DATA_T srtcData;
  srtcData.u32Year = (uint32_t)(dt.year()); 
  srtcData.u32Month = (uint32_t)(dt.month()); 
  srtcData.u32Day = (uint32_t)(dt.day());
  srtcData.u32Hour  = (uint32_t)(dt.hour());
  srtcData.u32Minute = (uint32_t)(dt.minute());
  srtcData.u32Second = (uint32_t)(dt.second());


  RTC_SetAlarmDateAndTime(&srtcData);

  /* Clear RTC alarm interrupt flag */
  RTC_CLEAR_ALARM_INT_FLAG();

  /* Enable RTC Alarm Interrupt */
  RTC_EnableInt(RTC_INTEN_ALMIEN_Msk);
  NVIC_EnableIRQ(RTC_IRQn);
}

void RTC_M467::setAlarmCallback(void (*callback)(void), uint32_t mode)
{
    if( callback == NULL )
    {
        RTC_DisableInt(RTC_INTEN_ALMIEN_Msk);
        NVIC_DisableIRQ(RTC_IRQn);
        return;
    } 

    if( mode )
     callbacksRTC = callback;
    else
     callbacksRTC = NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

void RTC_IRQHandler(void)
{
    /* To check if RTC alarm interrupt occurred */
    if(RTC_GET_ALARM_INT_FLAG() == 1)
    {
        /* Clear RTC alarm interrupt flag */
        RTC_CLEAR_ALARM_INT_FLAG();

        if (callbacksRTC!=NULL) callbacksRTC();	  	
    }

    if(RTC_GET_TICK_INT_FLAG() == 1)
    {
        /* Clear RTC tick interrupt flag */
        RTC_CLEAR_TICK_INT_FLAG();
    }

}

#ifdef __cplusplus
}
#endif