#ifndef __USART_H__
#define __USART_H__


/**
* @include
**/
#include "stm32f4xx_usart.h"
#include "stdio.h"
#include "nvic.h"


/**
* @define
**/
#define USE_PRINTF_FUNC 1


/**
* @variable
**/
extern char UsartReceiveData[50];



/**
* @function
* 作用：初始化UART1
* 参数：{ Baud: UART的波特率 }
* 返回值：void
**/
void Uart1_Init(uint32_t Baud);


/**
* @function
* 作用：使用USART1发送数据
* 参数：{ Data: 要发送的数据指针 }
* 返回值：void
**/
void Uart1_Send(char *Data);



#endif
