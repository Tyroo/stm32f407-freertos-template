#include "start.h"


TaskHandle_t Led0_TaskHandle;
TaskHandle_t Led1_TaskHandle;
TaskHandle_t Float_TaskHandle;
TaskHandle_t Start_TaskHandle;


static void vLed0Task(void* pArgs)
{
	for( ; ; )
	{
		Led_Control(1, LED_0); // ����LED
		vTaskDelay(500);
		Led_Control(0, LED_0); // �ر�LED
		vTaskDelay(500);
	}
}


static void vLed1Task(void* pArgs)
{
	for( ; ; )
	{
		Led_Control(1, LED_1); // ����LED
		vTaskDelay(1000);
		Led_Control(0, LED_1); // �ر�LED
		vTaskDelay(1000);
	}
}

static void vFloatTask(void* pArgs)
{
	static float floatNumber = 0.00F;
	
	for( ; ; )
	{
		floatNumber += 0.01F;
		printf("floatNumber: %.4f", floatNumber);
		vTaskDelay(1000);
	}
}


void vStartTask(void* pArgs)
{
	// �����ٽ������������ж�
	taskENTER_CRITICAL();
	
	// ��̬����LED0��˸����
	xTaskCreate(
		(TaskFunction_t )(vLed0Task),
		(const char *   )("vLed0Task"),
		(uint16_t       )(50),
		(void *         )(NULL),
		(BaseType_t     )(2),
		(TaskHandle_t * )(&Led0_TaskHandle)
	);
	// ��̬����LED1��˸����
	xTaskCreate(
		(TaskFunction_t )(vLed1Task),
		(const char *   )("vLed1Task"),
		(uint16_t       )(50),
		(void *         )(NULL),
		(BaseType_t     )(3),
		(TaskHandle_t * )(&Led1_TaskHandle)
	);
	// ��̬���������������
	xTaskCreate(
		(TaskFunction_t )(vFloatTask),
		(const char *   )("vFloatTask"),
		(uint16_t       )(128),
		(void *         )(NULL),
		(BaseType_t     )(4),
		(TaskHandle_t * )(&Float_TaskHandle)
	);
	
	// ɾ����������
	vTaskDelete(Start_TaskHandle);
		
	// �˳��ٽ������������ж�
	taskEXIT_CRITICAL();
	
}