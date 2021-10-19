/*******************************************************************************
*********** Author		: Ahmed Mohamed Badra
*********** Date		: 18/10/21			
*********** Version	: V01					
******************************************************************************/

#include "TYPES.h"
#include "MACROS.h"

#include "USART_INTERFACE.h"

#include "ESP8266_INTERFACE.h"

#include "DELAY_INTERFACE.h"

void ESP8266_VidInit ( void ){

	MUSART_VidInit();
	MUSART1_VidSetINTMode(INT_DISABLE);

	/* Sending AT Command To Check ESP8266 Is Working Or Not  */
	MUSART1_VidSendStringSynch( (u8 *)"AT\r\n" );

	_delay_ms( 1000 );

	/* Clear ESP8266 Buffer */
	ESP8266_VidClearBuffer(DataCome);

	MUSART1_VidSendStringSynch( (u8 *)"AT+CWMODE=3\r\n" );
	_delay_ms( 2000 );

	ESP8266_VidClearBuffer(DataCome);

	MUSART1_VidSendStringSynch( (u8 *)"AT+CIPMODE=0\r\n" );
	_delay_ms( 2000 );
}

void ESP8266_VidSetReceiveCallBack(void (*ptr)(void))
{
	/* Set USART1 CallBack To Receive The Response Of The ESP8266 */
	MUSART1_VidSetCallBack( ptr );
}

void ESP8266_VidConnectToWiFi ( u8 * SSID , u8 * Password ){

	ESP8266_VidClearBuffer(DataCome);

	MUSART1_VidSendStringSynch( (u8 *) "AT+CWJAP=\"" );
	MUSART1_VidSendStringSynch( (u8 *) SSID );
	MUSART1_VidSendStringSynch( (u8 *) "\",\"" );
	MUSART1_VidSendStringSynch( (u8 *) Password);
	MUSART1_VidSendStringSynch( (u8 *) "\"\r\n");
	_delay_ms( 8000 );
}

void ESP8266_VidConnectToSrvTcp ( u8 * Copy_u8Domain , u8 * Copy_u8Port ){

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSTART=\"TCP\",\"" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Domain );
	MUSART1_VidSendStringSynch( (u8 *) "\"," );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Port );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 3000 );

}

u8   ESP8266_u8ReceiveHttpReq( u8 * Copy_Request , u8 * Copy_u8Length ){

	ESP8266_VidConnectToSrvTcp( (u8 *)"162.253.155.226" , (u8 *)"80" );

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSEND=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Length );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 2000 );
	MUSART1_VidClearFlags();

	MUSART1_VidSetINTMode(RXNE_INT_ENABLE);

	MUSART1_VidSendStringSynch( (u8 *) Copy_Request );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 2000 );
	MUSART1_VidSetINTMode(INT_DISABLE);
	MUSART1_VidClearFlags();

	/*For bfota.freevar.com ( Array Index )*/
	return 38;

}

void   ESP8266_vidSendHttpReq( u8 * Copy_Request, u8 * Copy_u8Length ){

	ESP8266_VidConnectToSrvTcp( (u8 *)"162.253.155.226" , (u8 *)"80" );

	MUSART1_VidSendStringSynch( (u8 *) "AT+CIPSEND=" );
	MUSART1_VidSendStringSynch( (u8 *) Copy_u8Length );
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 2000 );
	MUSART1_VidClearFlags();

	MUSART1_VidSetINTMode(RXNE_INT_ENABLE);

	MUSART1_VidSendStringSynch(Copy_Request);
	MUSART1_VidSendStringSynch( (u8 *) "\r\n" );
	_delay_ms( 2000 );
	MUSART1_VidSetINTMode(INT_DISABLE);
	MUSART1_VidClearFlags();
}

void ESP8266_VidClearBuffer ( u8* buffer ){

	u8 LOC_u8Iterator1 = 0 ;
	Iterator     = 0 ;
	for( LOC_u8Iterator1 = 0 ; LOC_u8Iterator1 < 150 ; LOC_u8Iterator1++ ){
		buffer[ LOC_u8Iterator1 ] = 0 ;
	}
}
