#ifndef __DELAY_H__
#define __DELAY_H__

/**
*	@include
**/
#include "stm32f4xx_rcc.h"


/**
*	@define
**/
#define DELAY_SYSCLK_HZ     (168000000)						// ϵͳʱ��Ƶ���£�1s�ļ���ֵ
#define DELAY_ONE_US        (DELAY_SYSCLK_HZ / 1000000)	    // ��ʱ1us����ֵ
#define DELAY_ONE_MS        (DELAY_ONE_US * 1000) 	        // ��ʱ1ms����ֵ


/**
* @function
*	���ã���ʱģ���ʼ������
*	������void
*	����ֵ��void
**/
void Delay_Init(uint32_t Rate);


/**
* @function
*	���ã�ʵ��΢�뼶��ʱ
*	������{ Us��Ҫ��ʱ��΢���� }
*	����ֵ��void
**/
void Delay_Us(uint32_t Us);


/**
* @function
*	���ã�ʵ�ֺ��뼶��ʱ
*	������{ Ms��Ҫ��ʱ�ĺ����� }
*	����ֵ��void
**/
void Delay_Ms(uint32_t Ms);

#endif