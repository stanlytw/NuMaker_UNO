/**
  ******************************************************************************
  * @file    SdFatFs.cpp
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
#include "SdFatFs.h"
#include "SDGlue.h"
bool SdFatFs::init(void)
{

  /* Mount/register the file system object to the FatFs module */
#if defined(__USESDH1__)  //For SDH1  
  _Path[0] = '1';
#else
  _Path[0] = '0';
#endif  
  _Path[1] = ':';
  _Path[2] =  0;
  if(f_mount(&_FatfsVolSd0, _Path, 1)== FR_OK)
  {
    return true;
  } 

  return false;
  
}

bool SdFatFs::deinit(void)
{
  /* Unmount/unregister the file system object to the FatFs module */
  if(f_unmount(_Path)== FR_OK)
  {
      return true;  
  }
  return false;
}

uint8_t SdFatFs::fatType(void)
{
  switch (_FatfsVolSd0.fs_type) {
    case FS_FAT12:
      return 12;
    case FS_FAT16:
      return 16;
    case FS_FAT32:
      return 32;
    default:
      return 0;
  }
}
