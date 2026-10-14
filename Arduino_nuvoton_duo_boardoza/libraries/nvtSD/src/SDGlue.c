/**************************************************************************//**
 * @file     SDGlue.c
 * @version  V1.00
 * @brief    SD glue functions for FATFS
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "SDGlue.h"
#include "./utility/nvtDef_helper.h"

//FATFS  _FatfsVolSd0;
//FATFS  _FatfsVolSd1;
//static TCHAR  _Path[3];
extern SDH_INFO_T SD0, SD1;//declared in sdh driver

int SDGlue_Open_Disk(SDH_T *sdh, uint32_t u32CardDetSrc)
{
    SDH_Open(sdh, u32CardDetSrc);
    nvtDbg_printf("SDH Open finished!\n");
    if (SDH_Probe(sdh))
    {
        nvtDbg_printf("++SD initial fail!!\n");
        return -1;
    }
    else
    {
        nvtDbg_printf("++SD initial pass!!\n");
        nvtDbg_printf("++Card Type:%d\n", SD0.CardType);
    }
    
    return 0;
}

void SDGlue_Close_Disk(SDH_T *sdh)
{
    if (sdh == SDH0)
    {
        memset(&SD0, 0, sizeof(SDH_INFO_T));
    }
    else if(sdh == SDH1)
    {
        memset(&SD1, 0, sizeof(SDH_INFO_T));
    }
    
}

void SDH0_IRQHandler(void)
{
    unsigned int volatile isr;
    unsigned int volatile ier;

    // FMI data abort interrupt
    if (SDH0->GINTSTS & SDH_GINTSTS_DTAIF_Msk)
    {
        /* ResetAllEngine() */
        SDH0->GCTL |= SDH_GCTL_GCTLRST_Msk;
    }

    //----- SD interrupt status
    isr = SDH0->INTSTS;
    ier = SDH0->INTEN;

    if (isr & SDH_INTSTS_BLKDIF_Msk)
    {
        // block down
        SD0.DataReadyFlag = TRUE;
        SDH0->INTSTS = SDH_INTSTS_BLKDIF_Msk;
    }

    if ((ier & SDH_INTEN_CDIEN_Msk) &&
            (isr & SDH_INTSTS_CDIF_Msk))    // card detect
    {
        //----- SD interrupt status
        // it is work to delay 50 times for SD_CLK = 200KHz
        {
            int volatile i;         // delay 30 fail, 50 OK
            for (i=0; i<0x500; i++);  // delay to make sure got updated value from REG_SDISR.
            isr = SDH0->INTSTS;
        }

#if (DEF_CARD_DETECT_SOURCE==CardDetect_From_DAT3)
        if (!(isr & SDH_INTSTS_CDSTS_Msk))
#else
        if (isr & SDH_INTSTS_CDSTS_Msk)
#endif
        {
            nvtDbg_printf("\n***** card remove !\n");
            SD0.IsCardInsert = FALSE;   // SDISR_CD_Card = 1 means card remove for GPIO mode
            memset(&SD0, 0, sizeof(SDH_INFO_T));
        }
        else
        {
            nvtDbg_printf("***** card insert !\n");
//            SDH_Open(SDH0, CardDetect_From_GPIO);
//            SDH_Probe(SDH0);
        }

        SDH0->INTSTS = SDH_INTSTS_CDIF_Msk;
    }

    // CRC error interrupt
    if (isr & SDH_INTSTS_CRCIF_Msk)
    {
        if (!(isr & SDH_INTSTS_CRC16_Msk))
        {
            //printf("***** ISR sdioIntHandler(): CRC_16 error !\n");
            // handle CRC error
        }
        else if (!(isr & SDH_INTSTS_CRC7_Msk))
        {
            if (!SD0.R3Flag)
            {
                //printf("***** ISR sdioIntHandler(): CRC_7 error !\n");
                // handle CRC error
            }
        }
        SDH0->INTSTS = SDH_INTSTS_CRCIF_Msk;      // clear interrupt flag
    }

    if (isr & SDH_INTSTS_DITOIF_Msk)
    {
        nvtDbg_printf("***** ISR: data in timeout !\n");
        SDH0->INTSTS |= SDH_INTSTS_DITOIF_Msk;
    }

    // Response in timeout interrupt
    if (isr & SDH_INTSTS_RTOIF_Msk)
    {
        nvtDbg_printf("***** ISR: response in timeout !\n");
        SDH0->INTSTS |= SDH_INTSTS_RTOIF_Msk;
    }
}


void SDH1_IRQHandler(void)
{
    unsigned int volatile isr;
    unsigned int volatile ier;

    // FMI data abort interrupt
    if (SDH1->GINTSTS & SDH_GINTSTS_DTAIF_Msk)
    {
        /* ResetAllEngine() */
        SDH1->GCTL |= SDH_GCTL_GCTLRST_Msk;
    }

    //----- SD interrupt status
    isr = SDH1->INTSTS;
    ier = SDH1->INTEN;

    if (isr & SDH_INTSTS_BLKDIF_Msk)
    {
        // block down
        SD1.DataReadyFlag = TRUE;
        SDH1->INTSTS = SDH_INTSTS_BLKDIF_Msk;
    }

    if ((ier & SDH_INTEN_CDIEN_Msk) &&
            (isr & SDH_INTSTS_CDIF_Msk))    // card detect
    {
        //----- SD interrupt status
        // it is work to delay 50 times for SD_CLK = 200KHz
        {
            int volatile i;         // delay 30 fail, 50 OK
            for (i=0; i<0x500; i++);  // delay to make sure got updated value from REG_SDISR.
            isr = SDH1->INTSTS;
        }

#if (DEF_CARD_DETECT_SOURCE==CardDetect_From_DAT3)
        if (!(isr & SDH_INTSTS_CDSTS_Msk))
#else
        if (isr & SDH_INTSTS_CDSTS_Msk)
#endif
        {
            nvtDbg_printf("\n***** card remove !\n");
            SD1.IsCardInsert = FALSE;   // SDISR_CD_Card = 1 means card remove for GPIO mode
            memset(&SD1, 0, sizeof(SDH_INFO_T));
        }
        else
        {
            nvtDbg_printf("***** card insert !\n");
//            SDH_Open(SDH0, CardDetect_From_GPIO);
//            SDH_Probe(SDH0);
        }

        SDH1->INTSTS = SDH_INTSTS_CDIF_Msk;
    }

    // CRC error interrupt
    if (isr & SDH_INTSTS_CRCIF_Msk)
    {
        if (!(isr & SDH_INTSTS_CRC16_Msk))
        {
            //printf("***** ISR sdioIntHandler(): CRC_16 error !\n");
            // handle CRC error
        }
        else if (!(isr & SDH_INTSTS_CRC7_Msk))
        {
            if (!SD1.R3Flag)
            {
                //printf("***** ISR sdioIntHandler(): CRC_7 error !\n");
                // handle CRC error
            }
        }
        SDH1->INTSTS = SDH_INTSTS_CRCIF_Msk;      // clear interrupt flag
    }

    if (isr & SDH_INTSTS_DITOIF_Msk)
    {
        nvtDbg_printf("***** ISR: data in timeout !\n");
        SDH1->INTSTS |= SDH_INTSTS_DITOIF_Msk;
    }

    // Response in timeout interrupt
    if (isr & SDH_INTSTS_RTOIF_Msk)
    {
        nvtDbg_printf("***** ISR: response in timeout !\n");
        SDH1->INTSTS |= SDH_INTSTS_RTOIF_Msk;
    }
}

int8_t SDGlue_Get_CardInfo(_SD_CardInfo * psdCardInfo)
{
    /*Get card info in Glue layer from sdh driver*/
    //Card Type
#if defined(__M467SJHAN__) 
    psdCardInfo->CardType = SD1.CardType;
#else
    psdCardInfo->CardType = SD0.CardType;
#endif 

    //Card Version(CSD?)
    psdCardInfo->CardVersion = 1; //dummy

    //Card Class
    psdCardInfo->CardVersion = 3; //dummy

#if defined(__M467SJHAN__)     
    //RelCardAddr(RCA)
    psdCardInfo->RelCardAdd = SD1.RCA;

    //Card Capacity in bytes
    psdCardInfo->BlockNbr = SD1.diskSize * 1024;

#else
    //RelCardAddr(RCA)
    psdCardInfo->RelCardAdd = SD0.RCA;

    //Card Capacity in bytes
    psdCardInfo->BlockNbr = SD0.diskSize * 1024;

#endif
    
    //One block size in bytes
    psdCardInfo->BlockSize = 2468;//dummy

    //Card logical Capacity in blocks
    psdCardInfo->LogBlockNbr = 137;//dummy

    //logical block size in bytes 
    psdCardInfo->LogBlockSize =258;//dummy

    return 0;

}


unsigned long get_fattime (void)
{
    unsigned long tmr;

    tmr=0x00000;

    return tmr;
}

