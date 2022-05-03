#include "led.h"


// 初始化LED模块
void Led_Init() {
	
	/* 配置结构体定义 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 时钟使能 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	// 使能PC端口时钟
	
	/* GPIOF配置 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				// 设置该GPIO为输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					// 设置为10口
	GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed;	// 输出速度为中速
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// 推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		// 不使用引脚上下拉
	
	/* 应用GPIO配置 */
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}


// 设置LED灯状态
void Led_Control(uint8_t Status) {
	
	if (Status&1)
		GPIO_ResetBits(GPIOF, GPIO_Pin_10);	// 开启LED灯
	else
		GPIO_SetBits(GPIOF, GPIO_Pin_10);		// 关闭LED灯
		
}
