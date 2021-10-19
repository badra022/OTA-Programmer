/*******************************************************************************
*********** Author		: Ahmed Mohamed Badra
*********** Date		: 18/10/21			
*********** Version	: V01					
*********** Note       : This driver developed under 
 *                        the assumption that Clock System (AHB) = 8MHZ EXT 
 *                        & The Systick Clock Is = AHB/8 
******************************************************************************/

#ifndef DELAY_INTERFACE_H_
#define DELAY_INTERFACE_H_

/* Args: Copy_u32Time which is systick counts and number of msec at the same time*/
void _delay_ms( u32 Copy_u32Time );

/* Args: Copy_u32Time which is systick counts and number of msec at the same time*/
void _delay_us( u32 Copy_u32Time );     

#endif /* DELAY_INTERFACE_H_ */
