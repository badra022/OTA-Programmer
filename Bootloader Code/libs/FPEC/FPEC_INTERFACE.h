#ifndef FPEC_INTERFACE
#define FPEC_INTERFACE

void MFPEC_VidInit(void);
void MFPEC_VidEraseFlash(void);     /* must be called before any Writing//programming to Flash */
void MFPEC_VidWriteAllFlash(u16* Copy_Binary);
void MFPEC_VidWriteFlash(u16* Copy_Binary, u16* Copy_targetAddress, u8 Copy_u8Length);

#endif      /* FPEC_INTERFACE */
