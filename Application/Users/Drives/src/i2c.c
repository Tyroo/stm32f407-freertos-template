/* 软件IIC模块（验证通过） */
#include "i2c.h"


void IIC_Init(void) 
{
	/* 配置结构体定义 */
	GPIO_InitTypeDef GPIO_InitStructure;						// 定义一个GPIO配置结构体
	
	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		// 使能GPIOB时钟
	
	/* 配置GPIO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				// GPIO输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽输出
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;				// 上拉模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;		// 配置Pin8~9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// 输出速度为100MHz
	
	/* 初始化GPIO */
	GPIO_Init(GPIOB, &GPIO_InitStructure);						// 应用GPIOB配置
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);				// 初始拉高SCK（Pin8）和SDA（Pin9）
}


// 发送起始信号，SCK高电平下，SDA由高变低
void IIC_Start(void) 
{
	IIC_Mode_Tx();     //sda线输出
	IIC_SDA_OUT = 1;	  	  
	IIC_SCL = 1;
	Delay_Us(4);
 	IIC_SDA_OUT = 0;//START:when CLK is high,DATA change form high to low 
	Delay_Us(4);
	IIC_SCL = 0;//钳住I2C总线，准备发送或接收数据 
}

    
// 发送结束信号，SCK高电平下，SDA由低变高
void IIC_Stop(void) 
{
	IIC_Mode_Tx();//sda线输出
	IIC_SCL = 0;
	IIC_SDA_OUT = 0;//STOP:when CLK is high DATA change form low to high
 	Delay_Us(4);
	IIC_SCL = 1; 
	IIC_SDA_OUT = 1;//发送I2C总线结束信号
	Delay_Us(4);	
}


// 发送响应信号（应答、非应答），接收完数据后将SDA拉高
static void IIC_TxReply(uint8_t Res)
{ 
	IIC_SCL = 0;
	IIC_Mode_Tx();
	
	IIC_SDA_OUT = (1 - Res);	// 将SDA拉低则应答，否则不应答
	Delay_Us(2);
	
	IIC_SCL = 1;
	Delay_Us(2);
	
	IIC_SCL = 0;
}


// 接收响应信号（应答、非应答），接收完数据后将SDA拉高
static bool IIC_RxReply(void)
{ 
	uint8_t ReplyTime;
	
	ReplyTime = 0;

	IIC_Mode_Rx();
	IIC_SDA_OUT = 1;
	Delay_Us(1);	   
	IIC_SCL=1;
	Delay_Us(1);
	while(IIC_SDA_IN)
	{
		ReplyTime++;
		if(ReplyTime>250)
		{
			IIC_Stop();
			return false;
		}
	}
	
	IIC_SCL = 0;

	return true;	

}


// 发送8位数据
bool IIC_Send_Byte(uint8_t Data) 
{
	uint8_t Bit;
	
	IIC_Mode_Tx();	// 将IIC设置为发送模式
	
	for(Bit=0;Bit<8;Bit++) 
	{
		IIC_SDA_OUT = (Data&0x80)>>7;
		Data<<=1;
		Delay_Us(1);
		
		IIC_SCL = 1;
		Delay_Us(2);
		
		IIC_SCL = 0;		// 拉低SCL电平，准备发送第一帧数据
		Delay_Us(1);

	}
	return IIC_RxReply();
}


// 接收8位数据
uint8_t IIC_Read_Byte(uint8_t IsAck) 
{
	
	uint8_t RxData, RxIndex;
	
	IIC_Mode_Rx();
	
	for (RxIndex=0;RxIndex<8;RxIndex++) 
	{
		IIC_SCL = 0;
		Delay_Us(2);
		
		IIC_SCL = 1;
		RxData <<= 1;
		if (IIC_SDA_IN) RxData++;
		Delay_Us(1);
	}
	
	IIC_TxReply(IsAck);
	
	return RxData;
}
