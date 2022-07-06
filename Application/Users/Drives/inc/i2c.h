#ifndef __I2C_H__
#define __I2C_H__


/**
* @include
**/
#include "stm32f4xx.h"
#include "delay.h"
#include "system.h"


/**
*	@define
**/
// I2C为发送模式
#define IIC_Mode_Tx() {GPIOB->MODER &= ~(3<<18);GPIOB->MODER |= 1<<9*2;}
// I2C为接收模式
#define IIC_Mode_Rx() {GPIOB->MODER &= ~(3<<18);GPIOB->MODER |= 0<<9*2;}
// GPIOB引脚映射IIC线
#define IIC_SCL     		PBout(8)	// IIC时钟线
#define IIC_SDA_OUT     	PBout(9)	// IIC数据输出线
#define IIC_SDA_IN			PBin(9)		// IIC数据读入线
#define IIC_DATA_SIZE_MAX 	32			// IIC接收数据最大长度（字节）


void IIC_Init(void);

static void IIC_TxReply(uint8_t Res);
static bool IIC_RxReply(void);

void IIC_Stop(void);
void IIC_Start(void);

bool IIC_Send_Byte(uint8_t Data);
uint8_t IIC_Read_Byte(uint8_t IsAck);

#endif
