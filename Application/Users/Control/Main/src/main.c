#include "main.h"


static void vSystem_Init(void)
{
	Nvic_Init(4);                   // �жϷ���
	Delay_Init(configTICK_RATE_HZ); // ϵͳ�δ���ʱ����
	Uart1_Init(115200);             // ���ڳ�ʼ��
	Led_Init();                     // LED��ʼ��
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	
	Timer3_Init(999, 83);			// 1ms
	
	lv_example_table_2();
	
	printf("System initialize complete!");
}


extern TaskHandle_t Start_TaskHandle;


int main(void)
{
	// ϵͳ��ʼ��
	vSystem_Init();
	
	// ��̬����һ����ʼ��������һ����������������
	xTaskCreate(
		(TaskFunction_t )(vStartTask),        // ����ִ�к���
		(const char *   )("vStartTask"),      // ��������
		(uint16_t       )(128),               // �����ջ��С
		(void *         )(NULL),              // ����ִ�к����Ĳ���
		(BaseType_t     )(1),                 // �������ȼ�
		(TaskHandle_t * )(&Start_TaskHandle)  // ������
	);
		
	// ����������
	vTaskStartScheduler();
}