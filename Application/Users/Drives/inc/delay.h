#ifndef __DELAY_H__
#define __DELAY_H__

/**
*	@include
**/
#include "stm32f4xx_rcc.h"


/**
*	@define
**/
#define DELAY_SYSCLK_HZ     (168000000)						// 系统时钟频率下，1s的计数值
#define DELAY_ONE_US        (DELAY_SYSCLK_HZ / 1000000)	    // 延时1us计数值
#define DELAY_ONE_MS        (DELAY_ONE_US * 1000) 	        // 延时1ms计数值


/**
* @function
*	作用：延时模块初始化函数
*	参数：void
*	返回值：void
**/
void Delay_Init(uint32_t Rate);


/**
* @function
*	作用：实现微秒级延时
*	参数：{ Us：要延时的微秒数 }
*	返回值：void
**/
void Delay_Us(uint32_t Us);


/**
* @function
*	作用：实现毫秒级延时
*	参数：{ Ms：要延时的毫秒数 }
*	返回值：void
**/
void Delay_Ms(uint32_t Ms);

#endif
