#ifndef __XPT2046_H__
#define __XPT2046_H__


#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "at24c02.h"
#include <math.h>
#include <stdio.h>


//触摸屏控制器
typedef struct
{
	uint16_t XAxis[5]; 		//当前坐标
	uint16_t YAxis[5];		//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
							//x[4],y[4]存储第一次按下时的坐标. 
	uint8_t  TouchState;	//笔的状态 
							//b7:按下1/松开0; 
	                        //b6:0,没有按键按下;1,有按键按下. 
							//b5:保留
							//b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)							
	float XFactor;			// X系数	
	float YFactor;			// Y系数
	short XOffset;			// X偏移
	short YOffset;	   		// Y偏移
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//b0:0,竖屏(适合左右为X坐标,上下为Y坐标的TP)
//   1,横屏(适合左右为Y坐标,上下为X坐标的TP) 
//b1~6:保留.
//b7:0,电阻屏
//   1,电容屏
	uint8_t ReadXAxisComd;
	uint8_t ReadYAxisComd;
	
	uint8_t TouchType;
} TFTLCD_TouchTypedef;


//电阻屏芯片连接引脚	 
#define TP_EN      PBin(1)  	//T_PEN
#define TP_CLK     PBout(0)  	//T_SCK
#define TP_MOSI    PFout(11)  	//T_MOSI
#define TP_MISO    PBin(2)  	//T_MISO
#define TP_CS      PCout(13)  	//T_CS 

// 触摸屏读写坐标命令
#define TP_CMD_RDX  (0XD0)		// 读X坐标命令
#define TP_CMD_RDY  (0X90)		// 都Y坐标命令

// 触摸屏按下状态
#define TP_PRES_DOWN 0x80  		//触屏被按下	  
#define TP_CATH_PRES 0x40  		//有按键按下了 


extern TFTLCD_TouchTypedef TFTLCD_TouchObj;

void XPT2046_Init(void);

void XPT2046_WriteData(uint8_t Data);

void XPT2046_TouchAdjust(uint8_t Mode);

uint16_t XPT2046_ReadData(uint8_t Comd);

uint16_t XPT2046_ReadAxis(uint8_t AxisComd);

uint8_t XPT2046_ReadPoint(uint16_t *pXAxis, uint16_t *pYAxis);

uint8_t XPT2046_TouchScan(void);



#endif
