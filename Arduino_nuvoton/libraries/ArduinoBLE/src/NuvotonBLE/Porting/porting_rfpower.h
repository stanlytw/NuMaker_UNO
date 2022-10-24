#ifndef _BLE_RF_POWER_DEFINITION_H_
#define _BLE_RF_POWER_DEFINITION_H_
/**************************************************************************//**
 * @file       porting_rfpower.h
 * @brief
 *
 *
 * @defgroup rfPowerDef RF Power Definition
 * @{
 * @ingroup mcuPortingDef
 * @details RF Power definition.
 * @}
 **************************************************************************/

/**
 * @ingroup rfPowerDef
 * @defgroup rfPowerTableDef RF Power Table Definition
 * @{
 * @brief Defined supported RF power table.
 */
#define TXPOWER_HP_MATCHING    0           /**< Maximum supports to +10 dBm. */
#define TXPOWER_LP_MATCHING    1           /**< Maximum supports to +5 dBm. */
/** @} */


/** Selected a RF power table from @ref rfPowerTableDef "RF power table list".
 * @ingroup rfPowerDef
 * @{
*/
#define TXPOWER TXPOWER_HP_MATCHING


/** BLE TX power level table
 * @ingroup rfPowerDef
 * @attention  BLE TX power level table can be modified but please noted that the default TX power setting is set to index 0. \n
 * @note  @ref TXPOWER_TABLE is defined in "porting_rfpower.c.
*/
extern const int8_t (*TXPOWER_HV_TABLE)[6];


/** The size of defined BLE TX power level table
 * @ingroup rfPowerDef
 * @note  @ref TXPOWER_TABLE is defined in "porting_rfpower.c.
*/
extern uint8_t size_TXPOWER_TABLE;


/** Define Tx Power Level Mapping Index for user uses.
 * @enum TxPowerLevel
 * @note The index is mapping to the TX power level table, and the default Tx power is set to the index "0" of "TXPOWER_HV_TABLE".
*/
#if (TXPOWER == TXPOWER_HP_MATCHING)
typedef enum txPowerLevel
{
    TXPOWER_10DBM = 0, /**< Default Setting is index 0 = TX power is +10dBm (mapping to "TX power level table" index 0). */
    TXPOWER_9DBM,      /**< TX power is +9dBm (mapping to "TX power level table" index 1). */
    TXPOWER_8DBM,      /**< TX power is +8dBm (mapping to "TX power level table" index 2). */
    TXPOWER_7DBM,      /**< TX power is +7dBm (mapping to "TX power level table" index 3). */
    TXPOWER_6DBM,      /**< TX power is +6dBm (mapping to "TX power level table" index 4). */
    TXPOWER_5DBM,      /**< TX power is +5dBm (mapping to "TX power level table" index 5). */
    TXPOWER_4DBM,      /**< TX power is +4dBm (mapping to "TX power level table" index 6). */
    TXPOWER_3DBM,      /**< TX power is +3dBm (mapping to "TX power level table" index 7). */
    TXPOWER_2DBM,      /**< TX power is +2dBm (mapping to "TX power level table" index 8). */
    TXPOWER_1DBM,      /**< TX power is +1dBm (mapping to "TX power level table" index 9). */
    TXPOWER_0DBM,      /**< TX power is +0dBm (mapping to "TX power level table" index 10). */
    TXPOWER_m1DBM,     /**< TX power is -1dBm (mapping to "TX power level table" index 11). */
    TXPOWER_m2DBM,     /**< TX power is -2dBm (mapping to "TX power level table" index 12). */
    TXPOWER_m3DBM,     /**< TX power is -3dBm (mapping to "TX power level table" index 13). */
    TXPOWER_m4DBM,     /**< TX power is -4dBm (mapping to "TX power level table" index 14). */
    TXPOWER_m5DBM,     /**< TX power is -5dBm (mapping to "TX power level table" index 15). */
    TXPOWER_m6DBM,     /**< TX power is -6dBm (mapping to "TX power level table" index 16). */
    TXPOWER_m7DBM,     /**< TX power is -7dBm (mapping to "TX power level table" index 17). */
    TXPOWER_m8DBM,     /**< TX power is -8dBm (mapping to "TX power level table" index 18). */
    TXPOWER_m9DBM,     /**< TX power is -9dBm (mapping to "TX power level table" index 19). */
    TXPOWER_m10DBM,    /**< TX power is -10dBm (mapping to "TX power level table" index 20). */
    TXPOWER_m15DBM,    /**< TX power is -15dBm (mapping to "TX power level table" index 21). */
    TXPOWER_m20DBM,    /**< TX power is -20dBm (mapping to "TX power level table" index 22). */
    TXPOWER_EXT1,      /**< TX power is EXT1 (mapping to "TX power level table" index 23). */
    TXPOWER_EXT2,      /**< TX power is EXT2 (mapping to "TX power level table" index 24). */
    TXPOWER_EXT3,      /**< TX power is EXT3 (mapping to "TX power level table" index 25). */
} TxPowerLevel;
#elif (TXPOWER == TXPOWER_LP_MATCHING)
typedef enum txPowerLevel
{
    TXPOWER_5DBM = 0,  /**< Default Setting is index 0 = TX power is +4dBm (mapping to "TX power level table" index 0). */
    TXPOWER_4DBM,      /**< TX power is +4dBm (mapping to "TX power level table" index 1). */
    TXPOWER_3DBM,      /**< TX power is +3dBm (mapping to "TX power level table" index 2). */
    TXPOWER_2DBM,      /**< TX power is +2dBm (mapping to "TX power level table" index 3). */
    TXPOWER_1DBM,      /**< TX power is +1dBm (mapping to "TX power level table" index 4). */
    TXPOWER_0DBM,      /**< TX power is +0dBm (mapping to "TX power level table" index 5). */
    TXPOWER_m1DBM,     /**< TX power is -1dBm (mapping to "TX power level table" index 6). */
    TXPOWER_m2DBM,     /**< TX power is -2dBm (mapping to "TX power level table" index 7). */
    TXPOWER_m3DBM,     /**< TX power is -3dBm (mapping to "TX power level table" index 8). */
    TXPOWER_m4DBM,     /**< TX power is -4dBm (mapping to "TX power level table" index 9). */
    TXPOWER_m5DBM,     /**< TX power is -5dBm (mapping to "TX power level table" index 10). */
    TXPOWER_m6DBM,     /**< TX power is -6dBm (mapping to "TX power level table" index 11). */
    TXPOWER_m7DBM,     /**< TX power is -7dBm (mapping to "TX power level table" index 12). */
    TXPOWER_m8DBM,     /**< TX power is -8dBm (mapping to "TX power level table" index 13). */
    TXPOWER_m9DBM,     /**< TX power is -9dBm (mapping to "TX power level table" index 14). */
    TXPOWER_m10DBM,    /**< TX power is -10dBm (mapping to "TX power level table" index 15). */
    TXPOWER_m15DBM,    /**< TX power is -15dBm (mapping to "TX power level table" index 16). */
    TXPOWER_m20DBM,    /**< TX power is -20dBm (mapping to "TX power level table" index 17). */
} TxPowerLevel;
#endif
/** @} */ // (@ingroup rfPowerTableDef)


#endif // (_BLE_RF_POWER_DEFINITION_H_)
