#include "TYPES.h"
#include "MACROS.h"
#include"ERROR_STATUS.h"

#include "FPEC_INTERFACE.h"
#include "FPEC_PRIVATE.h"

static void MFPEC_VidUnLock(void)
{
  /* UnLock sequence */
  FLASH->KEYR = KEY1;
  FLASH->KEYR = KEY2;
}
static void MFPEC_VidLock(void)
{
  SET_BIT(FLASH->CR, 7); /* Set Lock Bit */
}

void MFPEC_VidInit(void){
 /* default (empty) */
}

void MFPEC_VidWriteFlash(u16* Copy_Binary){
  u16 N = sizeof(Copy_Binary)/sizeof(*Copy_Binary);

  /* init the targetAddress to first u16 element of the Binary file (array) */
  u16* targetAddress = APPLICATION_CODE_FLASH_BASE_ADDRESS;

  if(N < 60000){    /* MAX Code Size is 60 Kbytes */
    while(GET_BIT(FLASH->CR, 7) == HIGH){
      MFPEC_VidUnLock();
    }

    SET_BIT(FLASH->CR, 2); /* Set PG bit to select flashing or Programming Mode */

    /* Loop over the Array and Write each u16 value into memory address */
    /* This part of code can be Asynch by using DMA peripheral */
    for(u16 i = 0; i<= N; i++){
      *targetAddress = Copy_Binary[i];    /* store current character in the target Address in the flash */

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

ErrorStatus MFPEC_VidWriteFlash(u16 Copy_Binary, u16* Copy_targetAddress){
      ErrorStatus returnState = OK;
      while(GET_BIT(FLASH->CR, 7) == HIGH){
      MFPEC_VidUnLock();
      }

      SET_BIT(FLASH->CR, 2); /* Set PG bit to select flashing or Programming Mode */

      *Copy_targetAddress = Copy_Binary;    /* store current character in the target Address in the flash */

      while(GET_BIT(FLASH->SR, 0) == HIGH){}    /* wait BSY flag to be reset */

      if(*Copy_targetAddress != Copy_Binary){ /* check if the value programmed successfully */
        returnState = NOK;
      }

      SET_BIT(FLASH->SR, 5);    /* clear EOP (End of Operation flag) */
      CLR_BIT(FLASH->CR, 0);    /* this is an important step to Deselect the programming mode if you will execute some normal code between flash writes */

      return returnState;
}

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

  ErrorStatus state = OK;

  for(u32 i = 0; i<1024;i++){
    if(*(volatile u8*)(pageAddress + i) != 0x0FF){
      return NOK;
    }
  }

  /* if the Loop passed then all values of the Memory Page had been erased and equal 0xFF successfully */
  return OK;
}

void MFPEC_VidEraseFlash(void){
  u8 pageID = 4;    /* begin with fifth Page that is the start Page of the preserved Memory of the Application */

  while(pageID < 64){
    ErrorStatus state = MFPEC_VidErasePage(pageID);   /*  Erase the Page */
    if(state == OK){
      pageID++;   /* if the Page Erased successfully, Proceed to the next Page */
    }
  }

  MFPEC_VidLock();
}
