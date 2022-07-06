#include "tftlcd.h"


TFTLCD_Config TFTLCD_Conf;


void TFTLCD_Init(void)
{
	TFTLCD_Display_Init();	// 触摸屏显示初始化
	TFTLCD_Touch_Init();	// 触摸屏触控初始化
}


void TFTLCD_Touch_Init(void)
{
	XPT2046_Init();
	XPT2046_TouchAdjust(1);
}


#ifdef USE_FSMC
/* 使用FSMC的情况下 */

// TFTLCD初始化函数
void TFTLCD_Display_Init(void) {
	
	u8 GPIOD_AF_Array[9] = {0,1,4,5,8,9,10,14,15};
	u8 GPIOE_AF_Array[9] = {7,8,9,10,11,12,13,14,15};
	u8 GPIOF_AF_Array[1] = {12};
	u8 GPIOG_AF_Array[1] = {12};
	
	/* 定义配置结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_InitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef FSMC_WriteTiming;

	/* 时钟使能/失能 */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	/* 配置GPIO */
	// GPIOB配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;						// PB15 推挽输出,控制背光
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					// 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			// 50MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// 上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);								// 初始化 //PB15 推挽输出,控制背光
	GPIO_SetBits(GPIOB, GPIO_Pin_15);											// 开启背光
	// GPIOD配置
	GPIO_InitStructure.GPIO_Pin =  (0x33)|(7<<8)|(3<<14); // PD0,1,4,5,8~15,NOE,NWE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					// 复用输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		// 100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// 上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);								// 初始化数据线
	// GPIOE配置
	GPIO_InitStructure.GPIO_Pin =  (0X1FF<<7);			// PE0,1,7~15,NBL0,NBL1
	GPIO_Init(GPIOE, &GPIO_InitStructure);					// 初始化数据线
	// GPIOF配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			// PB15
	GPIO_Init(GPIOF, &GPIO_InitStructure);					// 初始化地址线
	// GPIOG配置	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;				// PG0~5,10,NE3
	GPIO_Init(GPIOG, &GPIO_InitStructure);					// 初始化地址线
	
	/* GPIO复用配置 */
	Fsmc_AF_Config(GPIOD, GPIOD_AF_Array, 9);
	Fsmc_AF_Config(GPIOE, GPIOE_AF_Array, 9);
	Fsmc_AF_Config(GPIOF, GPIOF_AF_Array, 1);
	Fsmc_AF_Config(GPIOG, GPIOG_AF_Array, 1);
	
	/* FSMC配置 */
	// FSMC读写时序配置
	FSMC_ReadWriteTiming.FSMC_AddressSetupTime = 0xF; //地址建立时间为 16 个 HCLK
	FSMC_ReadWriteTiming.FSMC_AddressHoldTime = 0x00; //地址保持时间模式 A 未用到
	FSMC_ReadWriteTiming.FSMC_DataSetupTime = 60; 	//数据保存时间为 25个 HCLK
	FSMC_ReadWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadWriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadWriteTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //模式 A 
	// FSMC写时序
	FSMC_WriteTiming.FSMC_AddressSetupTime = 0x03; //地址建立时间为 3 个 HCLK
	FSMC_WriteTiming.FSMC_AddressHoldTime = 0x00; //地址保持时间模式 A 未用到
	FSMC_WriteTiming.FSMC_DataSetupTime = 0x02; 	//数据保存时间为 3个 HCLK
	FSMC_WriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_WriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_WriteTiming.FSMC_DataLatency = 0x00;
	FSMC_WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //模式 A 
	// FSMC配置
	FSMC_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;						// 设置FSMC管理的外部SRAM的地址（Bank1的块4）
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;	// 关闭数据地址复用
	FSMC_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;				// 内存类型为SRAM
	FSMC_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; 	// 存储器数据宽度为8bit 
	FSMC_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	FSMC_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; 
	FSMC_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	// 存储器写使能
	FSMC_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 
	FSMC_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 		// 读写使用不相同的时序
	FSMC_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTiming;	// 读写时序不相同
	FSMC_InitStructure.FSMC_WriteTimingStruct = &FSMC_WriteTiming;			// 读写时序不相同
	
	/* 使能/失能 */
	FSMC_NORSRAMInit(&FSMC_InitStructure);									// 初始化FSMC配置
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);							// 使能BANK1区域4
	
	/* TFTLCD设备初始化 */
	TFTLCD_Conf.TFTLCD_Dir = 0;		// 竖屏模式
	TFTLCD_Conf.TFTLCD_Height = MY_DISP_VER_RES;// 屏幕高度320
	TFTLCD_Conf.TFTLCD_Width = MY_DISP_HOR_RES; // 屏幕宽度240
	TFTLCD_Conf.TFTLCD_Cmd = (TFTLCD_CMD){0xD3,0x36,0x2A,0x2B,0x2C,0x2E};	// IC指令

	TFTLCD_ReadDeviceId();								// 获取TFTLCD IC ID
	ILI9341_Init(&TFTLCD.RWCD_REG, &TFTLCD.RWCD_RAM);	// ILI9341初始化
	TFTLCD_SetScanDir(TFTLCD_Conf.TFTLCD_Dir);			// 设置屏幕方向
	TFTLCD_SetWindow(0, TFTLCD_Conf.TFTLCD_Width-1, 0, TFTLCD_Conf.TFTLCD_Height-1);	// 在屏幕上开窗
	
	TFTLCD_Clear(0xFFFFFF);	
}

 
// 以指定命令读TFTLCD RAM
uint16_t TFTLCD_ReadRAM(uint16_t REG_Addr) {
	TFTLCD.RWCD_REG = REG_Addr;
	Delay_Us(5);
	return TFTLCD.RWCD_RAM;
}


// 以指定指令写TFTLCD RAM
void TFTLCD_WriteRAM(uint16_t REG_Addr, uint16_t Data) {
	TFTLCD.RWCD_REG = REG_Addr;
	TFTLCD.RWCD_RAM = Data;
}


void TFTLCD_ReadDeviceId(void) {
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Read_ID;
	TFTLCD_Conf.TFTLCD_Id = TFTLCD.RWCD_RAM;	// 无效数据
	TFTLCD_Conf.TFTLCD_Id = TFTLCD.RWCD_RAM;	// 无效数据 0x00
	TFTLCD_Conf.TFTLCD_Id = TFTLCD.RWCD_RAM;	// 0x93
	TFTLCD_Conf.TFTLCD_Id <<= 8;
	TFTLCD_Conf.TFTLCD_Id |= TFTLCD.RWCD_RAM;	// 0x9341
}


void TFTLCD_SetScanDir(uint16_t Dir) {
	// Dir=0，竖屏；Dir=1，横屏
	TFTLCD_WriteRAM(TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ScanDir, Dir);
}


void TFTLCD_SetCursor(uint16_t XPos, uint16_t YPos) {
	
	// 设置X坐标
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_XPos;
	TFTLCD.RWCD_RAM = XPos>>8;
	TFTLCD.RWCD_RAM = XPos&0xFF;
	// 设置Y坐标
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_YPos;
	TFTLCD.RWCD_RAM = YPos>>8;
	TFTLCD.RWCD_RAM = YPos&0xFF;
	
}


void TFTLCD_DrawPoint(uint16_t XPos, uint16_t YPos, uint32_t Color) {
	
	static uint16_t Color16;
	
	Color16 = COLOR16(Color);
	TFTLCD_SetCursor(XPos, YPos);	// 设置点坐标
	
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
	TFTLCD.RWCD_RAM = Color16;
}


//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void TFTLCD_DrawBlock(uint16_t StartXPos, uint16_t StartYPos, 
	uint16_t EndXPos, uint16_t EndYPos, 
	uint16_t *ColorBuff)
{  
	static uint16_t height, width;
	static uint16_t i, j;
	
	width = EndXPos - StartXPos + 1; 			//得到填充的宽度
	height = EndYPos - StartYPos + 1;			//高度
	
 	for(i=0;i<height;i++)
	{
 		TFTLCD_SetCursor(StartXPos, StartYPos+i);  //设置光标位置 
		TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
		
		for(j=0;j<width;j++)
			TFTLCD.RWCD_RAM = ColorBuff[i*width+j];//写入数据 
	}		  
}


//在指定区域内填充固定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void TFTLCD_DrawBlock1(uint16_t StartXPos, uint16_t StartYPos, 
	uint16_t EndXPos, uint16_t EndYPos, 
	uint16_t ColorBuff)
{  
	static uint16_t height, width;
	static uint16_t i, j;
	
	width = EndXPos - StartXPos + 1; 			//得到填充的宽度
	height = EndYPos - StartYPos + 1;			//高度
	
 	for(i=0;i<height;i++)
	{
 		TFTLCD_SetCursor(StartXPos, StartYPos+i);  //设置光标位置 
		TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
		
		for(j=0;j<width;j++)
			TFTLCD.RWCD_RAM = ColorBuff;//写入数据 
	}		  
}


// 以指定颜色清屏
void TFTLCD_Clear(uint32_t Color) {
	
	uint32_t PointSum;
	uint32_t PointIndex;
	uint16_t Color16;
	
	Color16 = COLOR16(Color);
	PointSum = TFTLCD_Conf.TFTLCD_Height * TFTLCD_Conf.TFTLCD_Width;
	
	TFTLCD_SetCursor(0, 0);
	
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_ColorCode;
	
	for(PointIndex=0;PointIndex<PointSum;PointIndex++) 
	{
		TFTLCD.RWCD_RAM = Color16;
	}
}


// 在屏幕上开辟一个窗口
void TFTLCD_SetWindow(uint16_t StarXPos, uint16_t EndXPos,
	uint16_t StarYPos, uint16_t EndYPos) {
	
	// 规划屏幕的横向宽度
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_XPos;
	TFTLCD.RWCD_RAM = StarXPos>8;
	TFTLCD.RWCD_RAM = StarXPos&0xFF;
	TFTLCD.RWCD_RAM = EndXPos>8;
	TFTLCD.RWCD_RAM = EndXPos&0xFF;
	
	// 规划屏幕的纵向高度
	TFTLCD.RWCD_REG = TFTLCD_Conf.TFTLCD_Cmd.CMD_Write_YPos;
	TFTLCD.RWCD_RAM = StarYPos>8;
	TFTLCD.RWCD_RAM = StarYPos&0xFF;
	TFTLCD.RWCD_RAM = EndYPos>8;
	TFTLCD.RWCD_RAM = EndYPos&0xFF;
}

#else

// TFTLCD初始化函数
__weak void TFTLCD_Init(void) {
	
	u8 GPIOD_AF_Array[9] = {0,1,4,5,8,9,10,14,15};
	u8 GPIOE_AF_Array[9] = {7,8,9,10,11,12,13,14,15};
	u8 GPIOF_AF_Array[1] = {12};
	u8 GPIOG_AF_Array[1] = {12};
	
		/* 定义配置结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef FSMC_InitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_ReadWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef FSMC_WriteTiming;

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
	GPIO_InitStructure.GPIO_Pin =  (0x33)|(7<<8)|(3<<14); // PD0,1,4,5,8~15,NOE,NWE
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					// 复用输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		// 100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					// 上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);								// 初始化数据线
	// GPIOE配置
	GPIO_InitStructure.GPIO_Pin =  (0X1FF<<7);			// PE0,1,7~15,NBL0,NBL1
	GPIO_Init(GPIOE, &GPIO_InitStructure);					// 初始化数据线
	// GPIOF配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			// PB15
	GPIO_Init(GPIOF, &GPIO_InitStructure);					// 初始化地址线
	// GPIOG配置	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;				// PG0~5,10,NE3
	GPIO_Init(GPIOG, &GPIO_InitStructure);					// 初始化地址线
	
	/* GPIO复用配置 */
	Fsmc_AF_Config(GPIOD, GPIOD_AF_Array, 9);
	Fsmc_AF_Config(GPIOE, GPIOE_AF_Array, 9);
	Fsmc_AF_Config(GPIOF, GPIOF_AF_Array, 1);
	Fsmc_AF_Config(GPIOG, GPIOG_AF_Array, 1);
	
	/* FSMC配置 */
	// FSMC读写时序配置
	FSMC_ReadWriteTiming.FSMC_AddressSetupTime = 0xF; //地址建立时间为 16 个 HCLK
	FSMC_ReadWriteTiming.FSMC_AddressHoldTime = 0x00; //地址保持时间模式 A 未用到
	FSMC_ReadWriteTiming.FSMC_DataSetupTime = 0x19; 	//数据保存时间为 24个 HCLK
	FSMC_ReadWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_ReadWriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_ReadWriteTiming.FSMC_DataLatency = 0x00;
	FSMC_ReadWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //模式 A 
	// FSMC写时序
	FSMC_WriteTiming.FSMC_AddressSetupTime = 0xF; //地址建立时间为 16 个 HCLK
	FSMC_WriteTiming.FSMC_AddressHoldTime = 0x00; //地址保持时间模式 A 未用到
	FSMC_WriteTiming.FSMC_DataSetupTime = 0x19; 	//数据保存时间为 24个 HCLK
	FSMC_WriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_WriteTiming.FSMC_CLKDivision = 0x00;
	FSMC_WriteTiming.FSMC_DataLatency = 0x00;
	FSMC_WriteTiming.FSMC_AccessMode = FSMC_AccessMode_A; //模式 A 
	// FSMC配置
	FSMC_InitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;										// 设置FSMC管理的外部SRAM的地址（Bank1的块4）
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_InitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 关闭数据地址复用
	FSMC_InitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;						// 内存类型为SRAM
	FSMC_InitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b; 	// 存储器数据宽度为8bit 
	FSMC_InitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_InitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_InitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable; 
	FSMC_InitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState; 
	FSMC_InitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	// 存储器写使能
	FSMC_InitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable; 
	FSMC_InitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 			// 读写使用不相同的时序
	FSMC_InitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_InitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadWriteTiming;// 读写时序不相同
	FSMC_InitStructure.FSMC_WriteTimingStruct = &FSMC_WriteTiming; 				// 读写时序不相同
	
	/* 使能/失能 */
	FSMC_NORSRAMInit(&FSMC_InitStructure); 				// 初始化FSMC配置
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE); // 使能BANK1区域4
}


// 以指定命令读TFTLCD RAM
uint16_t TFTLCD_ReadRAM(uint16_t REG_Addr) {
	TFTLCD.RWCD_REG = REG_Addr;
	Delay_Us(5);
	return TFTLCD.RWCD_RAM;
}


// 以指定指令写TFTLCD RAM
void TFTLCD_WriteRAM(uint16_t REG_Addr, uint16_t Data) {
	TFTLCD.RWCD_REG = REG_Addr;
	TFTLCD.RWCD_RAM = Data;
}

#endif




