#ifndef FPEC_PRIVATE
#define FPEC_PRIVATE

typedef struct{
  volatile u32 ACR;
  volatile u32 KEYR;
  volatile u32 OPTKEYR;
  volatile u32 SR;
  volatile u32 CR;
  volatile u32 AR;
  volatile u32 RESERVED;
  volatile u32 OBR;
  volatile u32 WRPR;
}FLASH_t;


#define FLASH   ((FLASH_t*)0x40022000)

#define FLASH_BASE_ADDRESS                      0x08000000
#define BOOTLOADER_CODE_FLASH_BASE_ADDRESS      0x08000000
#define APPLICATION_CODE_FLASH_BASE_ADDRESS     0x08001000

#define KEY1    0x45670123
#define KEY2    0xCDEF89AB

#endif  /* FPEC_PRIVATE */