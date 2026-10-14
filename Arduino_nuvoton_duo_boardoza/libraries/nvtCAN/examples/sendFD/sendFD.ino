
#define CANFD_NVT

#ifdef CANFD_NVT
#include "nvtCAN_m460.h"
nvtCAN_m460 CAN(0); // Set nvt's parameter, if required.
#endif
#define SERIAL_PORT_MONITOR Serial

// CANFD could carry data up to 64 bytes
#define MAX_DATA_SIZE 64

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while (!SERIAL_PORT_MONITOR) {}

    /*
     * nvtCAN defauat to run CAN mode.
	 * In this sendFD eample, set to CANFD mode.
     */
    CAN.setMode(CANFD_OP_CAN_FD_MODE);

    // init can bus : arbitration bitrate = 100k, data bitrate = 400K
    while (0 != CAN.begin(CAN_BAUDRATE_100K_400K)) {
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");

    byte mode = CAN.getMode();
    SERIAL_PORT_MONITOR.print("CAN mode = ");
    SERIAL_PORT_MONITOR.println(mode);
}

unsigned char stmp[MAX_DATA_SIZE] = {0};
void loop() {
    // send data:  id = 0x00, standrad frame, data len = 64, stmp: data buf
    stmp[MAX_DATA_SIZE - 1] = stmp[MAX_DATA_SIZE - 1] + 1;
    if (stmp[MAX_DATA_SIZE - 1] == 100) {
        stmp[MAX_DATA_SIZE - 1] = 0;

        stmp[MAX_DATA_SIZE - 2] = stmp[MAX_DATA_SIZE - 2] + 1;
        if (stmp[MAX_DATA_SIZE - 2] == 100) {
            stmp[MAX_DATA_SIZE - 2] = 0;

            stmp[MAX_DATA_SIZE - 3] = stmp[MAX_DATA_SIZE - 3] + 1;
        }
    }

    CAN.sendMsgBuf(0x00, 0, MAX_DATA_SIZE, stmp, IS_FD);
    delay(100);                       // send data per 100ms
    SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok!");
}

// END FILE
