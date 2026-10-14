/*
  SD card test

 This example shows how use the utility libraries on which the'
 SD library is based in order to get info about your SD card.
 Very useful for testing a card when you're not sure whether its working or not.

  * SD card attached

 */
// include the SD library:
#include <NVTSD.h>

//Call 'card.init()' without parameter.
//Skip SD_DETECT_PIN and ignore it. 

Sd2Card card;
SdFatFs fatFs;
char        *ptr;
char Line[256]; 
void setup() {
  bool disp = false;
  uint8_t n = 0;
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial)
    ;
  Serial.println("\nInitialize SD card...");
  
  do
  {
    delay(500);
    if (!disp) {
      Serial.println("Wait initializing..");
      disp = true;
    }
    
    if (n<10) 
        n++;
    else 
        break;
  }while (!card.init());

  Serial.println("card initialized."); 
   
  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!fatFs.init()) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }

  // print the type and size of the first FAT-type volume
  uint64_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(fatFs.fatType(), DEC);
  Serial.println();

  volumesize = fatFs.blocksPerCluster();  // clusters are collections of blocks
  volumesize *= fatFs.clusterCount();     // we'll have a lot of clusters
  volumesize *= 512;                      // SD card blocks are always 512 bytes
  
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.print((uint32_t)(volumesize));
  Serial.println();
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.print((uint32_t)(volumesize));
  Serial.println();

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  
  delay(1000);

  Serial.print("CardInfo Example...");

}

void loop(void) {
 
  File root = SD.openRoot();
  delay(1000);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  
  root.close();
  if (!fatFs.deinit()) {
    Serial.println("Failed to deinit card");
  }
  if (!card.deinit()) {
    Serial.println("Failed to deinit card");
  }

  Serial.println("###### End of the SD tests ######");
  
  while(1);
}
