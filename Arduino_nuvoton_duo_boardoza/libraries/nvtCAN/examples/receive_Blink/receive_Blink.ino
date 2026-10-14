

#define CANFD_NVT


#ifdef CANFD_NVT
#include "nvtCAN_m460.h"
nvtCAN_m460 CAN(0); // Set nvt's parameter, if required.
#endif
#define SERIAL_PORT_MONITOR Serial

const int LED        = 8;
boolean ledON        = 1;

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    pinMode(LED, OUTPUT);

    while (CAN_OK != CAN.begin(CAN_BAUDRATE_100KBPS)) {             // init can bus : baudrate = 100k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}


void loop() {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        SERIAL_PORT_MONITOR.println("-----------------------------");
        SERIAL_PORT_MONITOR.println("get data from ID: 0x");
        SERIAL_PORT_MONITOR.println(canId, HEX);

        for (int i = 0; i < len; i++) { // print the data
            SERIAL_PORT_MONITOR.print(buf[i]);
            SERIAL_PORT_MONITOR.print("\t");
            if (ledON && i == 0) {

                digitalWrite(LED, buf[i]);
                ledON = 0;
                delay(500);
            } else if ((!(ledON)) && i == 4) {

                digitalWrite(LED, buf[i]);
                ledON = 1;
            }
        }
        SERIAL_PORT_MONITOR.println();
    }
    delay(10);
}

//END FILE
