/***************************************************************************************/
/****************************  IMT School Training Center ******************************/
/***************************************************************************************/
/** This file is developed by IMT School training center, All copyrights are reserved **/
/***************************************************************************************/

#include "TYPES.h"
#include "MACROS.h"

#include "USART_INTERFACE.h"

#include "ESP8266_INTERFACE.h"

#include "DELAY_INTERFACE.h"

u8 volatile Iterator = 0  ;
u8 volatile DataCome[200] ;

void UART_receiveCallBack ( void ){

	/* Receive ESP8266 Response */
	DataCome[ Iterator ] = MUSART1_u8ReadDataRegister();
	if(DataCome[ Iterator ] == '\n' && Iterator >= 82){
		if (u8BLWriteReq == 1)
		{
			MFPEC_VidEraseFlash();
			u8BLWriteReq = 0;
		}				
		/* Parse */
		Parser_voidParseRecord(&DataCome[82]);
		MUSART1_VidSetINTMode(INT_DISABLE);
		Iterator = 0;
	}
	else{
		Iterator++;
	}

	MUSART1_VidClearFlags();
	MUSART1_VidSetINTMode(RXNE_INT_ENABLE);
}

void ESP8266_VidInit ( void ){

	MUSART_VidInit();
	/* Sending AT Command To Check ESP8266 Is Working Or Not  */
	MUSART1_VidSendStringSynch( (u8 *)"AT\r\n" );

	_delay_ms( 3000 );

	/* Clear ESP8266 Buffer */
	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *)"AT+CWMODE=3\r\n" );
	_delay_ms( 3000 );

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *)"AT+CIPMODE=0\r\n" );
	_delay_ms( 3000 );
}

void ESP8266_VidSetReceiveCallBack(void (*ptr)(void))
{
	/* Set USART1 CallBack To Receive The Response Of The ESP8266 */
	MUSART1_VidSetCallBack( ptr );
}

void ESP8266_VidConnectToWiFi ( u8 * SSID , u8 * Password ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CWJAP=\"" );
	MUSART1_VidSendStringSynch( (u8 *) SSID );
	MUSART1_VidSendStringSynch( (u8 *) "\",\"" );
	MUSART1_VidSendStringSynch( (u8 *) Password);
	MUSART1_VidSendStringSynch( (u8 *) "\"\r\n");
	_delay_ms( 4000 );

}

void ESP8266_VidConnectToSrvTcp ( u8 * Copy_u8Domain , u8 * Copy_u8Port ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSTART=\"TCP\",\"" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Domain );
	MUSART1_VidSendStringSynch( (u8 *) "\"," );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Port );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 2000 );

}

void ESP8266_VidSendHttpReq( u8 * Copy_u8Key , u8 * Copy_u8Data , u8 * Copy_u8Length ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSEND=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Length );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 4000 );

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "GET /update?api_key=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Key );
	MUSART1_VidSendStringSynch( (u8 *) "&field1=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Data );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 20000 );

}

u8   ESP8266_u8ReceiveHttpReq( u8 * Copy_Request , u8 * Copy_u8Length ){

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSEND=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Length );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 1000 );

	ESP8266_VidClearBuffer();

	MUSART1_VidSendStringSynch( (u8 *) Copy_Request );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 2000 );

	ESP8266_VidConnectToSrvTcp( (u8 *)"162.253.155.226" , (u8 *)"80" );

	/*For yrabiot3.freevar.com ( Value Array Index )*/
	return  DataCome[82] ;

}

void ESP8266_VidClearBuffer ( void ){

	u8 LOC_u8Iterator1 = 0 ;
	Iterator     = 0 ;

	for( LOC_u8Iterator1 = 0 ; LOC_u8Iterator1 < 150 ; LOC_u8Iterator1++ ){

		DataCome[ LOC_u8Iterator1 ] = 0 ;

	}

}
