#include "main.h"


static void System_Init(void)
{
	Nvic_Init(2);          // 中断分组
	Uart1_Init(115200);    // USART初始化
	Led_Init();            // LED初始化
	Delay_Init(configTICK_RATE_HZ);
	
	printf("System initialize complete!");
}


int main(void)
{
	System_Init();
	
	Led_Control(1);
	
	for( ; ; );
	
}
