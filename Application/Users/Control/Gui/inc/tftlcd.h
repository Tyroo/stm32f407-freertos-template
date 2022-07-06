#ifndef __TFTLCD_H__
#define __TFTLCD_H__


#include "delay.h"
#include "fsmc.h"
#include "LittleVGL/core/lv_port_disp.h"
#include "LittleVGL/core/lv_port_indev.h"
#include "tftlcd_conf.h"
#include "ILI9341.h"
#include "xpt2046.h"
#include "rgb.h"


#define USE_FSMC

#ifdef USE_FSMC
#define TFTLCD_BASE_ADDR (0x6C000000|0x7e)
#define TFTLCD (*(TFTLCD_RWCD*)TFTLCD_BASE_ADDR)
#else
#define TFTLCD_CMD_MODE
#define TFTLCD_DATA_MODE
#endif

#define TFTLCD_REG(CMD) (TFTLCD.RWCD_REG = CMD)
#define TFTLCD_RAM(DATA) (TFTLCD.RWCD_RAM = DATA)

void TFTLCD_Init(void);
void TFTLCD_Touch_Init(void);
void TFTLCD_Display_Init(void);

uint16_t TFTLCD_ReadRAM(uint16_t REG_Addr);

void TFTLCD_WriteRAM(uint16_t REG_Addr, uint16_t Data);

void TFTLCD_SetWindow(uint16_t StarXPos, uint16_t EndXPos,
	uint16_t StarYPos, uint16_t EndYPos);

void TFTLCD_ReadDeviceId(void);

void TFTLCD_SetScanDir(uint16_t Dir);

void TFTLCD_SetCursor(uint16_t XPos, uint16_t YPos);

void TFTLCD_Clear(uint32_t Color);

void TFTLCD_DrawPoint(uint16_t XPos, uint16_t YPos, uint32_t Color);

void TFTLCD_DrawBlock(uint16_t StartXPos, uint16_t StartYPos, 
	uint16_t EndXPos, uint16_t EndYPos, 
	uint16_t *ColorBuff);

void TFTLCD_DrawBlock1(uint16_t StartXPos, uint16_t StartYPos, 
	uint16_t EndXPos, uint16_t EndYPos, 
	uint16_t ColorBuff);



#endif
