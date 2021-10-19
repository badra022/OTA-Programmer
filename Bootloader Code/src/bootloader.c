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
#include "NVIC_INTERFACE.h"
#include "USART_INTERFACE.h"
#include "ESP8266_INTERFACE.h"
#include "FPEC_INTERFACE.h"

#include "parse.h"
#include "wifi_access.h"	/* containing Private WIFI access Data */

#define SCB_VTOR   *((volatile u32*)0xE000ED08)

volatile u8  u8BLWriteReq    = 1;
volatile u8 dataComeBeginIdx = 32;

typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

void func(void)
{
	SCB_VTOR = 0x08002000;
	addr_to_call = *(Function_t*)(0x08002004);
	addr_to_call();
}

u8 ascii_to_decimal(u8 Copy_character){
	if ( (Copy_character >= '0') && (Copy_character <= '9') )
	{
		return Copy_character - 48;
	}
	else{
		return -1;
	}
}

u8 decimal_to_ascii(u8 Copy_number){
	if (Copy_number <= 9)
	{
		return Copy_number + 48;
	}
	return -1;
}

u8 numBuffer[3];
u8* number_to_string(u8 Copy_number){
	if(Copy_number >= 10){
		numBuffer[0] = decimal_to_ascii(Copy_number/10);
		numBuffer[1] = decimal_to_ascii(Copy_number%10);
		numBuffer[2] = '\0';
	}
	else{
		numBuffer[0] = decimal_to_ascii(Copy_number);
		numBuffer[1] = '\0';
	}
	return numBuffer;
}

u16 volatile Iterator = 0  ;
u8 volatile numHexLines = 0; /* till the end of the hex file */
u8 volatile numBatchLines = 0; /* till 30 */
u16 volatile file_length = -1;		/* will be set from the server */
u8 DataCome[1400] ;	/* buffer to receive the hex datafile */
bool volatile READ_FLAG_REQUEST = TRUE;
bool volatile READ_BINARY_BLOCK_REQUEST = FALSE;
bool volatile RESPONSE_STATE = FALSE;

void ESP8266_Response( void ){
	MGPIO_VidSetPinValue(GPIOA, PIN2, HIGH);
	if(READ_BINARY_BLOCK_REQUEST){

		/* Receive ESP8266 Response */
		DataCome[ Iterator ] = MUSART1_u8ReadDataRegister();
		if(DataCome[ Iterator ] == '\n' && Iterator >= 40){
			numHexLines++;
			numBatchLines++;
		}
		if(DataCome[40] != '<'){
			if(numHexLines >= file_length || numBatchLines >= 30){
				if (u8BLWriteReq == 1)
				{
					MFPEC_VidEraseFlash();
					u8BLWriteReq = 0;
				}
				/* Parse */
				RESPONSE_STATE = FALSE;
				MUSART1_VidSetINTMode(INT_DISABLE);
				u8* beginAddress = ((numBatchLines < 23)?((u8 *)&DataCome[40]):(u8 *)&(DataCome[41]));
				Parser_voidParseBlock(beginAddress, Iterator);
				Iterator = 0;
				numBatchLines = 0;
			}
			else{
				Iterator++;
			}
		}
		else{
			RESPONSE_STATE = TRUE;
			MUSART1_VidSetINTMode(INT_DISABLE);
		}
		MUSART1_VidClearFlags();
	}

	else if(READ_FLAG_REQUEST){
		/* Receive ESP8266 Response */
		DataCome[ Iterator ] = MUSART1_u8ReadDataRegister();
		if(Iterator <= 50){
				Iterator++;
			}
		else{
				Iterator = 0;
				MUSART1_VidSetINTMode(INT_DISABLE);
			}
			MUSART1_VidClearFlags();
	}
}

void setFileLength(u8 idx){
	file_length = 0x0000;
	while(DataCome[idx] != '\n' && DataCome[idx] != 'C' && DataCome[idx] != '\r'){
		file_length *= 10;
		u8 number = ascii_to_decimal(DataCome[idx]);
		file_length += number;
		idx = idx + 1;
	}
}

static u8* string_concat(u8* dest, u8* src){
	u8 idx = 0;
	while(dest[idx] != '$'){
		idx++;
	}
	u8 idx_src = 0;
	while(src[idx_src] != '\0'){
		dest[idx] = src[idx_src];
		idx++;
		idx_src++;
	}
	dest[idx] = '\0';
	return dest;
}

int main(void)
{

	MRCC_VidInit();
	MRCC_VidEnablePeripheralClock(APB2_BUS,USART1_RCC); /* USART1 */
	MRCC_VidEnablePeripheralClock(APB2_BUS,GPIOA_RCC);  /* PortA  */
	MRCC_VidEnablePeripheralClock(AHB_BUS,FPEC_RCC);   /* FPEC   */
	MNVIC_VidEnablePeripheral(37);		/* NVIC enable the USART1 ints */

	MGPIO_VidSetPinDirection(GPIOA,PIN9,OUTPUT_SPEED_10MHZ_AFPP);   /* TX AFPP */
	MGPIO_VidSetPinDirection(GPIOA,PIN10,INPUT_FLOATING);  /* RC Input Floating */
	MGPIO_VidSetPinDirection(GPIOA,PIN1,OUTPUT_SPEED_10MHZ_PP);  /* LED */
	MGPIO_VidSetPinDirection(GPIOA,PIN2,OUTPUT_SPEED_10MHZ_PP);  /* LED */

	MSTK_VidInit();

	ESP8266_VidInit();
	ESP8266_VidConnectToWiFi((u8* )WIFI_NAME, (u8 *)WIFI_PASSWORD);
	ESP8266_VidSetReceiveCallBack(ESP8266_Response);

	while(1)
	{
		MGPIO_VidSetPinValue(GPIOA, PIN1, LOW);

		dataComeBeginIdx = ESP8266_u8ReceiveHttpReq((u8*)"GET http://bfota.freevar.com/flag.txt", (u8 *)"39");

		if(DataCome[dataComeBeginIdx] == '1')
		{
			ESP8266_VidClearBuffer(DataCome);
			dataComeBeginIdx = ESP8266_u8ReceiveHttpReq((u8*)"GET http://bfota.freevar.com/uploads/", (u8 *)"39");
			setFileLength(dataComeBeginIdx);
			READ_BINARY_BLOCK_REQUEST = TRUE;
			READ_FLAG_REQUEST = FALSE;
			ESP8266_VidClearBuffer(DataCome);
			u8 blockIdx = 0;
			while(TRUE){
				u8* str_idx = number_to_string(blockIdx);
				u8* command_length = blockIdx >= 10?(u8*)"47":(u8*)"46";
				do{
					u8 request[] = "GET http://bfota.freevar.com/parser/?Block=$$";
					string_concat(request, str_idx);
					ESP8266_vidSendHttpReq(request, command_length);
					ESP8266_VidClearBuffer(DataCome);
				}while(RESPONSE_STATE == TRUE);
				if(MGPIO_u8GetPinValue(GPIOA, PIN1) == HIGH){
					MGPIO_VidSetPinValue(GPIOA, PIN1, LOW);
				}
				else{
					MGPIO_VidSetPinValue(GPIOA, PIN1, HIGH);
				}
				blockIdx++;

				if(numHexLines >= file_length){
					func();			/* head to the flashed code */
				}
			}
		}
		else if(DataCome[dataComeBeginIdx] == '0'){
			ESP8266_VidClearBuffer(DataCome);
			MGPIO_VidSetPinValue(GPIOA, PIN1, LOW);
		}
	}
	return 0;
}
