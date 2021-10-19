/*******************************************************************************
*********** Author		: Ahmed Mohamed Badra
*********** Date		: 18/10/21			
*********** Version	: V01					
*********** Note       : This driver developed under 
 *                        the assumption that Clock System (AHB) = 8MHZ EXT 
 *                        & The Systick Clock Is = AHB/8 
******************************************************************************/

#include"TYPES.h"

#include"SYSTICK_INTERFACE.h"

#include"DELAY_INTERFACE.h"


void _delay_ms( u32 Copy_u32Time ){

	MSTK_VidInit();
	/* Disable INT */
	MSTK_VidINTStatus( MSTK_INT_DIS );
	MSTK_VidStart( Copy_u32Time * 1000 );
	while( MSTK_u8ReadFlag() == 0 );		/* wait until Systick Flag to be set */

}

void _delay_us( u32 Copy_u32Time ){

	MSTK_VidInit();
	/* Disable INT */
	MSTK_VidINTStatus( MSTK_INT_DIS );
	MSTK_VidStart( Copy_u32Time );
	while( MSTK_u8ReadFlag() == 0 );		/* wait until Systick Flag to be set */

}
