
#define CANFD_NVT

#ifdef CANFD_NVT
#include "nvtCAN_m460.h"
nvtCAN_m460 CAN(0); // Set nvt's parameter, if required.
#endif

#define SERIAL_PORT_MONITOR Serial

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while(!SERIAL_PORT_MONITOR){};

    
    while (CAN_OK != CAN.begin(CAN_BAUDRATE_100KBPS)) {             // init can bus : baudrate = 100k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0x30};
void loop() {
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    stmp[7] = stmp[7] + 1;
    if (stmp[7] == 100) {
        stmp[7] = 0;
        stmp[6] = stmp[6] + 1;

        if (stmp[6] == 100) {
            stmp[6] = 0;
            stmp[5] = stmp[5] + 1;
        }
    }

    CAN.sendMsgBuf(0x33, 0, 8, stmp);
    delay(100);                       // send data per 100ms
    SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok!");
}

// END FILE
