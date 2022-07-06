#ifndef __FSMC_H__
#define __FSMC_H__


/**
* @include
**/
#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_rcc.h"


/**
* @define
**/
#define SRAM_START_ADDR 0x68000000


/**
* @function
**/
void FsmcSram_Init(void);
void Fsmc_AF_Config(GPIO_TypeDef* Gpio, u8* Arr, u8 Len);
void FsmcSram_Write(u16* Data, u32 StartAddr, u16 Length);
vu16 FsmcSram_Read(u32 Addr);

#endif
