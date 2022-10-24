/*----------------------------------------------------------------------------*/
/* This file implement RF TX Power Register Settings                          */
/*----------------------------------------------------------------------------*/
// #include "mcu_definition.h"
#include "Arduino.h"
#include "porting_rfpower.h"

/** BLE TX power level definition
 * @attention  1. BLE TX power level table can be modified but please noted that the default TX power setting is set to index 0. \n
 *             2. BLE channel 36/39 TX power can be modified by setting different CH36_idx/ CH39_idx index. \n
 *                i.e., default TX power is set to index 0 and reduce BLE channel 36/39 maximum TX power to stage 3.
 *                          default stage 0  : {0xC9,  0x87,  0xBC,  0x1A,  0x20,   0},
 *                      --> modified stage 0 : {0xC9,  0x87,  0xBC,  0x1A,  0x20,   3},
*/

#if (TXPOWER == TXPOWER_HP_MATCHING)

const int8_t TXPOWER_TABLE_MATCHING[][6] =
{
    //R22   R23    R24    R25    R26  CH36_idx/ CH39_idx
    {0xC9,  0x87,  0xBC,  0x1A,  0x20,   0},   /**<  stage 0, Default Tx power is set to index 0. */
    {0xC9,  0x82,  0xBC,  0x9A,  0x20,   1},   /**<  stage 1. */
    {0xCB,  0x93,  0xAB,  0x7C,  0x27,   2},   /**<  stage 2. */
    {0xCB,  0x8E,  0xAB,  0xFC,  0x27,   3},   /**<  stage 3. */
    {0xCB,  0x9E,  0xAB,  0xFC,  0x27,   4},   /**<  stage 4. */
    {0xCB,  0x8A,  0x8A,  0xDA,  0x2D,   5},   /**<  stage 5. */
    {0xCB,  0x87,  0x8A,  0x5E,  0x3F,   6},   /**<  stage 6. */
    {0xCB,  0x82,  0xAA,  0xFE,  0x2F,   7},   /**<  stage 7. */
    {0xCB,  0x8D,  0xA9,  0xBC,  0x2B,   8},   /**<  stage 8  */
    {0xCB,  0x86,  0xA8,  0x3E,  0x2B,   9},   /**<  stage 9. */
    {0xCB,  0x8B,  0xA4,  0xFE,  0x2F,  10},   /**<  stage 10. */
    {0xCB,  0x9A,  0xA4,  0xFE,  0x2A,  11},   /**<  stage 11. */
    {0xCB,  0x92,  0xA4,  0xBE,  0x3A,  12},   /**<  stage 12. */
    {0xCB,  0x52,  0xA4,  0xBE,  0x3A,  13},   /**<  stage 13. */
    {0xCB,  0x4E,  0xA4,  0xFE,  0x3E,  14},   /**<  stage 14. */
    {0xCB,  0x13,  0xA4,  0xBE,  0x3E,  15},   /**<  stage 15. */
    {0xCB,  0x40,  0xB1,  0x7E,  0x2F,  16},   /**<  stage 16. */
    {0xCB,  0x44,  0xB1,  0xFC,  0x3F,  17},   /**<  stage 17. */
    {0xCB,  0x48,  0xB1,  0xFE,  0x3F,  18},   /**<  stage 18. */
    {0xCB,  0x48,  0xB0,  0xFE,  0x2F,  19},   /**<  stage 19. */
    {0xCB,  0x4C,  0xB0,  0x3E,  0x3F,  20},   /**<  stage 20. */
    {0xCB,  0x00,  0xB0,  0x2A,  0x3E,  21},   /**<  stage 21. */
    {0xCB,  0x20,  0xB0,  0xCE,  0x3F,  22},   /**<  stage 22. */
    {0xC9,  0x87,  0xAC,  0x1A,  0x10,  23},   /**<  stage EXT1. */
    {0xC9,  0x87,  0x9C,  0x1A,  0x00,  24},   /**<  stage EXT2. */
    {0xC9,  0x87,  0x8C,  0x30,  0x00,  25},   /**<  stage EXT3. */
};

/** BLE TX power level table definition */
const int8_t (*TXPOWER_TABLE)[6] = TXPOWER_TABLE_MATCHING;

/** The size of BLE TX power level table definition */
uint8_t size_TXPOWER_TABLE = sizeof(TXPOWER_TABLE_MATCHING) / sizeof(TXPOWER_TABLE_MATCHING[0]);;


#elif (TXPOWER == TXPOWER_LP_MATCHING)

const int8_t TXPOWER_TABLE_MATCHING[][6] =
{
    //R22   R23    R24    R25    R26  CH36_idx/ CH39_idx
    {0xC9,  0x97,  0x85,  0xEA,  0x20,   0},   /**<   stage 0, Default Tx power is set to index 0. */
    {0xC9,  0x93,  0x85,  0xEA,  0x30,   1},   /**<   stage 1. */
    {0xC9,  0x93,  0x85,  0xEC,  0x31,   2},   /**<   stage 2. */
    {0xC9,  0x92,  0x85,  0xAC,  0x31,   3},   /**<   stage 3. */
    {0xC9,  0x96,  0x85,  0xEE,  0x31,   4},   /**<   stage 4. */
    {0xC9,  0x8F,  0x90,  0x6E,  0x32,   5},   /**<   stage 5. */
    {0xC9,  0x82,  0x90,  0x2E,  0x32,   6},   /**<   stage 6. */
    {0xC9,  0x82,  0xA0,  0xEE,  0x32,   7},   /**<   stage 7. */
    {0xC9,  0x81,  0xA0,  0x2E,  0x32,   8},   /**<   stage 8. */
    {0xC9,  0x81,  0xA0,  0x2E,  0x38,   9},   /**<   stage 9. */
    {0xC9,  0x81,  0xA0,  0x2E,  0x3A,  10},   /**<   stage 10. */
    {0xC9,  0x81,  0xA0,  0xEE,  0x3C,  11},   /**<   stage 11. */
    {0xC9,  0x81,  0xA0,  0xEE,  0x3E,  12},   /**<   stage 12. */
    {0xC9,  0x81,  0xB0,  0xAE,  0x3F,  13},   /**<   stage 13. */
    {0xC9,  0x41,  0xB0,  0xAE,  0x3F,  14},   /**<   stage 14. */
    {0xCA,  0x41,  0xB0,  0xAE,  0x3D,  15},   /**<   stage 15. */
    {0xCB,  0x01,  0xB0,  0xAE,  0x3E,  16},   /**<   stage 16. */
    {0xCB,  0x0C,  0xB0,  0xCE,  0x3F,  17},   /**<   stage 17. */
};

/** BLE TX power level table definition */
const int8_t (*TXPOWER_TABLE)[6] = TXPOWER_TABLE_MATCHING;

/** The size of BLE TX power level table definition */
uint8_t size_TXPOWER_TABLE = sizeof(TXPOWER_TABLE_MATCHING) / sizeof(TXPOWER_TABLE_MATCHING[0]);
#endif

