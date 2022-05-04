#include "start.h"


TaskHandle_t Led0_TaskHandle;
TaskHandle_t Led1_TaskHandle;
TaskHandle_t Float_TaskHandle;
TaskHandle_t Start_TaskHandle;


static void vLed0Task(void* pArgs)
{
	for( ; ; )
	{
		Led_Control(1, LED_0); // 点亮LED
		vTaskDelay(500);
		Led_Control(0, LED_0); // 关闭LED
		vTaskDelay(500);
	}
}


static void vLed1Task(void* pArgs)
{
	for( ; ; )
	{
		Led_Control(1, LED_1); // 点亮LED
		vTaskDelay(1000);
		Led_Control(0, LED_1); // 关闭LED
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
	// 进入临界区保护，关中断
	taskENTER_CRITICAL();
	
	// 动态创建LED0闪烁任务
	xTaskCreate(
		(TaskFunction_t )(vLed0Task),
		(const char *   )("vLed0Task"),
		(uint16_t       )(50),
		(void *         )(NULL),
		(BaseType_t     )(2),
		(TaskHandle_t * )(&Led0_TaskHandle)
	);
	// 动态创建LED1闪烁任务
	xTaskCreate(
		(TaskFunction_t )(vLed1Task),
		(const char *   )("vLed1Task"),
		(uint16_t       )(50),
		(void *         )(NULL),
		(BaseType_t     )(3),
		(TaskHandle_t * )(&Led1_TaskHandle)
	);
	// 动态创建浮点计算任务
	xTaskCreate(
		(TaskFunction_t )(vFloatTask),
		(const char *   )("vFloatTask"),
		(uint16_t       )(128),
		(void *         )(NULL),
		(BaseType_t     )(4),
		(TaskHandle_t * )(&Float_TaskHandle)
	);
	
	// 删除启动任务
	vTaskDelete(Start_TaskHandle);
		
	// 退出临界区保护，开中断
	taskEXIT_CRITICAL();
	
}
