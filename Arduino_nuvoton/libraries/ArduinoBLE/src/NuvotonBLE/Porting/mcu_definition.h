#ifndef __MCU_DEFINITION_H__
#define __MCU_DEFINITION_H__

#include <stdint.h>
#include "NuMicro.h"

/**************************************************************************
 * Definitions
 **************************************************************************/
/**
 * @defgroup porting Porting
 * @{
 * @details MCU porting & BLE Bonding Porting Related \n
 * -  MCU Definition
 * -  MCU SPI Porting Definition
 * -  MCU MISC Porting Definition
 * -  MCU Flash Porting Definition
 * -  RF Power Definition
 * -  BLE Bonding Porting Related
 *
 * @defgroup mcuPortingDef MCU Porting Related
 * @{
 * @details MCU porting related definitions and functions. \n
 * -  MCU Definition
 * -  MCU SPI Porting Definition
 * -  MCU MISC Porting Definition
 * -  MCU Flash Porting Definition
 * -  RF Power Definition
 *
 * @defgroup mcuDef MCU Definition
 * @{
 * @details MCU setting definition for user configuration are listed as following. \n
 * @verbatim
   1. _BOARD_SELECTION_
   2. _USE_MCU_CLK_
   3. CPU_CLOCK_RATE
   4. PCLK_DIV
   5. InterruptDisable
   6. InterruptEnable
 @endverbatim
 * @}@}@}
 **************************************************************************/

/**
 * @ingroup mcuDef
 * @defgroup platformDef Platform Definition
 * @{
 * @brief Defined supported platform for user.
 */
#define _BOARD_NUVOTON_M031SE_              0               /**< M031 EVK */
#define _BOARD_NUVOTON_M032SE3AE_           1               /**< M032 EVK */
#define _BOARD_NUVOTON_M487JIDAE_B3_        2               /**< M487 new EVK */
#define _BOARD_NUVOTON_M031TD2AE_QFN33_     3               /**< M031 daughter board */
#define _BOARD_NUVOTON_M031_SIP_            4               /**< M031 + RF SIP  */
#define _BOARD_NUVOTON_M032_SIP_            5               /**< M032 + RF SIP  */
/** @} */

/** Selected a platform from @ref platformDef "platform definition list".
 * @ingroup mcuDef
*/
#define _BOARD_SELECTION_                   _BOARD_NUVOTON_M032_SIP_


/**
 * @ingroup mcuDef
 * @defgroup mcuClockDef MCU Clock Source Definition
 * @{
 * @brief Defined supported MCU clock source for user.
 */
#define MCU_CLK_SOURCE_HXT                  0               /**< HXT */
#define MCU_CLK_SOURCE_HIRC                 1               /**< HIRC */
/** @} */

/** Selected the clock source from @ref mcuClockDef "MCU clock source definition list".
 * @ingroup mcuDef
*/
#define _USE_MCU_CLK_                       MCU_CLK_SOURCE_HIRC


/**
 * @ingroup mcuDef
 * @{
 */
#if (_BOARD_SELECTION_==_BOARD_NUVOTON_M487JIDAE_B3_)
#define CPU_CLOCK_RATE                      64000000u       /**< CPU clock rate */
#define PCLK_DIV                            2u              /**< PCLK=CPU_CLOCK_RATE/PCLK_DIV. it is better to choose PCLK=8*N (24MHz or 32MHz) for SPI_clock=12M */
#else
#define CPU_CLOCK_RATE                      48000000u       /**< CPU clock rate */
#define PCLK_DIV                            1u              /**< PCLK=CPU_CLOCK_RATE/PCLK_DIV. it is better to choose PCLK=8*N (24MHz or 32MHz) for SPI_clock=12M */
#endif



/**
 * @brief Different MCU may supports different interrupt definition.
 * @note If it is not ARM based MCU please undefined it and re-defined @ref InterruptDisable and @ref InterruptEnable.
 */
#define _MCU_ARM_CORTEX_

#ifdef _MCU_ARM_CORTEX_
#define InterruptDisable                    __disable_irq   /**< Disable interrupt function definition for ARM based. */
#define InterruptEnable                     __enable_irq    /**< Enable interrupt function definition for ARM based. */
#endif  //(#ifdef _MCU_ARM_CORTEX_)
/** @} */


#endif  //__MCU_DEFINITION_H__


