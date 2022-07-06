#include "main.h"


static void vSystem_Init(void)
{
	Nvic_Init(4);                   // 中断分组
	Delay_Init(configTICK_RATE_HZ); // 系统滴答及延时配置
	Uart1_Init(115200);             // 串口初始化
	Led_Init();                     // LED初始化
	
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
	// 系统初始化
	vSystem_Init();
	
	// 动态创建一个开始任务，用来一次性启动其他任务
	xTaskCreate(
		(TaskFunction_t )(vStartTask),        // 任务执行函数
		(const char *   )("vStartTask"),      // 任务名称
		(uint16_t       )(128),               // 任务堆栈大小
		(void *         )(NULL),              // 任务执行函数的参数
		(BaseType_t     )(1),                 // 任务优先级
		(TaskHandle_t * )(&Start_TaskHandle)  // 任务句柄
	);
		
	// 启动调度器
	vTaskStartScheduler();
}
