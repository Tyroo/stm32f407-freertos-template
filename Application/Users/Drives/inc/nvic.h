#ifndef __NVIC_H__
#define __NVIC_H__

/**
* @include
**/
#include "misc.h"


/**
* @function
* 作用：配置NVIC，对中断分组等等
* 参数：Group
* 返回值：void
**/
void Nvic_Init(int Group);


/**
* @function
* 作用：配置USART1的中断优先级、使能等
* 参数：{ PrePriority：抢占优先级；SubPriority：响应优先级；Cmd：是否使能标志位 }
* 返回值：void
**/
void Nvic_Config(int NvicName, int PrePriority, int SubPriority, int Cmd);

#endif

