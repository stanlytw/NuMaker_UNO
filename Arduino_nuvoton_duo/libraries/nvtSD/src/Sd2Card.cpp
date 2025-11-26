/**
  ******************************************************************************
  * @file    Sd2Card.cpp
  * @author  Frederic Pillon <frederic.pillon@st.com> for STMicroelectronics
  * @date    2017
  * @brief
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include <Arduino.h>
#include "NuMicro.h"
#include "Sd2Card.h"
#include "SDGlue.h"
#include "./utility/nvtDef_helper.h"
/**
  * @brief  Default constructor. Use default pins definition.
  */
Sd2Card::Sd2Card()
{
  sd_Init = 0;
  sdh_id = 0;
  sdh_handler = TARGET_SDH;//Assign to SDH0
}

bool Sd2Card::init(uint32_t detect, uint32_t level)
{
  /* Pin Assignment if required*/
#if defined(__USESDH1__)  //For SDH1  
  TCHAR       sd_path[] = { '1', ':', 0 };    /* SD drive started from 1 */
#else//
  TCHAR       sd_path[] = { '0', ':', 0 };    /* SD drive started from 0 */
#endif

  //Check if handler is Null;
  if(sdh_handler!=TARGET_SDH) 
      return false;
  /* Detect Card Status, Inserted or not*/
  /* Init the Card and Get Card Information*/
  if(!(SDH_CardDetection(sdh_handler)))
  {
      sd_Init = 0;
      nvtDbg_printf("No card!!\n");
      //continue;
  }
  
  f_chdrive(sd_path);          /* set default path */

  if (!sd_Init)
  {
      sd_Init = (SDGlue_Open_Disk(sdh_handler, DEF_CARD_DETECT_SOURCE)==0)?1:0;
  }
  
  if (sd_Init)
  {
      /*SDCard HW init done*/
      nvtDbg_printf("sd_Init Done!!\n");

      /*Pack CardInfo into specified architecture*/
      memset(&_SdCardInfo, 0, sizeof(_SD_CardInfo));
      SDGlue_Get_CardInfo(&_SdCardInfo);
      return true;
  }
  else
  {
      nvtDbg_printf("sd_Init Fail!!\n");
      return false;
  }
      
}

bool Sd2Card::deinit(void)
{
  if(sdh_handler!=TARGET_SDH)
      return false;
  
  SDGlue_Close_Disk(sdh_handler);
  return true;
}

uint8_t Sd2Card::type(void) const
{
#if 0  
  uint8_t cardType = SD_CARD_TYPE_UKN;
  switch (_SdCardInfo.CardType) {
    case CARD_SDSC:
      switch (_SdCardInfo.CardVersion) {
        case CARD_V1_X:
          cardType = SD_CARD_TYPE_SD1;
          break;
        case CARD_V2_X:
          cardType = SD_CARD_TYPE_SD2;
          break;
        default:
          cardType = SD_CARD_TYPE_UKN;
      }
      break;
    case CARD_SDHC_SDXC:
      cardType = SD_CARD_TYPE_SDHC;
      break;
    case CARD_SECURED:
      cardType = SD_CARD_TYPE_SECURED;
      break;
    default:
      cardType = SD_CARD_TYPE_UKN;
  }
  return cardType;
#endif  
  return _SdCardInfo.CardType;
}

