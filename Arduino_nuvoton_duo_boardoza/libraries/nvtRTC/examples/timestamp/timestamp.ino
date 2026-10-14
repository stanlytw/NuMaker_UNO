/* Timestamp functions using a M467 RTC
**
** Useful for file name
**		` SD.open(time.timestamp()+".log", FILE_WRITE) `
**
**
** Created: 2015-06-01 by AxelTB
** Last Edit:
*/

#include "RTClib.h"

RTC_M467 rtc;

void setup () {
  Serial.begin(115200);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  Serial.println("Set the time!");
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime("May 15 2025","12:03:20"));
  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  //rtc.adjust(DateTime(2014U, 1U, 21U, 3U, 0U, 0U));
}

void loop() {
 DateTime time = rtc.now();

 //Full Timestamp
 Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));

 //Date Only
 Serial.println(String("DateTime::TIMESTAMP_DATE:\t")+time.timestamp(DateTime::TIMESTAMP_DATE));

 //Time Only
 Serial.println(String("DateTime::TIMESTAMP_TIME:\t")+time.timestamp(DateTime::TIMESTAMP_TIME));

 Serial.println("\n");

 //Delay 1s
 delay(1000);
}
