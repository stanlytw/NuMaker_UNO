/**************************************************************************//**
* @file       ble_cmd.h
* @brief      This file contains the functions of HOST to HCI interface.
*
*
* @defgroup ble_common BLE Common
* @{
* @details This file shows the common BLE definitions and functions. (ble_cmd.h, ble_event.h)
* @}
*****************************************************************************/

#ifndef _BLE_CMD_H_
#define _BLE_CMD_H_

#include "ble_stack_status.h"
#include "ble_gap.h"
/**************************************************************************
 * Definitions
 **************************************************************************/
/** @defgroup ble_cmd_definition BLE Command Definition
 * @{
 * @details Here shows the definitions in ble_cmd.h.
 * @ingroup ble_common
 * @}
 * @defgroup adv_definition BLE Advertising Definition
 * @{
 * @details Here shows the advertising related definition.
 * @ingroup ble_cmd_definition
 * @}
 * @defgroup scan_definition BLE Scan Definition
 * @{
 * @ingroup ble_cmd_definition
 * @details Here shows the scan related definition.
 * @}
 * @defgroup conn_definition BLE Connection Parameter Definition
 * @{
 * @ingroup ble_cmd_definition
 * @details Here shows the connection parameter related definition.
 * @}
 **************************************************************************/
/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_cmdEvent BLE Command Event Definition
 * @{
 * @details BLE Command event definition.
 * @name BleCmdEvent
 * @brief Define the different events that could be received by the @ref BleEventCallBack.
 * @{
 * @note The event parameter shall be ignored if the event status is not equal to @ref BLESTACK_STATUS_SUCCESS.
 */
typedef uint8_t BleCmdEvent;

/** Null event.*/
#define BLECMD_EVENT_NULL                                 0x00

/** Event indicates the advertisement is enabled or disabled.
 * @note The parameter field indicates the advertisement information though the @ref BLE_Event_AdvCompleteParam
*/
#define BLECMD_EVENT_ADV_COMPLETE                         0x01

/** Event indicates that receives the advertisement packets.
 * @note The parameter field indicates the received data though the @ref BLE_Event_ScanReportParam
*/
#define BLECMD_EVENT_SCAN_REPORT                          0x02

/** Event indicates the BLE connection is established.
 * @note The parameter field indicates the connection information though the @ref BLE_Event_ConnParam
*/
#define BLECMD_EVENT_CONN_COMPLETE                        0x03

/** Event indicates the BLE connection has been canceled.
 * @note The parameter field is always NULL
*/
#define BLECMD_EVENT_CONN_CANCEL_COMPLETE                 0x04


/** Event indicates the connection parameter update response can be retrieved.
 * @note The parameter field indicates the connection information though the
 * @ref ble_cmd_connUpdateStatus \n
 * @attention The event can be ignored, just wait for @ref BLECMD_EVENT_CONN_UPDATE_COMPLETE event
*/
#define BLECMD_EVENT_CONN_PARAMETER_UPDATE_RSP            0x05


/** Event indicates the BLE connection has been updated.
 * @note The parameter field indicates the connection information though the @ref BLE_Event_ConnUpdateParam
*/
#define BLECMD_EVENT_CONN_UPDATE_COMPLETE                 0x06

/** Event indicates the BLE connection has been disconnected.
 * @note The parameter field indicates the connection information though the @ref BLE_Event_DisconnParam
*/
#define BLECMD_EVENT_DISCONN_COMPLETE                     0x07

/** Event indicates that the Link Layer PHY update procedure is complete.
 * @note The parameter field indicates the Link Layer PHY information though the @ref BLE_Event_PhyUpdateParam
*/
#define BLECMD_EVENT_PHY_UPDATE_COMPLETE                  0x08

/** Event indicates the current Link Layer PHY can be retrieved.
 * @note The parameter field indicates the Link Layer PHY information though the @ref BLE_Event_PhyParam
*/
#define BLECMD_EVENT_PHY_READ_COMPLETE                    0x09

/** Event indicates the RSSI value can be retrieved.
 * @note The parameter field indicates the RSSI value though the @ref BLE_Event_RssiParam
*/
#define BLECMD_EVENT_READ_RSSI_COMPLETE                   0x0A

/** Event indicates the security STK generator method can be retrieved.
 * @note The parameter field indicates the security STK generator information though the @ref BLE_Event_StkGenMethodParam
*/
#define BLECMD_EVENT_STK_GEN_METHOD                       0x0B


/** Event indicates the passkey entry can be retrieved.
 * @note The parameter field indicates the passkey entry information though the @ref BLE_Event_PassKeyConfirmParam
*/
#define BLECMD_EVENT_PASSKEY_CONFIRM                      0x0C

/** Event indicates the authentication status can be retrieved.
 * @note The parameter field indicates the authentication status though the @ref BLE_Event_AuthStatusParam
*/
#define BLECMD_EVENT_AUTH_STATUS                          0x0D

/** Event indicates the scan is enabled or disabled. @note The parameter field is always NULL
*/
#define BLECMD_EVENT_SCAN_COMPLETE                        0x0E


/** Event indicates the connection is creating.
 * @note The parameter field indicates the connection information though the @ref BLE_Event_CreateConnParam
*/
#define BLECMD_EVENT_CREATE_CONNECTION                    0x0F

/** Event indicates the MTU size can be retrieved.
 * @note The parameter field indicates the MTU size though the @ref BLE_Event_MtuParam
*/
#define BLECMD_EVENT_EXCHANGE_MTU_SIZE                    0x10


/** Event indicates the updated data length can be retrieved.
 * @note The parameter field indicates the updated data length though the @ref BLE_Event_DataLengthParam
*/
#define BLECMD_EVENT_DATA_LENGTH_UPDATE                   0x11

/** Event indicates the host attribute database has been parsed.
 * @note The parameter field indicates the updated data length though the @ref BLE_Event_AttDbParsedParam
*/
#define BLECMD_EVENT_ATT_DATABASE_PARSING_FINISHED        0x12

/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_disconnReason BLE Disconnect Reason Definition
 * @{
 * @details BLE disconnect reason definition.
 * @name BleDisConnReason
 * @brief Define the BLE disconnect reason.
 * @{
 */
typedef uint8_t BleDisConnReason;
#define BLEDISCONNREASON_AUTHENTICATION_FAILURE                                       (0x05u)   /**< Authentication failure. */
#define BLEDISCONNREASON_PIN_OR_KEY_MISSING                                           (0x06u)   /**< Encryption key is missing or not saved on the remote device. */
#define BLEDISCONNREASON_CONNECTION_TIMEOUT                                           (0x08u)   /**< Connection timeout. */
#define BLEDISCONNREASON_REMOTE_USER_TERMINATED_CONNECTION                            (0x13u)   /**< Disconnection requested by the remote device users. */
#define BLEDISCONNREASON_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES     (0x14u)   /**< Disconnection by the remote device due to low resources. */
#define BLEDISCONNREASON_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF         (0x15u)   /**< Disconnection by the remote device due to power off. */
#define BLEDISCONNREASON_CONNECTION_TERMINATED_BY_LOCAL_HOST                          (0x16u)   /**< Disconnection by the local device host stack. */
#define BLEDISCONNREASON_UNSUPPORTED_REMOTE_FEATURE_UNSUPPORTED_LMP_FEATURE           (0x1Au)   /**< Unsupported remote feature. */
#define BLEDISCONNREASON_LMP_RESPONSE_TIMEOUT_LL_RESPONSE_TIMEOUT                     (0x22u)   /**< LMP response timeout. */
#define BLEDISCONNREASON_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                          (0x29u)   /**< Pairing with unit key unsupported. */
#define BLEDISCONNREASON_UNACCEPTABLE_CONNECTION_INTERVAL                             (0x3Bu)   /**< Connection interval unacceptable. */
#define BLEDISCONNREASON_CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE                     (0x3Du)   /**< Connection terminated due to MIC failure. */
#define BLEDISCONNREASON_CONNECTION_FAILED_TO_BE_ESTABLISHED                          (0x3Eu)   /**< Connection failed to be established. */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_gattRole BLE GATT Role Definition
 * @{
 * @details BLE GATT role.
 * @name BleGATTRole
 * @brief Define BLE GATT role.
 * @{
 */
typedef uint8_t BleGattRole;

/** GATT client is reading or writing data from or to the GATT server. */
#define BLE_GATT_ROLE_CLIENT                  (0x00u)

/** GATT server contains the characteristic database that is being read or written by a GATT client. */
#define BLE_GATT_ROLE_SERVER                  (0x01u)
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_advChannelMap BLE Advertising Channel Definition
 * @{
 * @ingroup adv_definition
 * @details BLE advertising channel definition.
 * @name BleAdvChannelMap
 * @brief Define BLE advertising channel.
 * @{
 */
typedef uint8_t BleAdvChannelMap;

/** Channel 37 */
#define ADV_CHANNEL_37         0x01

/** Channel 38 */
#define ADV_CHANNEL_38         0x02

/** Channel 39 */
#define ADV_CHANNEL_39         0x04

/** Channel 37 and Channel 38 */
#define ADV_CHANNEL_37_38      (ADV_CHANNEL_37 | ADV_CHANNEL_38)

/** Channel 37 and Channel 39 */
#define ADV_CHANNEL_37_39      (ADV_CHANNEL_37 | ADV_CHANNEL_39)

/** Channel 38 and Channel 39 */
#define ADV_CHANNEL_38_39      (ADV_CHANNEL_38 | ADV_CHANNEL_39)

/** Channel 37 and Channel 38 and channel 39 */
#define ADV_CHANNEL_ALL        (ADV_CHANNEL_37 | ADV_CHANNEL_38 | ADV_CHANNEL_39)
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_advFilter BLE Advertising Filter Policy Definition
 * @{
 * @ingroup adv_definition
 * @details BLE advertising filter policy definition.
 * @name BleAdvFilterPolicy
 * @brief Define BLE advertising filter policy.
 * @{
 */
typedef uint8_t BleAdvFilterPolicy;

/** Filter scan requests and connect requests from any device */
#define ADV_FILTER_POLICY_ACCEPT_ALL               0x00

/** Filter scan requests with whitelist,connect requests with from any . */
#define ADV_FILTER_POLICY_ACCEPT_SCAN_REQ_WL       0x01

/** Filter scan requests from any, connect requests with whitelist. */
#define ADV_FILTER_POLICY_ACCEPT_CONN_REQ_WL       0x02

/** Filter both scan and connect requests with whitelist. */
#define ADV_FILTER_POLICY_ACCEPT_SCAN_CONN_REQ_WL  0x03
/** @} */
/** @} */



/**
 * @defgroup ble_cmd_advInterval BLE Advertising Interval Minimum and Maximum Definition
 * @{
 * @ingroup adv_definition
 * @details BLE advertising interval range definition.
 * @name BleAdvInterval
 * @brief  Define BLE advertising interval minimum and maximum.
 * @{
 */
typedef uint8_t BleAdvIntervalRange;

/** The unit of advertising interval is 0.625ms \n
 * Minimum advertising interval is 20ms = (0x20 * 0.625) ms
*/
#define ADV_INTERVAL_MIN              0x0020

/** The unit of advertising interval is 0.625ms \n
 * Maximum advertising interval is 10.24s = (0x4000 * 0.625) ms
*/
#define ADV_INTERVAL_MAX              0x4000
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_advType BLE Advertising Type Definition
 * @{
 * @ingroup adv_definition
 * @details BLE advertising type definition.
 * @name BleAdvType
 * @brief  Define advertising type.
 * @{
 */
typedef uint8_t BleAdvType;
#define ADV_TYPE_ADV_IND              0x00  /**< Connectable and scannable undirected advertising. */
#define ADV_TYPE_ADV_DIRECT_IND       0x01  /**< Connectable directed advertising. */
#define ADV_TYPE_SCAN_IND             0x02  /**< Scanable undirected advertising. */
#define ADV_TYPE_ADV_NONCONN_IND      0x03  /**< Non-Connectable undirected advertising. */
#define ADV_TYPE_SCAN_RSP             0x04  /**< Scan Response. */
/** @} */
/** @} */



/**
 * @defgroup ble_cmd_scanType BLE Scan Type Definition
 * @{
 * @ingroup scan_definition
 * @details BLE scan type definition.
 * @name BleScanType
 * @brief  Define BLE scan type.
 * @{
 */
typedef uint8_t BleScanType;
#define SCAN_TYPE_PASSIVE             0x00  /**< Passive scanning. */
#define SCAN_TYPE_ACTIVE              0x01  /**< Active scanning. */
/** @} */
/** @} */



/**
 * @defgroup ble_cmd_scanInterval BLE Scan Interval Minimum and Maximum Definition
 * @{
 * @ingroup scan_definition
 * @details BLE scan Interval range definition.
 * @name BleScanInterval
 * @brief  Define BLE scan interval minimum and maximum.
 * @{
 */
typedef uint8_t BleScanIntervalRange;

/** The unit of scan interval is 0.625ms \n
 * Minimum scan interval is 2.5ms = (0x0004 * 0.625) ms
*/
#define SCAN_INTERVAL_MIN             0x0004

/** The unit of scan interval is 0.625ms \n
 * Maximum scan interval is 10.24s = (0x4000 * 0.625) ms
*/
#define SCAN_INTERVAL_MAX             0x4000
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_scanWindow BLE Scan Window Minimum and Maximum Definition
 * @{
 * @ingroup scan_definition
 * @details BLE scan window range definition.
 * @name BleScanWindow
 * @brief  Define BLE Scan window minimum and maximum.
 * @{
 */
typedef uint8_t BleScanWindowRange;

/** The unit of scan window is 0.625ms \n
 * Minimum scan window is 2.5ms = (0x0004 * 0.625) ms
*/
#define SCAN_WINDOW_MIN               0x0004

/** The unit of scan window is 0.625ms \n
 * Maximum scan window is 10.24s = (0x4000 * 0.625) ms
*/
#define SCAN_WINDOW_MAX               0x4000
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_scanFilterPolicy BLE Scan Filter Policy Definition
 * @{
 * @ingroup scan_definition
 * @details BLE scan filter policy definition.
 * @name BleScanFilterPolicy
 * @brief  Define BLE scan filter policy.
 * @{
 */
typedef uint8_t BleScanFilterPolicy;

/** Filter advertisement packets except directed advertising and scan response from any device */
#define SCAN_FILTER_POLICY_ACCEPT_ALL               0x00

/** Filter advertisement from the device which is in the white list . */
#define ADV_FILTER_POLICY_ACCEPT_WL                 0x01
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_connInterval BLE Connection Interval Definition
 * @{
 * @ingroup conn_definition
 * @details BLE connection interval range definition.
 * @name BleConnInt
 * @brief  Define BLE connection interval minimum and maximum.
 * @{
 */
typedef uint8_t BleConnectionIntervalRange;

/** The unit of connection interval is 1.25ms \n
 * Minimum connection interval is 7.5ms = (0x0006 * 1.25) ms
*/
#define CONN_INTERVAL_MIN             0x0006

/** The unit of connection interval is 1.25ms \n
 * Maximum connection interval is 4s = (0x0C80 * 1.25) ms
*/
#define CONN_INTERVAL_MAX             0x0C80
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_connLatency BLE Connection Latency Definition
 * @{
 * @ingroup conn_definition
 * @details BLE connection latency range definition.
 * @name BleConnLatency
 * @brief  Define BLE connection latency minimum and maximum.
 * @{
 */
typedef uint8_t BleConnectionLatencyRange;

/** Minimum connection latency is 0 */
#define CONN_LATENCY_MIN              0

/** Maximum connection latency is 0x01F3 */
#define CONN_LATENCY_MAX              0x01F3
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_connSupervisionTimeout BLE Connection Supervision Timeout Range Definition
 * @{
 * @ingroup conn_definition
 * @details BLE connection supervision timeout range definition.
 * @name BleConnSupTimeout
 * @brief  Define BLE connection supervision timeout minimum and maximum.
 * @{
 */
typedef uint8_t BleConnectionSupTimeoutRange;

/** The unit of connection supervision timeout is 10ms \n
 * Minimum connection supervision timeout is 100ms = (0x000A * 10) ms
*/
#define CONN_SUBTIMEOUT_MIN           0x000A

/** The unit of connection supervision timeout is 10ms \n
 * Maximum connection supervision timeout is 32s = (0x0C80 * 10) ms
*/
#define CONN_SUBTIMEOUT_MAX           0x0C80
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_addrType BLE Address Type Definition
 * @{
 * @details BLE device address type.
 * @name BleAddrType
 * @brief Define different BLE address types.
 * @{
 */
typedef uint8_t BleAddrType;
#define PUBLIC_ADDR                   0x00  /**< Public device address.  */
#define RANDOM_ADDR                   0x01  /**< Random device address.  */
#define PUBLIC_IDENTITY_ADDR          0x02  /**< Public identity address (corresponds to resolved private address). */
#define RANDOM_IDENTITY_ADDR          0x03  /**< Random (static) identity address (corresponds to resolved private address). */

/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_blePhy BLE PHY Definition
 * @{
 * @details BLE PHY definition.
 * @name BlePhy
 * @brief  Define BLE PHY.
 * @{
 */
typedef uint8_t BlePhy;
#define BLE_PHY_1M                    0x01  /**< The transmitter PHY is LE 1M.*/
#define BLE_PHY_2M                    0x02  /**< The transmitter PHY is LE 2M.*/
/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_bleState BLE State Definition
 * @{
 * @details BLE state definition.
 * @name BleState
 * @brief  Define BLE State.
 * @{
 */
typedef uint8_t BleMode;
#define STATE_BLE_STANDBY             0x00  /**< Standby mode.                  */
#define STATE_BLE_ADVERTISING         0x01  /**< Advertising mode.              */
#define STATE_BLE_CONNECTION          0x02  /**< Connection mode.               */
#define STATE_BLE_SCANNING            0x03  /**< Scanning mode.                 */
#define STATE_BLE_INITIATING          0x04  /**< Initialating mode.             */
#define STATE_BLE_CONN_ESTABLISHING   0x05  /**< Connection establishing mode.  */
/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_STK_GenMethod BLE STK Generator Method Definition
 * @{
 * @details BLE STK Generator method definition.
 * @name BleSTK_GenMethod
 * @brief  Define BLE STK generator methods.
 * @{
 */
typedef uint8_t BleSTK_GenMethod;
#define PASSKEY_ENTRY                 0x01  /**< Entry only.   */
#define PASSKEY_DISPLAY               0x02  /**< Display only. */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_connUpdateStatus BLE Connection Update Status Definition
 * @{
 * @details BLE connection update status definition.
 * @name BleConnUpdateStatus
 * @brief  Define BLE connection update status.
 * @{
 */
typedef uint8_t BleConnUpdateStatus;
#define CMD_SUCCESS                   0x00  /**< Success.  */
#define CMD_REJECTED                  0x01  /**< Rejected. */
#define CMD_TIMEOUT                   0x02  /**< Timeout.  */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_Auth_Status BLE Authentication Status Definition
 * @{
 * @details BLE authentication status definition.
 * @name BleAuthenticationStatus
 * @brief  Define BLE authentication status.
 * @{
 */
typedef uint8_t BleAuthStatus;
#define AUTH_SUCCESS                                 0x00  /**< Success.  */
#define AUTH_ERR_PASSKEY_ENTRY_FAILED                0x01  /**< Passkey entry failed.  */
#define AUTH_ERR_OOB_NOT_AVAILABLE                   0x02  /**< Out of Band key not available.  */
#define AUTH_ERR_AUTHENTICATION_REQUIREMENTS         0x03  /**< Error authentication requirement.  */
#define AUTH_ERR_CONFIRM_VALUE_FAILED                0x04  /**< Confirm value failed.  */
#define AUTH_ERR_PAIRING_NOT_SUPPORTED               0x05  /**< Pairing not supported.  */
#define AUTH_ERR_ENCRYPTION_KEY_SIZE                 0x06  /**< Encryption key size.  */
#define AUTH_ERR_COMMAND_NOT_SUPPORTED               0x07  /**< Command not supported.  */
#define AUTH_ERR_UNSPECIFIED_REASON                  0x08  /**< Unspecified reason.  */
#define AUTH_ERR_REPEATED_ATTEMPTS                   0x09  /**< Too little time has elapsed since last attempt.  */
#define AUTH_ERR_INVALID_PARAMETERS                  0x0A  /**< Invalid parameters.  */
#define AUTH_ERR_DHKEY_CHECK_FAILED                  0x0B  /**< DHKey check failed.  */
#define AUTH_ERR_NUMERIC_COMPARISON_FAILED           0x0C  /**< Numeric comparison failed.  */
#define AUTH_ERR_BR_EDR_PAIRING_IN_PROGRESS          0x0D  /**< BR/EDR paring in progress.  */
#define AUTH_ERR_CROSS_TRANS_KEY_GEN_NOT_ALLOWED     0x0E  /**< BR/EDR Link Key cannot be used for LE keys.  */
#define AUTH_ERR_TIMEOUT                             0x0F  /**< Procedure time out.  */
#define AUTH_ERR_PIN_OR_KEY_MISSING                  0x10  /**< Bonding PIN / KEY missing.  */
/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_PhyUpdateStatus BLE Phy Update Status Definition
 * @{
 * @details BLE Phy update status definition.
 * @name BlePhyUpdateStatus
 * @brief  Define BLE Phy update status.
 * @{
 */
typedef uint8_t BlePhyUpdateStatus;
#define PHY_UPDATE_SUCCESS                           0x00  /**< Success.  */
#define PHY_UPDATE_REMOTE_FEATURE_UNSUPPORTED        0x1A  /**< Command cancel due to remote feature unsupported.  */
#define PHY_UDDATE_TRANSACTION_COLLISION             0x2A  /**< Command cancel due to transaction collision. */

/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_Adv_Complete_Event_Status BLE Advertising Complete Event Status Definition
 * @{
 * @details BLE advertising complete event status definition.
 * @name BleAdvCompleteStatus
 * @brief  Define BLE advertising complete event status.
 * @{
 */
typedef uint8_t BleAdvCompleteStatus;
#define ADV_ENABLE_CMD_SUCCESS                       0x00  /**< Success.  */
#define CMD_REJECTED_DUE_TO_LIMITED_RESOURCES        0x0D  /**< Rejected. */

/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_IOCaps BLE IO Capabilities Definition
 * @{
 * @details BLE IO capabilities definition.
 * @name BleIOCaps
 * @brief  Define BLE IO capabilities.
 * @{
 */
typedef uint8_t IOCaps;
#define DISPLAY_ONLY                                 0x00  /**< Display only.              */
#define DISPLAY_YESNO                                0x01  /**< Display and Yes/No entry.  */
#define KEYBOARD_ONLY                                0x02  /**< Keyboard only.             */
#define NOINPUT_NOOUTPUT                             0x03  /**< No IO capabilities.        */
#define KEYBOARD_DISPLAY                             0x04  /**< Keyboard and display.      */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_BondingFlags BLE Bonding Flags Definition
 * @{
 * @details BLE bonding flags definition.
 * @name BondingFlags
 * @brief  Define BLE bonding flags.
 * @{
 */
typedef uint8_t BondingFlags;
#define NO_BONDING                                   0x00  /**< NO Bonding.  */
#define BONDING                                      0x01  /**< Bonding.     */
/** @} */
/** @} */

/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_Conn_status BLE Connection Event Status Definition
 * @{
 * @details BLE connection event status definition.
 * @name BleConnStatus
 * @brief  Define BLE connection event status.
 * @{
 */
typedef uint8_t BleConnStatus;
#define COMMAND_SUCCESS                              0x00  /**< Success.  */
#define COMMAND_DISALLOWED                           0x0C  /**< Disallowed. */
#define COMMAND_REJECTED_DUE_TO_LIMITED_RESOURCES    0x0D  /**< Rejected due to the limited resources. */
#define COMMAND_ADVERTISING_TIMEOUT                  0x3C  /**< Advertising timeout. */

/** @} */
/** @} */



/**
 * @defgroup ble_cccd BLE GATT Client Characteristic Configuration Value Definition
 * @{
 * @details BLE GATT client characteristic configuration value definition.
 * @ingroup ble_cmd_definition
 * @{
 */
typedef uint16_t BleGattCCCDValue;
#define BLEGATT_CCCD_NONE                            0x0000  /**< Disabled notification/ indication.  */
#define BLEGATT_CCCD_NOTIFICATION                    0x0001  /**< The Characteristic shall be notified.  */
#define BLEGATT_CCCD_INDICATION                      0x0002  /**< The Characteristic shall be indicated.  */
#define BLEGATT_CCCD_NOTIFY_INDICATE                 0x0003  /**< The Characteristic shall be both notified and indicated. */
/** @} */
/** @} */



/**
 * @defgroup bleCharWrite BLE GATT Characteristic Value Write Definition
 * @{
 * @details There are five sub-procedures that can be used to write a Characteristic Value.
 * @ingroup ble_cmd_definition
 * @{
 */
typedef uint8_t BleGattWrite;
#define BLEGATT_WRITE                                0x00  /**< GATT write (Write request).  */
#define BLEGATT_WRITE_WITHOUT_RSP                    0x01  /**< GATT write without response (Write command). */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_connRole BLE Connection Role Definition
 * @{
 * @details BLE connection role.
 * @name BleConnRole
 * @brief Define BLE connection role.
 * @{
 */
typedef uint8_t BleConnRole;
#define BLEROLE_MASTER                      (0x00u) /**< Master role. */
#define BLEROLE_SLAVE                       (0x01u) /**< Slave role. */
/** @} */
/** @} */



/**************************************************************************
 * Structures
 **************************************************************************/
/** @defgroup ble_cmd_structure BLE Structure Definition
 * @{
 * @details Here shows the structures in ble_cmd.h.
 * @ingroup ble_common
 **************************************************************************/

/**
 * @brief BLE address.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Addr_Param
{
    BleAddrType addrType;             /**< @ref ble_cmd_addrType "BLE address type".  */
    uint8_t     addr[SIZE_BLE_ADDR];  /**< BLE address 48-bit in LSB.  */

} BLE_Addr_Param;


/** @brief BLE advertising parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Adv_Param
{
    /**
     * @ref ble_cmd_advType "BLE advertising type"
     */
    BleAdvType          advType;

    /** Minimum advertising interval. \n
      *  @note Advertising interval = value * 0.625ms \n
      *        Advertising interval range = @ref ADV_INTERVAL_MIN ~ @ref ADV_INTERVAL_MAX
      */
    uint16_t            advIntervalMin;

    /** Maximum advertising  interval.
      *  @note Advertising interval = value * 0.625ms \n
      *        Advertising interval range = @ref ADV_INTERVAL_MIN ~ @ref ADV_INTERVAL_MAX
      */
    uint16_t            advIntervalMax;


    /** The BLE address and BLE address type for directed advertising
     * @note MUST set advDirectAddrParam if BLE advertising type is set to @ref ADV_TYPE_ADV_DIRECT_IND.
    */
    BLE_Addr_Param      advDirectAddrParam;

    /**
     * @ref ble_cmd_advChannelMap "Advertising channel map"
     */
    BleAdvChannelMap    advChannelMap;

    /**
     * @ref ble_cmd_advFilter "Advertising filter policy"
     */
    BleAdvFilterPolicy  advFilterPolicy;
} BLE_Adv_Param;



/** @brief BLE scan parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Scan_Param
{
    /**
     * @ref ble_cmd_scanType "BLE scan type"
     */
    BleScanType           scanType;

    /** Scan interval.
     *  @note Scan interval = value * 0.625ms \n
     *        Scan interval range = @ref SCAN_INTERVAL_MIN ~ @ref SCAN_INTERVAL_MAX
     */
    uint16_t              scanInterval;

    /** Scan window.
     *  @note Scan window = value * 0.625ms \n
     *        Scan window range = @ref SCAN_WINDOW_MIN  ~ @ref SCAN_WINDOW_MAX
     */
    uint16_t              scanWindow;

    /**
     * @ref ble_cmd_scanFilterPolicy "BLE scan filter policy"
     */
    BleScanFilterPolicy   scanFilterPolicy;

} BLE_Scan_Param;



/** @brief BLE connection parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Conn_Param
{
    /** Minimum connection interval.
      *  @note Connection interval = value * 1.25ms \n
      *        Connection interval range = @ref CONN_INTERVAL_MIN ~ @ref CONN_INTERVAL_MAX
      */
    uint16_t connIntervalMin;

    /** Maximum connection interval.
    *  @note Connection interval = value * 1.25ms \n
    *        Connection interval range = @ref CONN_INTERVAL_MIN ~ @ref CONN_INTERVAL_MAX
    */
    uint16_t connIntervalMax;

    /** Slave latency for the connection in number of connection event.
     *  @note Latency range = @ref CONN_LATENCY_MIN  to @ref CONN_LATENCY_MAX
     */
    uint16_t connLatency;

    /** Connection supervision timeout.
     *  @note Supervision timeout = value * 10ms \n
     *        Supervision timeout range = @ref CONN_SUBTIMEOUT_MIN  ~ @ref CONN_SUBTIMEOUT_MAX
     */
    uint16_t connSupervisionTimeout;

} BLE_Conn_Param;



/** @brief BLE IO Capabilities.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_IOCaps_Param
{
    IOCaps          ioCapsParam;              /**< @ref ble_cmd_IOCaps "IO capabilities". */

} BLE_IOCaps_Param;



/** @brief BLE Bonding Flags.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_BondingFlags_Param
{
    BondingFlags     bondingFlags;            /**< @ref ble_cmd_BondingFlags "Bonding flags". */

} BLE_BondingFlags_Param;




/** @brief BLE Data Length.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_DataLength_Param
{
    /** Preferred maximum number of payload octets that the Controller for reception in a single Link Layer packet on this connection.*/
    uint16_t          txMaxOctets;

    /** Preferred maximum number of payload octets that the Controller for transmission in a single Link Layer packet on this connection.*/
    uint16_t          rxMaxOctets;

} BLE_DataLength_Param;


/**
 * @brief BLE Phy Parameters.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Phy_Param
{
    BlePhy             txPhy;                 /**< Tx Phy : @ref BLE_PHY_1M or @ref BLE_PHY_2M */
    BlePhy             rxPhy;                 /**< Rx Phy : @ref BLE_PHY_1M or @ref BLE_PHY_2M  */
} BLE_Phy_Param;


/** @brief BLE Event Attribute Database Parsed Result Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_AttDbParsed_Param
{
    uint8_t         hostId;                   /**< Host id. */
    uint8_t         result;                   /**< The result of parsing attribute database operation. */
} BLE_Event_AttDbParsed_Param;


/**
 * @brief BLE Event Advertising Completed Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_AdvCompleteParam
{
    BleAdvCompleteStatus  status;             /**< @ref ble_cmd_Adv_Complete_Event_Status "BLE advertising complete event status." */

} BLE_Event_AdvCompleteParam;



/**
 * @brief BLE Event Connection Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_ConnParam
{
    BleConnStatus      status;                   /**< @ref ble_cmd_Conn_status "BLE connection event status". */
    uint8_t            hostId;                   /**< Host id. */
    BleConnRole        connRole;                 /**< @ref ble_cmd_connRole "Connection role" */
    BLE_Addr_Param     peerAddr;                 /**< Remote device address type and address. */
    uint16_t           connInterval;             /**< The current link connection interval value. */
    uint16_t           connLatency;              /**< The current link connection latency value. */
    uint16_t           connSupervisionTimeout;   /**< The current link connection supervision timeout value. */

} BLE_Event_ConnParam;


/**
 * @brief BLE Event Connection Update Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_ConnUpdateParam
{
    BleConnUpdateStatus status;                   /**< @ref ble_cmd_connUpdateStatus "BLE connection update event status." */
    uint8_t             hostId;                   /**< Host id. */
    uint16_t            connInterval;             /**< The current link connection interval value. */
    uint16_t            connLatency;              /**< The current link connection latency value. */
    uint16_t            connSupervisionTimeout;   /**< The current link connection supervision timeout value. */

} BLE_Event_ConnUpdateParam;



/**
 * @brief BLE Event Create Connection Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_CreateConnParam
{
    BleConnStatus      status;                /**< @ref ble_cmd_Conn_status "BLE connection create event status". */

} BLE_Event_CreateConnParam;


/**
 * @brief BLE Event Scan Report Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_ScanReportParam
{
    BleAdvType      rptType;                  /**< @ref ble_cmd_addrType "BLE advertising type". */
    BLE_Addr_Param  rptPeerAddr;              /**< Remote device address type and address. */
    uint8_t         rptDataLength;            /**< Received advertising or scan response data length. */
    uint8_t         rptData[31];              /**< Received advertising or scan response data. */
    int8_t          rptRssi;                  /**< Received Signal Strength Indication in dBm. */

} BLE_Event_ScanReportParam;


/**
 * @brief BLE Event Disconnection Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_DisconnParam
{
    uint8_t            hostId;                /**< Host id. */
    BleDisConnReason   disconnectReason;      /**< @ref ble_cmd_disconnReason "Disconnection reason". */

} BLE_Event_DisconnParam;



/** @brief BLE Event RSSI Parameters.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_RssiParam
{
    uint8_t           hostId;                 /**< Host id. */
    int8_t            rssi;                   /**< RSSI value. */
} BLE_Event_RssiParam;



/**
 * @brief BLE Event STK Generator Method Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_StkGenMethodParam
{
    uint8_t            hostId;                /**< Host id. */
    BleSTK_GenMethod   stkGenMethod;          /**< @ref ble_cmd_STK_GenMethod "STK generate method." */

} BLE_Event_StkGenMethodParam;



/**
 * @brief BLE Event PassKey Confirmation Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_PassKeyConfirmParam
{
    uint8_t            hostId;                /**< Host id. */
} BLE_Event_PassKeyConfirmParam;



/**
 * @brief BLE Event Authentication Status Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_AuthStatusParam
{
    uint8_t            hostId;                /**< Host id. */
    BleAuthStatus      status;                /**< @ref ble_cmd_Auth_Status "BLE authentication event status." */
} BLE_Event_AuthStatusParam;


/**
 * @brief BLE Event Phy Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_PhyParam
{
    uint8_t            hostId;                /**< Host id. */
    BLE_Phy_Param      phy;                   /**< Tx/ Rx Phy. */

} BLE_Event_PhyParam;


/**
 * @brief BLE Event Phy Update Complete Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_PhyUpdateParam
{
    BlePhyUpdateStatus     status;            /**< @ref ble_cmd_PhyUpdateStatus "BLE Phy update event status". */
    uint8_t                hostId;            /**< Host id. */
    BLE_Phy_Param          phy;               /**< Tx/Rx Phy. */
} BLE_Event_PhyUpdateParam;




/**
 * @brief BLE Event MTU Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_MtuParam
{
    uint8_t      hostId;                      /**< Host id. */
    uint16_t     mtuSize;                     /**< MTU size */

} BLE_Event_MtuParam;


/**
 * @brief BLE Event Data Length Update Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_DataLengthParam
{
    uint8_t                 hostId;           /**< Host id. */
    BLE_DataLength_Param    dataLenparam;     /**< Data length parameter. */

} BLE_Event_DataLengthParam;


/**
 * @brief BLE Event Host Attribute Database Parsed Result Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_AttDbParsedParam
{
    BLE_Event_AttDbParsed_Param     attDbParsedResultparam;     /**< Parameter shows the result of parsing attribute database operation. */

} BLE_Event_AttDbParsedParam;

/** @} */


/**************************************************************************
 * Functions
 **************************************************************************/
/** @defgroup ble_cmd_comAPI BLE Command Function
 * @{
 * @details Here shows the function in ble_cmd.h. \n\n
 *          <b>BLE stacks:
 *          - [BLELIB_NOCONN]: advertisement + scan (not support BLE connection).
 *          - [BLELIB_P]     : peripheral (+ server) only.
 *          - [BLELIB_C]     : Central (+ client) only.
 *          - [BLELIB_PC]    : peripheral and Central (Full function). </b>
 * @see @ref ble_BLEAPI
 * @ingroup ble_common
 * @}
 * @defgroup ble_BLEAPI BLE API Comparison
 * @{
 * @ingroup ble_cmd_comAPI
 * @}
 * @defgroup ble_cmd_function BLE Command Function
 * @{
 * @ingroup ble_cmd_comAPI
 * @}
 **************************************************************************/

/** Set BLE Stack Initialization
 *
 * This function is used to initial BLE stack.
 *
 * @ingroup ble_cmd_function
 *
 * @remark <b>[BLELIB_NOCONN], [BLELIB_P], [BLELIB_C], [BLELIB_PC] </b>
 *
 * @attention MUST initial BLE stack after @ref setRF_Init \n
 *
 * @retval BLESTACK_STATUS_ERR_INVALID_STATE     : RF Phy has NOT initialized.
 * @retval BLESTACK_STATUS_ERR_INVALID_HOSTCOUNT : Can ONLY be set while in standby state.
 * @retval BLESTACK_STATUS_SUCCESS               : Setting success.
 */
BleStackStatus setBLE_BleStackInit(void);


/** Set BLE Stack Handle Interrupt ISR
 *
 * This function is used to handle BLE stack interrupt.
 *
 * @ingroup ble_cmd_function
 *
 * @remark <b>[BLELIB_NOCONN], [BLELIB_P], [BLELIB_C], [BLELIB_PC] </b>
 *
 */
void setBLE_BleStackGpio_Isr(void);


/** Set BLE Device Address and Device Address Type
 *
 * This function is used to set BLE Local Device Address and Device Address Type.
 *
 * @ingroup ble_cmd_function
 *
 * @remark <b>[BLELIB_NOCONN], [BLELIB_P], [BLELIB_C], [BLELIB_PC] </b>
 *
 * @attention MUST set BLE Device Address and Address Type after @ref setRF_Init and @ref setBLE_BleStackInit \n
 *
 * @attention  If the address/ address type change, MUST re-init @ref setBLE_AdvParam "advertisement parameter" or @ref setBLE_ScanParam "scan parameter" or
 *            @ref setBLE_ConnCreate "create connection" to use the updated address/ address type.
 *
 * @note      BLE Address Type : MUST be either @ref PUBLIC_ADDR or @ref RANDOM_ADDR \n
 *            If BLE Address Type is set to @ref RANDOM_ADDR (static random address),
 *            the two most significant bits of the address shall be equal to 1. \n
 *
 * @note      BLE Address : Little Endian and the length is @ref SIZE_BLE_ADDR \n
 *            If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
 *
 * @param[in] addrParam : a pointer to local device BLE address and BLE address type \n
 *
 * @retval BLESTACK_STATUS_ERR_NOT_SUPPORTED : Command not supported.
 * @retval BLESTACK_STATUS_ERR_INVALID_STATE : BLE stack has not initialized or can ONLY be set while in standby state.
 * @retval BLESTACK_STATUS_ERR_INVALID_PARAM : Invalid parameter.
 * @retval BLESTACK_STATUS_SUCCESS           : Setting success.
 */
BleStackStatus setBLE_BleDeviceAddr(BLE_Addr_Param *addrParam);


/** Get BLE Device Address and Device Address Type
 *
 *  This function is used to get BLE Local Device Address and Device Address Type.
 *
 * @ingroup ble_cmd_function
 *
 * @remark <b>[BLELIB_NOCONN], [BLELIB_P], [BLELIB_C], [BLELIB_PC] </b>
 *
 * @note       BLE Address Type : @ref PUBLIC_ADDR or @ref RANDOM_ADDR \n
 *             BLE Address : Little Endian and the length is @ref SIZE_BLE_ADDR \n
 *             If Device BLE Address is set to "01:02:03:04:05:06", addrParam->addr[0] = 0x06
 *
 * @param[out] addrParam : a pointer to local device BLE address and BLE address type. \n
 *
 * @retval BLESTACK_STATUS_ERR_INVALID_STATE : BLE stack has not initialized.
 * @retval BLESTACK_STATUS_SUCCESS           : Setting success.
 */
BleStackStatus getBLE_BleDeviceAddr(BLE_Addr_Param *addrParam);



#endif // _BLE_CMD_H_

