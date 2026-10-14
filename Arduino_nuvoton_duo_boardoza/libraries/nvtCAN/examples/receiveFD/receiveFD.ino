
#define CANFD_NVT

#ifdef CANFD_NVT
#include "nvtCAN_m460.h"
nvtCAN_m460 CAN_RECEIVE(0); // Set nvt's parameter, if required.
#endif
#define SERIAL_PORT_MONITOR Serial


#define MAX_DATA_SIZE 64

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while (!SERIAL_PORT_MONITOR) {} // wait for Serial

    /*
     * To compatible with MCP2515 API,
     * default mode is CAN_CLASSIC_MODE
     * Now set to CANFD mode.
     */
    CAN_RECEIVE.setMode(CANFD_OP_CAN_FD_MODE);
    
    if (CAN_RECEIVE.begin(CAN_BAUDRATE_100K_400K) != 0) {
        SERIAL_PORT_MONITOR.println("CAN-BUS initiliased error!");
        while (1);
    }
    byte receive_mode = CAN_RECEIVE.getMode();
    SERIAL_PORT_MONITOR.print("CAN BUS Receive Mode = "); SERIAL_PORT_MONITOR.println(receive_mode);
    SERIAL_PORT_MONITOR.println("CAN BUS Shield init ok!");
}

unsigned char stmp[MAX_DATA_SIZE] = {0};
unsigned char len = 0;
unsigned char buf[MAX_DATA_SIZE];

void loop() {
    
    // ---------------------
    if (CAN_MSGAVAIL == CAN_RECEIVE.checkReceive()) {
        // read data,  len: data length, buf: data buf
        SERIAL_PORT_MONITOR.println("checkReceive");
        CAN_RECEIVE.readMsgBuf(&len, buf);
        // print the data
        for (int i = 0; i < len; i++) {
            SERIAL_PORT_MONITOR.print(buf[i]);
            SERIAL_PORT_MONITOR.print(' ');
        }
        SERIAL_PORT_MONITOR.println();
    }
    SERIAL_PORT_MONITOR.println("---LOOP END---");
}

// END FILE

