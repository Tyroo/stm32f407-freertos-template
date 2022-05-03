#include "main.h"


static void System_Init(void)
{
	Nvic_Init(2);          // �жϷ���
	Uart1_Init(115200);    // USART��ʼ��
	Led_Init();            // LED��ʼ��
	Delay_Init(configTICK_RATE_HZ);
	
	printf("System initialize complete!");
}


int main(void)
{
	System_Init();
	
	Led_Control(1);
	
	for( ; ; );
	
}