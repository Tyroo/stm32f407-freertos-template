/** USART收发数据模块（验证通过） **/

#include "usart.h"

// USART1接收的数据
char UsartReceiveData[50];

#if USE_PRINTF_FUNC        
#pragma import(__use_no_semihosting)             

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
};


FILE __stdout;       

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 

}

void _ttywrch(int ch)
{
	ch = ch;
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);	//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif


// 初始化UART
void Uart1_Init(uint32_t Baud) {
	
	/* 配置结构体指针定义 */
	USART_InitTypeDef USART_InitStructre;					// 定义一个USART配置结构体
	GPIO_InitTypeDef	GPIO_InitStructre;					// 定义一个GPIO 配置结构体
	
	/* 时钟配置 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// 使能GPIOA端口时钟，使能UART1端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	// 使能USART1时钟
	
	/* GPIOA9 IO口复用USART1配置 */
	USART_DeInit(USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);		//PA9 复用为 USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	//PA10复用为USART1
	GPIO_InitStructre.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		// 配置引脚PA9、PA10
	GPIO_InitStructre.GPIO_Mode = GPIO_Mode_AF;					// 配置GPIO位复用推挽输出
	GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;				// 推挽输出
	GPIO_InitStructre.GPIO_PuPd = GPIO_PuPd_UP;					// IO上拉
	GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;			// 配置PA9引脚的输出速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructre);						// 将配置应用给PA9
	
	/* USART1配置 */
	USART_InitStructre.USART_BaudRate = Baud;					// 设置波特率
	USART_InitStructre.USART_WordLength = USART_WordLength_8b;		// 设置传输的数据位位数
	USART_InitStructre.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 使能USART具备收发数据的功能	
	USART_InitStructre.USART_StopBits = USART_StopBits_1;			// 设置在传输帧结尾设置一个位的停止位
	USART_InitStructre.USART_Parity = USART_Parity_No;				// 失能奇偶校验位
	USART_InitStructre.USART_HardwareFlowControl =					// 无硬件数据流控制，不配置此位会出现接收不到数据
	USART_HardwareFlowControl_None; 	
	
	USART_Init(USART1, &USART_InitStructre);						// 初始化USART1配置
	
	/* 使能 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					// 使能USART1的接收中断
	USART_Cmd(USART1, ENABLE);										// 使能串口
	
	/* NVIC配置 */
	Nvic_Config(USART1_IRQn, 3, 3, 1);								// 抢占优先级为0，响应优先级为0，并使能
	
}


// UART1发送数据
void Uart1_Send(char *Data) {
	
	// 当发送字符遇到\0时停止发送
	while (*Data != '\0') {
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, *(Data++));							// 调用USART_SendData发送单个字符
	}
}


// USART1接收数据<中断函数>
void USART1_IRQHandler() {
	
	uint8_t RxData;										// 定义一个接收单个字符的变量
	static int DataBit = 0;								// 定义一个接收位
	
	// 检查接收寄存器是否非空，非空为1，空为0
	FlagStatus ReceiveFlag = USART_GetITStatus(USART1, USART_IT_RXNE);
	
	if (ReceiveFlag != RESET) {
		RxData = USART_ReceiveData(USART1);				// 获取接收到的数据
//		USART_SendData(USART1, RxData);

		UsartReceiveData[DataBit++] = RxData;			// 将数据存入接收字符串中
		
		// 当接收的数据遇到结尾字符时终止接收
		if (RxData == '\n') {
			UsartReceiveData[DataBit] = '\0';
			DataBit = 0;								// 清除接收位
		}
	}
}
