/*
  Dancing LED
  Turns on an LED1,LED2 and LED3 on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
  LED1(D7),  It work for UNO-M4.
  LED2(D8),  It work for UNO-M4.
  LED3(D11), It work for UNO-M4.
    
  Note: This sample does not support NuEdu-NUC131.
 */



#define lenPin 3
int led[]={D7,D8,D11};  /* LED1,LED2,LED3 */

void setup() {
  int thisPin;
  for (thisPin =0; thisPin < lenPin; thisPin++) { 
    pinMode(led[thisPin], OUTPUT); 
  }
}

void loop() {
  // iterate over the pins:
  for (int thisPin =0; thisPin < lenPin; thisPin++) { 
    //The LEDs on thisPin from off to on:  
    digitalWrite(led[thisPin], HIGH);
    delay(100);
  }
  
  // iterate over the pins:
  for (int thisPin =0; thisPin < lenPin; thisPin++) { 
    //The LEDs on thisPin from on to off:
    digitalWrite(led[thisPin], LOW);
    delay(100);
  }  
}