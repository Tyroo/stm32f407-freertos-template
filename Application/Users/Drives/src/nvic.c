/** 中断分组-优先级配置模块（验证通过） **/
#include "nvic.h"


// 配置NVIC，对中断分组等等
void Nvic_Init(int Group) {
	/* 对NVIC分组 */
	uint32_t GroupDefaultValue;
	
//	/* 设置中断向量表偏移 */
//	SCB->VTOR = (FLASH_BASE | 0x2000);
//	
//	__set_PRIMASK(0);    //开启总中断
	
	switch(Group) {
		
		case 0: GroupDefaultValue = NVIC_PriorityGroup_0;break;
		case 1: GroupDefaultValue = NVIC_PriorityGroup_1;break;
		case 2: GroupDefaultValue = NVIC_PriorityGroup_2;break;
		case 3: GroupDefaultValue = NVIC_PriorityGroup_3;break;
		case 4: GroupDefaultValue = NVIC_PriorityGroup_4;break;
		default: GroupDefaultValue = NVIC_PriorityGroup_0;break;
		
	}
	
	NVIC_PriorityGroupConfig(GroupDefaultValue);
	
	/*  Other  */
	
}


void Nvic_Config(int NvicName, int PrePriority, int SubPriority, int Cmd) {
	
	NVIC_InitTypeDef NVIC_InitStructre;																	// 定义一个NVIC配置结构体
	
	NVIC_InitStructre.NVIC_IRQChannel = NvicName;												// 指定中断源名称
	NVIC_InitStructre.NVIC_IRQChannelPreemptionPriority = PrePriority;	// 设置中断的抢占优先级
	NVIC_InitStructre.NVIC_IRQChannelSubPriority = SubPriority; 				// 设置中断的响应优先级
	NVIC_InitStructre.NVIC_IRQChannelCmd = Cmd? ENABLE : DISABLE;				// 是否使能中断，1使能，0失能
	
	NVIC_Init(&NVIC_InitStructre);																			// 应用NVIC配置更改
	
}
