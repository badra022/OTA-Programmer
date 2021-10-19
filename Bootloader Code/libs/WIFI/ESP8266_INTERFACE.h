/*******************************************************************************
*********** Author		: Ahmed Mohamed Badra
*********** Date		: 18/10/21			
*********** Version	: V01					
******************************************************************************/


#ifndef ESP8266_INTERFACE_H_
#define ESP8266_INTERFACE_H_

extern u16 volatile Iterator;
extern u8 DataCome[1400];

void ESP8266_VidInit            ( void                                                    ) ;
void ESP8266_VidConnectToWiFi   ( u8 * SSID , u8 * Password                               ) ;
void ESP8266_VidConnectToSrvTcp ( u8 * Copy_u8Domain , u8 * Copy_u8Port                   ) ;
void ESP8266_VidSendHttpReq     ( u8 * Copy_u8Key , u8 * Copy_u8Data , u8 * Copy_u8Length ) ;
u8   ESP8266_u8ReceiveHttpReq   ( u8 * Copy_u8ChannelID , u8 * Copy_u8Length              ) ;
void ESP8266_VidSetReceiveCallBack(void (*ptr)(void)                                      ) ;

void ESP8266_VidClearBuffer     ( u8*   buffer                                            ) ;

#endif /* ESP8266_INTERFACE_H_ */
