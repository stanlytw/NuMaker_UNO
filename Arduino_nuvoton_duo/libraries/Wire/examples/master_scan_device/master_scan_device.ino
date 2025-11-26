// Demonstrates use of the Wire library
// Master scan what slave device is present.

// Created 8 August 2025

// This example code is in the public domain.


#include <Wire.h>
#define MAX_ADDR_7BIT  (0x7F)
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

uint8_t addr = 0x00;
uint8_t res;
void loop()
{
  res = Wire.isDevicePresent(addr); // ask slave to respond
  Serial.print("Device address 0x");
  Serial.print(addr, HEX);
  if ( res==1)
  {
      Serial.println(" is present");
      delay(500);
  }
  else
  {
      Serial.println(" is NOT present");
  }
  if( addr<MAX_ADDR_7BIT )
  {
      addr++;
  }
  else
  {    
      Serial.println(" Scan End. ");
	  while(1);    
  }
  delay(100);
}
