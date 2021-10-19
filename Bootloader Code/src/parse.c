/***********************************************************
 * 	brief:	Helper Functions file for the main Application
 *	Time:	18/10/21
 *	Author: Ahmed Badra
 ***********************************************************/
#include "TYPES.h"
#include "MACROS.h"
#include"ERROR_STATUS.h"

#include "FPEC_INTERFACE.h"
#include "parse.h"

u16 Data[100];
u32 Address= 0x08000000;

static u8 AsciToHex(u8 Copy_u8Asci)
{
	u8 Result;
	if ( (Copy_u8Asci >= 48) && (Copy_u8Asci <= 57) )
	{
		Result = Copy_u8Asci - 48;
	}

	else
	{
		Result = Copy_u8Asci - 55;
	}

	return Result;
}


static void ParseData(u8* Copy_u8BufData)
{
	u8 DigitLow,DigitHigh,CC,i;
	u8 DataDigit0,DataDigit1,DataDigit2,DataDigit3;
	u8 DataCounter = 0;

	/* Get Character Count */
	DigitHigh = AsciToHex (Copy_u8BufData[1]);
	DigitLow  = AsciToHex (Copy_u8BufData[2]);
	CC        = (DigitHigh<<4) | DigitLow ;

	/* Get Address */
	DataDigit0 = AsciToHex (Copy_u8BufData[3]);
	DataDigit1 = AsciToHex (Copy_u8BufData[4]);
	DataDigit2 = AsciToHex (Copy_u8BufData[5]);
	DataDigit3 = AsciToHex (Copy_u8BufData[6]);

	/* Clear Low Part of Address */
	Address = Address & 0xFFFF0000;
	Address = Address | (DataDigit3) | (DataDigit2 << 4) | (DataDigit1 << 8) | (DataDigit0<<12);

	for (i=0;i<CC/2; i++)
	{
		DataDigit0 = AsciToHex (Copy_u8BufData[4*i+9 ]);
		DataDigit1 = AsciToHex (Copy_u8BufData[4*i+10]);
		DataDigit2 = AsciToHex (Copy_u8BufData[4*i+11]);
		DataDigit3 = AsciToHex (Copy_u8BufData[4*i+12]);

		Data[DataCounter] = (DataDigit3 << 8) | (DataDigit2 << 12) | (DataDigit1) | (DataDigit0<<4);
		DataCounter++;
	}
	MFPEC_VidWriteFlash(Data,(u16*)(Address | 0x08000000), DataCounter);
}

static void ParseUpperAddress(u8* Copy_u8BufData)
{
	u8 DataDigit0,DataDigit1,DataDigit2,DataDigit3;

	/* Get Address */
	DataDigit0 = AsciToHex (Copy_u8BufData[3]);
	DataDigit1 = AsciToHex (Copy_u8BufData[4]);
	DataDigit2 = AsciToHex (Copy_u8BufData[5]);
	DataDigit3 = AsciToHex (Copy_u8BufData[6]);

	/* Clear Low Part of Address */
	Address = Address & (u32)0x0000FFFF;
	Address = Address | (u32)((u32)(DataDigit3 << 28) | (u32)(DataDigit2 << 24) | (u32)(DataDigit1 << 20) | (u32)(DataDigit0 << 16));
}

static void Parser_voidParseRecord(u8* Copy_u8BufData)
{
	switch (Copy_u8BufData[8])
	{
	case '0': ParseData(Copy_u8BufData); break;
	case '4': ParseUpperAddress(Copy_u8BufData); break;
	}
}

void Parser_voidParseBlock(u8* buffer, u16 endIdx){
	u16 idx = 0;
	Parser_voidParseRecord(buffer);
	while(idx < endIdx){
		while(buffer[idx] != '\n'){
			idx++;
		}
		Parser_voidParseRecord(&buffer[++idx]);
	}
}