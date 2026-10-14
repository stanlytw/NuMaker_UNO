/*************************************************************************************************
    OBD-II_PIDs TEST CODE
    LOOVEE @ JUN24, 2017

    Query
    send id: 0x7df
      dta: 0x02, 0x01, PID_CODE, 0, 0, 0, 0, 0

    Response
    From id: 0x7E9 or 0x7EA or 0x7EB
      dta: len, 0x41, PID_CODE, byte0, byte1(option), byte2(option), byte3(option), byte4(option)

    https://en.wikipedia.org/wiki/OBD-II_PIDs

    Input a PID, then you will get reponse from vehicle, the input should be end with '\n'
***************************************************************************************************/
#define CANFD_NVT

#ifdef CANFD_NVT
#include "nvtCAN_m460.h"
nvtCAN_m460 CAN(0); // Set nvt's parameter, if required.
#endif

#define SERIAL_PORT_MONITOR Serial

#define PID_ENGIN_PRM       0x0C
#define PID_VEHICLE_SPEED   0x0D
#define PID_COOLANT_TEMP    0x05

#define CAN_ID_PID          0x7DF

unsigned char PID_INPUT;
unsigned char getPid    = 0;

void set_mask_filt() {
    /*
        set mask, set both the mask to 0x3ff
    */
    CAN.init_Mask(0, 0, 0x7FC);
    CAN.init_Mask(1, 0, 0x7FC);

    /*
        set filter, we can receive id from 0x04 ~ 0x09
    */
    CAN.init_Filt(0, 0, 0x7E8);
    CAN.init_Filt(1, 0, 0x7E8);

    CAN.init_Filt(2, 0, 0x7E8);
    CAN.init_Filt(3, 0, 0x7E8);
    CAN.init_Filt(4, 0, 0x7E8);
    CAN.init_Filt(5, 0, 0x7E8);
}

void sendPid(unsigned char __pid) {
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};
    SERIAL_PORT_MONITOR.print("SEND PID: 0x");
    SERIAL_PORT_MONITOR.println(__pid, HEX);
    CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
}

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while(!Serial){};

    while (CAN_OK != CAN.begin(CAN_BAUDRATE_100KBPS)) {             // init can bus : baudrate = 100k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
    set_mask_filt();
}


void loop() {
    taskCanRecv();
    taskDbg();

    if (getPid) {       // GET A PID
        getPid = 0;
        sendPid(PID_INPUT);
        PID_INPUT = 0;
    }
}

void taskCanRecv() {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {                // check if get data
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
}

void taskDbg() {
    while (SERIAL_PORT_MONITOR.available()) {
        char c = SERIAL_PORT_MONITOR.read();

        if (c >= '0' && c <= '9') {
            PID_INPUT *= 0x10;
            PID_INPUT += c - '0';

        } else if (c >= 'A' && c <= 'F') {
            PID_INPUT *= 0x10;
            PID_INPUT += 10 + c - 'A';
        } else if (c >= 'a' && c <= 'f') {
            PID_INPUT *= 0x10;
            PID_INPUT += 10 + c - 'a';
        } else if (c == '\n') { // END
            getPid = 1;
        }
    }
}
// END FILE
