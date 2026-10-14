


#define CANFD_NVT


#ifdef CANFD_NVT
#include "nvtCAN_m460.h"
nvtCAN_m460 CAN(0); // Set nvt's parameter, if required.
#define MAX_DATA_SIZE 8
#endif

#define SERIAL_PORT_MONITOR Serial

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while (!SERIAL_PORT_MONITOR) {}

    while (CAN_OK != CAN.begin(CAN_BAUDRATE_100KBPS)) {             // init can bus : baudrate = 100k
        SERIAL_PORT_MONITOR.println(F("CAN init fail, retry..."));
        delay(100);
    }
    SERIAL_PORT_MONITOR.println(F("CAN init ok!"));
}

uint32_t id;
uint8_t  type; // bit0: ext, bit1: rtr
uint8_t  len;
byte cdata[MAX_DATA_SIZE] = {0};

void loop() {
    // check if data coming
    if (CAN_MSGAVAIL != CAN.checkReceive()) {
        return;
    }

    char prbuf[32 + MAX_DATA_SIZE * 3];
    int i, n;

    unsigned long t = millis();
    // read data, len: data length, buf: data buf
    CAN.readMsgBuf(&len, cdata);

    id = CAN.getCanId();
    type = (CAN.isExtendedFrame() << 0) |
           (CAN.isRemoteRequest() << 1);
    /*
     * MCP2515(or this driver) could not handle properly
     * the data carried by remote frame
     */

    n = sprintf(prbuf, "%04lu.%03d ", t / 1000, int(t % 1000));
    /* Displayed type:
     *
     * 0x00: standard data frame
     * 0x02: extended data frame
     * 0x30: standard remote frame
     * 0x32: extended remote frame
     */
    static const byte type2[] = {0x00, 0x02, 0x30, 0x32};
    n += sprintf(prbuf + n, "RX: [%08lX](%02X) ", (unsigned long)id, type2[type]);
    // n += sprintf(prbuf, "RX: [%08lX](%02X) ", id, type);

    for (i = 0; i < len; i++) {
        n += sprintf(prbuf + n, "%02X ", cdata[i]);
    }
    SERIAL_PORT_MONITOR.println(prbuf);

    delay(100);
}
// END FILE
