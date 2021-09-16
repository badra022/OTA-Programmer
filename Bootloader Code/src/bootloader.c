/***********************************************************
 * 	brief:	Main Application file for UART Bootloader Code 
 *	Time:	16/9/21
 *	Author: Ahmed Badra
 ***********************************************************/
#include "TYPES.h"
#include "MACROS.h"
#include"ERROR_STATUS.h"

#include "RCC_INTERFACE.h"
#include "GPIO_INTERFACE.h"
#include "SYSTICK_INTERFACE.h"
#include "USART_INTERFACE.h"
#include "ESP8266_INTERFACE.h"
#include "parse.h"

#define SCB_VTOR   *((volatile u32*)0xE000ED08)
void Parser_voidParseRecord(u8* Copy_u8BufData);

volatile u8  u8RecBuffer[100]   ;
volatile u8  u8RecCounter    = 0;
volatile u8  u8TimeOutFlag   = 0;
volatile u16 u16TimerCounter = 0;
volatile u8  u8BLWriteReq    = 1;
typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

void func(void)
{
	SCB_VTOR = 0x08001000;
	addr_to_call = *(Function_t*)(0x08001004);
	addr_to_call();
}


int main(void)
{
	u8 Local_u8RecStatus;

	MRCC_VidInit();
	MRCC_VidEnablePeripheralClock(APB2_BUS,USART1_RCC); /* USART1 */
	MRCC_VidEnablePeripheralClock(APB2_BUS,GPIOA_RCC);  /* PortA  */
	MRCC_VidEnablePeripheralClock(AHB_BUS,FPEC_RCC);   /* FPEC   */


	MGPIO_VidSetPinDirection(GPIOA,PIN9,OUTPUT_SPEED_10MHZ_AFPP);   /* TX AFPP */
	MGPIO_VidSetPinDirection(GPIOA,PIN10,INPUT_FLOATING);  /* RC Input Floating */
	
	MSTK_VidInit();

	MSTK_VidSetCallBack(func);

	MSTK_VidINTStatus(MSTK_INT_EN);

	MSTK_VidStart(15000000);

	while(u8TimeOutFlag == 0)
	{

	}
	return 0;
}

		// Local_u8RecValue = MUSART1_u8RecCharSynch( &(u8RecBuffer[u8RecCounter]) );
		// if (Local_u8RecValue != 255)
		// {
		// 	MSTK_VidStop();

		// 	if(u8RecBuffer[u8RecCounter] == '\n')
		// 	{
		// 		if (u8BLWriteReq == 1)
		// 		{
		// 			MFPEC_VidEraseFlash();
		// 			u8BLWriteReq = 0;
		// 		}
				
		// 		/* Parse */
		// 		Parser_voidParseRecord(u8RecBuffer);
		// 		MUSART1_VidSendStringSynch("ok");
		// 		u8RecCounter = 0;
		// 	}

		// 	else
		// 	{
		// 		u8RecCounter ++ ;
		// 	}

		// 	MSTK_VidStart(15000000);
		// }

		// else
		// {

		// }