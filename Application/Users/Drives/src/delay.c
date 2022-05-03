/* 延时模块（验证通过） */
#include "delay.h"


// 延时模块初始化函数
void Delay_Init(uint32_t Rate) 
{
	
	uint32_t ReloadCount;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	// 设置SysTick的时钟(168MHz)
	
	ReloadCount = DELAY_SYSCLK_HZ / Rate;
	
	SysTick->LOAD = ReloadCount;                 // 计数值加载
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   // 开启系统滴答中断
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;    // 开始倒数
	
}


// 实现微秒级延时
void Delay_Us(uint32_t Us) 
{
	
	uint32_t CountValue;
	uint32_t CountValueOld, CountValueNew;
	uint32_t StopCountValue = Us * DELAY_ONE_US;    // 计算出延时Us微秒需要计数的值
	
	CountValueOld = SysTick->VAL;

	// 当systick定时器递减计数器未减至0且减计数器处于开启时一直阻塞
	do 
	{
		CountValueNew = SysTick->VAL;
		CountValue += (SysTick->LOAD + CountValueOld - CountValueNew) % SysTick->LOAD;
		CountValueOld = CountValueNew;
	} 
	while(CountValue < StopCountValue);
}


// 实现毫秒级延时
void Delay_Ms(uint32_t Ms) 
{
	
	uint32_t CountValue;
	uint32_t CountValueOld, CountValueNew;
	uint32_t StopCountValue = Ms * DELAY_ONE_MS;    // 计算出延时Us微秒需要计数的值
	
	CountValueOld = SysTick->VAL;

	// 当systick定时器递减计数器未减至0且减计数器处于开启时一直阻塞
	do 
	{
		CountValueNew = SysTick->VAL;
		CountValue += (SysTick->LOAD + CountValueOld - CountValueNew) % SysTick->LOAD;
		CountValueOld = CountValueNew;
	} 
	while(CountValue < StopCountValue);
}

