#ifndef _BLEAPPSETTING_H_
#define _BLEAPPSETTING_H_


#include "mcu_definition.h"
#include "porting_misc.h"

/**************************************************************************
 * Application Setting
 **************************************************************************/
// Define the maximum number of BLE connection link.
#define BLE_SUPPORT_NUM_CONN_MAX    1

// BLE stack supports large MTU size if define ENABLE_DEF.
#define BLE_SUPPORT_MTU_LARGE       ENABLE_DEF

// Set device BLE company ID
#define BLE_COMPANY_ID_L            0xFF
#define BLE_COMPANY_ID_H            0xFF


#endif
