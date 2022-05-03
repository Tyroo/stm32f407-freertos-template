/* ��ʱģ�飨��֤ͨ���� */
#include "delay.h"


// ��ʱģ���ʼ������
void Delay_Init(uint32_t Rate) 
{
	
	uint32_t ReloadCount;
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	// ����SysTick��ʱ��(168MHz)
	
	ReloadCount = DELAY_SYSCLK_HZ / Rate;
	
	SysTick->LOAD = ReloadCount;                 // ����ֵ����
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   // ����ϵͳ�δ��ж�
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;    // ��ʼ����
	
}


// ʵ��΢�뼶��ʱ
void Delay_Us(uint32_t Us) 
{
	
	uint32_t CountValue;
	uint32_t CountValueOld, CountValueNew;
	uint32_t StopCountValue = Us * DELAY_ONE_US;    // �������ʱUs΢����Ҫ������ֵ
	
	CountValueOld = SysTick->VAL;

	// ��systick��ʱ���ݼ�������δ����0�Ҽ����������ڿ���ʱһֱ����
	do 
	{
		CountValueNew = SysTick->VAL;
		CountValue += (SysTick->LOAD + CountValueOld - CountValueNew) % SysTick->LOAD;
		CountValueOld = CountValueNew;
	} 
	while(CountValue < StopCountValue);
}


// ʵ�ֺ��뼶��ʱ
void Delay_Ms(uint32_t Ms) 
{
	
	uint32_t CountValue;
	uint32_t CountValueOld, CountValueNew;
	uint32_t StopCountValue = Ms * DELAY_ONE_MS;    // �������ʱUs΢����Ҫ������ֵ
	
	CountValueOld = SysTick->VAL;

	// ��systick��ʱ���ݼ�������δ����0�Ҽ����������ڿ���ʱһֱ����
	do 
	{
		CountValueNew = SysTick->VAL;
		CountValue += (SysTick->LOAD + CountValueOld - CountValueNew) % SysTick->LOAD;
		CountValueOld = CountValueNew;
	} 
	while(CountValue < StopCountValue);
}
