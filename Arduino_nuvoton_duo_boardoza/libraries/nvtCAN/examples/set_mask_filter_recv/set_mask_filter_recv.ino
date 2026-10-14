
#define CANFD_NVT

#ifdef CANFD_NVT
#include "nvtCAN_m460.h"
nvtCAN_m460 CAN(0); // Set nvt's parameter, if required.
#endif                        
#define SERIAL_PORT_MONITOR Serial

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while(!SERIAL_PORT_MONITOR){};
    
    while (CAN_OK != CAN.begin(CAN_BAUDRATE_100KBPS)) {             // init can bus : baudrate = 100k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");

    /*
        set mask, set both the mask to 0x3ff
    */
	// kendof dummy, keep for compatibility of mcp2515 sample, 
	// init_Mask here configure GFC to reject all unmatched Msg, 
	// since we try to set Msg Id filter
    CAN.init_Mask(0, 0, 0x3ff);                         
    CAN.init_Mask(1, 0, 0x3ff);


    /*
        set filter, we can receive id from 0x04 ~ 0x09
    */
    CAN.init_Filt(0, 0, 0x04);                          // there are 6 filter in mcp2515
    CAN.init_Filt(1, 0, 0x05);                          // there are 6 filter in mcp2515

    CAN.init_Filt(2, 0, 0x06);                          // there are 6 filter in mcp2515
    CAN.init_Filt(3, 0, 0x07);                          // there are 6 filter in mcp2515
    CAN.init_Filt(4, 0, 0x08);                          // there are 6 filter in mcp2515
    CAN.init_Filt(5, 0, 0x09);                          // there are 6 filter in mcp2515
}


void loop() {
    
  if (CAN_MSGAVAIL == CAN.checkReceive()) 
  {
      CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

      SERIAL_PORT_MONITOR.println("\r\n------------------------------------------------------------------");
      SERIAL_PORT_MONITOR.print("Get Data From id: 0x");
      SERIAL_PORT_MONITOR.println(CAN.getCanId(), HEX);
      for (int i = 0; i < len; i++) { // print the data
         SERIAL_PORT_MONITOR.print("0x");
         SERIAL_PORT_MONITOR.print(buf[i], HEX);
         SERIAL_PORT_MONITOR.print("\t");
      }
      SERIAL_PORT_MONITOR.println();
  }
  delay(100);
    
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/