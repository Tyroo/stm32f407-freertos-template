#ifndef __LED_H__
#define __LED_H__

/**
*	@include
**/
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define LED_0     GPIO_Pin_9
#define LED_1     GPIO_Pin_10


/**
*	@function
**/
void Led_Init(void);

void Led_Control(uint8_t Status, uint16_t Led);



#endif
