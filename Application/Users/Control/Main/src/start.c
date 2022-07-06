#include "start.h"


TaskHandle_t Queue_TaskHandle;
TaskHandle_t Queue_Task1Handle;
TaskHandle_t Start_TaskHandle;

QueueHandle_t Queue_Handle; // ����һ�����о��


static void vQueueTask(void* pArgs)
{
	uint16_t count = 0;
	BaseType_t Result;
	
	for( ; ; )
	{
//		count++;
//		
//		Result = xQueueSend(Queue_Handle, &count, 10);
//		
//		if (Result == pdFALSE)
//		{
//			
//			printf("Queue Send failed!\r\n");
//		}
//		
//		Led_Control(0, LED_1);
//		vTaskDelay(1000);
		XPT2046_TouchScan();
		vTaskDelay(5);
	}
}


static void vQueueTask1(void* pArgs)
{
	BaseType_t Result;
	uint16_t numData;
	
	for( ; ; )
	{
//		Result = xQueueReceive(Queue_Handle, &numData, 10);
//		
//		if (Result == pdTRUE)
//		{
//			
//			printf("Receive Queue Data Number: %d\r\n", numData);
//		}
//		
//		Led_Control(1, LED_1);
//		vTaskDelay(1000);
		lv_timer_handler();
		
		vTaskDelay(10);
	}
}


void vStartTask(void* pArgs)
{
	// �����ٽ������������ж�
	taskENTER_CRITICAL();
	
	Queue_Handle = xQueueCreate(1, sizeof(uint16_t));	// ����һ������
	
	// ��̬����LED0��˸����
	xTaskCreate(
		(TaskFunction_t )(vQueueTask),
		(const char *   )("vQueueTask"),
		(uint16_t       )(50),
		(void *         )(NULL),
		(BaseType_t     )(1),
		(TaskHandle_t * )(&Queue_TaskHandle)
	);
	// ��̬����LED1��˸����
	xTaskCreate(
		(TaskFunction_t )(vQueueTask1),
		(const char *   )("vQueueTask1"),
		(uint16_t       )(1000),
		(void *         )(NULL),
		(BaseType_t     )(3),
		(TaskHandle_t * )(&Queue_Task1Handle)
	);
	
	// ɾ����������
	vTaskDelete(Start_TaskHandle);

	// �˳��ٽ������������ж�
	taskEXIT_CRITICAL();
	
}