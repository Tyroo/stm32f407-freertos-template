#include "fsmc.h"


void FsmcSram_Init(void) {
	
	u8 GPIOD_AF_Array[12] = {0,1,4,5,8,9,10,11,12,13,14,15};
	u8 GPIOE_AF_Array[11] = {0,1,7,8,9,10,11,12,13,14,15};
	u8 GPIOF_AF_Array[10] = {0,1,2,3,4,5,12,13,14,15};
	u8 GPIOG_AF_Array[7]  = {0,1,2,3,4,5,10};
	
	/* 定义配置结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_InitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTiming; 

	/* 时钟使能/失能 */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* 配置GPIO */
	// GPIOB配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;						//PB15 推挽输出,控制背光
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化 //PB15 推挽输出,控制背光
	// GPIOD配置
	GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(0XFF<<8);// PD0,1,4,5,8~15,NOE,NWE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					// 复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		// 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// 上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);								// 初始化数据线
	// GPIOE配置
	GPIO_InitStructure.GPIO_Pin = (3<<0)|(0X1FF<<7);			// PE0,1,7~15,NBL0,NBL1
	GPIO_Init(GPIOE, &GPIO_InitStructure);								// 初始化数据线
	// GPIOF配置
	GPIO_InitStructure.GPIO_Pin = (0X3F<<0)|(0XF<<12);		// PB15
	GPIO_Init(GPIOF, &GPIO_InitStructure);								// 初始化地址线
	// GPIOG配置	
	GPIO_InitStructure.GPIO_Pin =(0X3F<<0)| GPIO_Pin_10;	// PG0~5,10,NE3
	GPIO_Init(GPIOG, &GPIO_InitStructure);								// 初始化地址线
	
	/* GPIO复用配置 */
	Fsmc_AF_Config(GPIOD, GPIOD_AF_Array, 12);
	Fsmc_AF_Config(GPIOE, GPIOE_AF_Array, 11);
	Fsmc_AF_Config(GPIOF, GPIOF_AF_Array, 10);
	Fsmc_AF_Config(GPIOG, GPIOG_AF_Array, 7);
	
	/* FSMC配置 */
	// FSMC读写时序配置
	FSMC_ReadWriteTiming.FSMC_AddressSetupTime = 0x00; //地址建立时间为 2 个 HCLK
	FSMC_ReadWriteTiming.FSMC_AddressHoldTime = 0x00; //地址保持时间模式 A 未用到
	FSMC_ReadWriteTiming.FSMC_DataSetupTime = 0x08; //数据保持时间为 9个 HCLK
	FSMC_ReadWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadWriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadWriteTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //模式 A 
	// FSMC配置
	FSMC_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;											// 设置FSMC管理的外部SRAM的地址（Bank1的块3）
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
	FSMC_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;					// 内存类型为SRAM
	FSMC_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; //存储器数据宽度为8bit 
	FSMC_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	FSMC_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; 
	FSMC_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//存储器写使能
	FSMC_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 
	FSMC_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; 	// 读写使用相同的时序
	FSMC_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTiming;
	FSMC_InitStructure.FSMC_WriteTimingStruct = &FSMC_ReadWriteTiming; 	//读写同样时序
	
	/* 使能/失能 */
	FSMC_NORSRAMInit(&FSMC_InitStructure); 			//初始化 FSMC 配置
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);	// 使能 BANK1 区域 3

}


void FsmcSram_Write(u16* Data, u32 StartAddr, u16 Length) {
	
	u16 DataIndex;
	
	for(DataIndex=0;DataIndex<Length;DataIndex+=2) {
		*(vu16*)(SRAM_START_ADDR+StartAddr+DataIndex) = *(vu16*)(Data+DataIndex);
	}
}


vu16 FsmcSram_Read(u32 Addr) {
	return *(vu16*)(SRAM_START_ADDR+Addr);
}


void Fsmc_AF_Config(GPIO_TypeDef* Gpio, u8* Arr, u8 Len) {
	int i;
	for(i=0;i<Len;i++) {
		GPIO_PinAFConfig(Gpio, *(Arr+i), GPIO_AF_FSMC);
	}
}
