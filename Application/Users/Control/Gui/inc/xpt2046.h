#ifndef __XPT2046_H__
#define __XPT2046_H__


#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "at24c02.h"
#include <math.h>
#include <stdio.h>


//������������
typedef struct
{
	uint16_t XAxis[5]; 		//��ǰ����
	uint16_t YAxis[5];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
							//x[4],y[4]�洢��һ�ΰ���ʱ������. 
	uint8_t  TouchState;	//�ʵ�״̬ 
							//b7:����1/�ɿ�0; 
	                        //b6:0,û�а�������;1,�а�������. 
							//b5:����
							//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)							
	float XFactor;			// Xϵ��	
	float YFactor;			// Yϵ��
	short XOffset;			// Xƫ��
	short YOffset;	   		// Yƫ��
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP) 
//b1~6:����.
//b7:0,������
//   1,������
	uint8_t ReadXAxisComd;
	uint8_t ReadYAxisComd;
	
	uint8_t TouchType;
} TFTLCD_TouchTypedef;


//������оƬ��������	 
#define TP_EN      PBin(1)  	//T_PEN
#define TP_CLK     PBout(0)  	//T_SCK
#define TP_MOSI    PFout(11)  	//T_MOSI
#define TP_MISO    PBin(2)  	//T_MISO
#define TP_CS      PCout(13)  	//T_CS 

// ��������д��������
#define TP_CMD_RDX  (0XD0)		// ��X��������
#define TP_CMD_RDY  (0X90)		// ��Y��������

// ����������״̬
#define TP_PRES_DOWN 0x80  		//����������	  
#define TP_CATH_PRES 0x40  		//�а��������� 


extern TFTLCD_TouchTypedef TFTLCD_TouchObj;

void XPT2046_Init(void);

void XPT2046_WriteData(uint8_t Data);

void XPT2046_TouchAdjust(uint8_t Mode);

uint16_t XPT2046_ReadData(uint8_t Comd);

uint16_t XPT2046_ReadAxis(uint8_t AxisComd);

uint8_t XPT2046_ReadPoint(uint16_t *pXAxis, uint16_t *pYAxis);

uint8_t XPT2046_TouchScan(void);



#endif