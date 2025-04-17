/*
  Listfiles

  This example shows how print out the files in a
  directory on a SD card

  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 2 Feb 2014
  by Scott Fitzgerald

  This example code is in the public domain.

*/
#include <NVTSD.h>

File root;
void printDirectory(File dir, int numTabs);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("********Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("********initialization done.");

  
  root = SD.open("/");
  if (root._res==FR_OK) 
  {
       printDirectory(root, 0);
       root.close();
  }
  else
  {
    	 printf("Could not open root\r\n");
  }
  
  if (!SD.end()) {
    Serial.println("Failed to properly end the SD.");
  }
  Serial.println("End of the SD listfiles");
  
  delay(1000);

  Serial.print("listfiles Example...");
}

  


void loop() {
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if ((entry._res!=FR_OK)) {
      // no more files
      dir.rewindDirectory();//
      break;
    }
    else
    {
      for (uint8_t i = 0; i < numTabs; i++)
    	{
    	  Serial.print("\t");
    	}

      if (entry.isDirectory()) 
      {
        Serial.print("/");
        Serial.print(entry.name());
        Serial.println("  ");
        printDirectory(entry, numTabs + 1);
      } 
      else 
      {
      
        /*
           files have sizes, directories do not
        */
    	  Serial.print(entry.name());
        Serial.print(" ");
        Serial.println(entry.size(), DEC);
      }
    }
    
   
    entry.close();
  }
}



