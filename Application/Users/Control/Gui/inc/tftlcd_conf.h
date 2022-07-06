#ifndef __TFTLCD_CONF_H__
#define __TFTLCD_CONF_H__

#include "stm32f4xx.h"


// TFTLCD命令/数据读写结构体
typedef struct {
	
	uint16_t RWCD_REG;
	uint16_t RWCD_RAM;
	
} TFTLCD_RWCD;


// TFTLCD命令集
typedef struct {
	
	uint8_t CMD_Read_ID;			// 获取ID指令
	
	uint8_t CMD_Write_ScanDir;// 设置扫描方向
	
	uint8_t CMD_Write_XPos;	  // X坐标设置指令
	uint8_t CMD_Write_YPos;		// Y坐标设置指令
	
	uint8_t CMD_Write_ColorCode;
	uint8_t CMD_Read_ColorCode;
	
} TFTLCD_CMD;


// TFTLCD配置结构体
typedef struct {
	
	uint16_t TFTLCD_Width;	// 屏幕宽度
	uint16_t TFTLCD_Height;	// 屏幕高度
	uint16_t TFTLCD_Id;		// 屏幕IC型号
	uint8_t TFTLCD_Dir;		// 屏幕显示方向
	TFTLCD_CMD TFTLCD_Cmd;	// TFTLCD屏幕IC的指令集
	
} TFTLCD_Config;


#endif
