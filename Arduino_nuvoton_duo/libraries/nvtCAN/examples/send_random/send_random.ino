
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
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");

    randomSeed(millis());
}

uint32_t id;
uint8_t  type; // bit0: ext, bit1: rtr
unsigned len;
byte cdata[MAX_DATA_SIZE] = {0};

void loop() {
    type = random(4);
    if (type & 0x1) {
        // total 29 bits
        // Arduino AVR only generate up to 16 bits random number
        id = random(0x1U << 14);
        id |= (uint32_t)random(0x1U << 15) << 14;
    } else {
        id = random(0x1U << 11);
    }
    if (type & 0x2) {
        len = 0;
        // remote frame could also carry data
        // but don't do it.
        // len = random(0, MAX_DATA_SIZE + 1);
    } else {
        len = random(0, MAX_DATA_SIZE + 1);
    }

    int i;
    for (i = 0; i < len; i++) {
        cdata[i] = random(0x100);
    }
    #if MAX_DATA_SIZE > 8
    // pad CANFD extra bytes with 0
    for (i = len; i < MAX_DATA_SIZE; i++) {
        cdata[i] = 0;
    }
    #endif

    CAN.sendMsgBuf(id, bool(type & 0x1),
                       bool(type & 0x2),
                     #if MAX_DATA_SIZE > 8
                       CANFD::len2dlc(len),
                     #else
                       len,
                     #endif
                       cdata);

    char prbuf[32 + MAX_DATA_SIZE * 3];
    int n;

    /* Displayed type:
     *
     * 0x00: standard data frame
     * 0x02: extended data frame
     * 0x30: standard remote frame
     * 0x32: extended remote frame
     */
    static const byte type2[] = {0x00, 0x02, 0x30, 0x32};
    n = sprintf(prbuf, "TX: [%08lX](%02X) ", (unsigned long)id, type2[type]);
    // n = sprintf(prbuf, "TX: [%08lX](%02X) ", id, type);

    for (i = 0; i < len; i++) {
        n += sprintf(prbuf + n, "%02X ", cdata[i]);
    }
    SERIAL_PORT_MONITOR.println(prbuf);

    unsigned d = random(30);
    SERIAL_PORT_MONITOR.println(d);
    delay(d);
}
// END FILE
