/*******************************************************************************
*********** Author		: Ahmed Mohamed Badra
*********** Date		: 18/10/21			
*********** Version	: V01					
******************************************************************************/
#include "TYPES.h"
#include "MACROS.h"
#include"ERROR_STATUS.h"

#include "FPEC_INTERFACE.h"
#include "FPEC_PRIVATE.h"

/**************************************************************************
 * type               : Private
 * Input Parameters   : None
 * Global Parameters  : None
 * Return             : None
 * Description        : perform the Unlocking sequence for FPEC registers to be written
 * ************************************************************************/
static void MFPEC_VidUnLock(void)
{
  /* UnLock sequence */
  FLASH->KEYR = KEY1;
  FLASH->KEYR = KEY2;
}

/**************************************************************************
 * type               : Private
 * Input Parameters   : None
 * Global Parameters  : None
 * Return             : None
 * Description        : Lock the registers of the driver
 * ************************************************************************/
static void MFPEC_VidLock(void)
{
  SET_BIT(FLASH->CR, 7); /* Set Lock Bit */
}

/**************************************************************************
 * type               : Interface
 * Input Parameters   : None
 * Global Parameters  : None
 * Return             : None
 * Description        : None
 * ************************************************************************/
void MFPEC_VidInit(void){
 /* default (empty) */
}

/**************************************************************************
 * type               : Interface
 * Input Parameters   : Copy_Binary         , holds the unsigned short data array which will be Flashed
 *                      Copy_targetAddress  , the address which the function will start flashing
 *                      Copy_u8Length       , the length of Copy_Binary
 * Global Parameters  : None
 * Return             : None
 * Description        : write the Copy_Binary data array into the Flash memory starting from Copy_targetAddress
 * ************************************************************************/
void MFPEC_VidWriteFlash(u16 Copy_Binary[], u16* Copy_targetAddress, u8 Copy_u8Length){
  u16 N = Copy_u8Length;

  /* init the targetAddress to first u16 element of the Binary file (array) */
  u16* targetAddress = Copy_targetAddress;

  if(N < 100){    /* MAX Buffer size sent */
    while(GET_BIT(FLASH->CR, 7) == HIGH){
      MFPEC_VidUnLock();
    }
    /* Loop over the Array and Write each u16 value into memory address */
    /* This part of code can be Asynch by using DMA peripheral */
    for(u16 i = 0; i< N; i++){
		/* Write Flash Programming */
		SET_BIT(FLASH->CR,0);
    	*targetAddress = Copy_Binary[i];    /* store current Byte in the target Address in the flash */

      while(GET_BIT(FLASH->SR, 0) == HIGH){}    /* wait BSY flag to be reset */

      if(*targetAddress == Copy_Binary[i]){ /* check if the value programmed successfully */
        targetAddress++;      /* move to next character */
      }
      else{
        i--; /* drawBack To same Loop */
      }

      SET_BIT(FLASH->SR, 5);    /* clear EOP (End of Operation flag) */
      CLR_BIT(FLASH->CR, 0);    /* this is an important step to Deselect the programming mode if you will execute some normal code between flash writes */
    }
  }
  MFPEC_VidLock();
}

/**************************************************************************
 * type               : Private
 * Input Parameters   : Copy_pageID     , the Flash Page number selected to be Erased
 * Global Parameters  : None
 * Return             : ErrorStatus     , OK when the Page is erased successfully
 *                                      , NOK when the Page isn't erased successfully
 * Description        : Erase the selected Page number to 0xFF and return OK if it's Erased successfully
 * ************************************************************************/
static ErrorStatus MFPEC_VidErasePage(u8 Copy_pageID){
  while(GET_BIT(FLASH->SR, 0) == HIGH){}  /* check BSY flag */
  SET_BIT(FLASH->CR, 1); /* set PER to chose PAGE Erase */
  while(GET_BIT(FLASH->CR, 7) == HIGH){
    MFPEC_VidUnLock();
  }

  u32 pageAddress = FLASH_BASE_ADDRESS + (Copy_pageID * 1024);
  FLASH->AR = pageAddress;    /* calc. Page address using Page ID and put it in FLASH_AR */
  SET_BIT(FLASH->CR, 6);    /* start operation, which is selected as page erase */
  while(GET_BIT(FLASH->SR, 0) == HIGH){}  /* check BSY flag */

  SET_BIT(FLASH->SR, 5);    /* clear EOP (End of Operation flag) */
  CLR_BIT(FLASH->CR, 1);    /* this is an important step to Deselect the programming mode if you will execute some normal code between flash writes */

  for(u32 i = 0; i<1024;i++){
    if(*(volatile u8*)(pageAddress + i) != 0x0FF){
      return NOK;
    }
  }

  /* if the Loop passed then all values of the Memory Page had been erased and equal 0xFF successfully */
  return OK;
}

/**************************************************************************
 * type               : Interface
 * Input Parameters   : None
 * Global Parameters  : None
 * Return             : None
 * Description        : Erase All Flash Pages starting From Page 8, as the Bootloader acquire the first 8 Pages from 0 --> 7
 * ************************************************************************/
void MFPEC_VidEraseFlash(void){
  u8 pageID = 8;    /* begin with fifth Page that is the start Page of the preserved Memory of the Application */

  while(pageID < 64){
    if(MFPEC_VidErasePage(pageID) == OK){       /*  Erase the Page */
      pageID++;   /* if the Page Erased successfully, Proceed to the next Page */
    }
  }

  MFPEC_VidLock();
}
