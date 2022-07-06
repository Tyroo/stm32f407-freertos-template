#include "tftlcd.h"


TFTLCD_TouchTypedef TFTLCD_TouchObj = 
{
	{0,0,0,0,0},
	{0,0,0,0,0},
	 0,
	 1,
	 1,
	 0,
	 0,
 TP_CMD_RDX,
 TP_CMD_RDY,
	 0
};


void XPT2046_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 使能GPIOB,C,F时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF, ENABLE);

    // T_PEN、T_MISO初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;	//PB1/PB2 设置为上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化
	
	// T_SCK初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PB0设置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//输出模式
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化
	
	// T_CS初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//PC13设置为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//输出模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//初始化	
	
	// T_MOSI初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PF11设置推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//输出模式
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//初始化	
	
	XPT2046_ReadPoint(&TFTLCD_TouchObj.XAxis[0], &TFTLCD_TouchObj.YAxis[0]);
}


void XPT2046_WriteData(uint8_t Data)
{
	uint8_t count = 0;
	
	for(count=0; count<8; count++)  
	{ 	  
		if(Data&0x80)
			TP_MOSI = 1;  
		else 
			TP_MOSI = 0;   
		Data <<= 1;    
		TP_CLK = 0; 
		Delay_Us(1);
		TP_CLK = 1;		//上升沿有效	        
	}
}


uint16_t XPT2046_ReadData(uint8_t Comd)
{
	uint8_t  count = 0; 	  
	uint16_t Data = 0; 
	
	TP_CLK = 0;		//先拉低时钟 	 
	TP_MOSI = 0; 	//拉低数据线
	TP_CS = 0; 		//选中触摸屏IC
	
	XPT2046_WriteData(Comd);//发送命令字
	
	Delay_Us(6);	//ADS7846的转换时间最长为6us
	TP_CLK = 0; 	     	    
	Delay_Us(1);    	   
	TP_CLK = 1;		//给1个时钟，清除BUSY
	Delay_Us(1);    
	TP_CLK = 0;
	
	for(count=0; count<16; count++)//读出16位数据,只有高12位有效 
	{ 				  
		Data <<= 1; 	 
		TP_CLK = 0;	//下降沿有效  	    	   
		Delay_Us(1);    
 		TP_CLK = 1;
		Data += TP_MISO;
	}  	
	Data >>= 4;   	//只有高12位有效.
	TP_CS = 1;		//释放片选
	
	return Data;  	
}


//读取一个坐标值(x或者y)
//连续读取5次数据,对这些数据升序排列,
//然后去掉最低和最高两个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
uint16_t XPT2046_ReadAxis(uint8_t AxisComd)
{
	uint16_t i, j;
	uint16_t PointBuff[5];
	uint16_t PointSum = 0;
	uint16_t PointValue = 0;
	
	for(i=0; i<5; i++)
	{
		PointBuff[i]= XPT2046_ReadData(AxisComd);
	}
		
	
	for(i=0; i<4; i++)//排序
	{
		for(j=i+1;j<5;j++)
		{
			if(PointBuff[i]>PointBuff[j])//升序排列
			{
				PointValue = PointBuff[i];
				PointBuff[i] = PointBuff[j];
				PointBuff[j] = PointValue;
			}
		}
	}
	
	for(i=1; i<4; i++)
		PointSum += PointBuff[i];
	
	PointValue = PointSum / 3;
	
	return PointValue;   
}


//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
uint8_t XPT2046_ReadPoint(uint16_t *pXAxis, uint16_t *pYAxis) 
{
	static uint8_t ErrorValue = 50;
	static uint16_t XAxis1 = 0, YAxis1 = 0;
 	static uint16_t XAxis2 = 0, YAxis2 = 0;
	
	// 第一次采样
	XAxis1 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadXAxisComd);
	YAxis1 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadYAxisComd);
	
	// 第二次采样
	XAxis2 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadXAxisComd);
	YAxis2 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadYAxisComd);
	
	//前后两次采样在的坐标误差在正负50之内
    if(((XAxis2<=XAxis1 && XAxis1<XAxis2+ErrorValue)||(XAxis1<=XAxis2 && XAxis2<XAxis1+ErrorValue)) &&
       ((YAxis2<=YAxis1 && YAxis1<YAxis2+ErrorValue)||(YAxis1<=YAxis2 && YAxis2<YAxis1+ErrorValue)))
    {
        *pXAxis = (XAxis1 + XAxis2) / 2;
        *pYAxis = (YAxis1 + YAxis2) / 2;
        return 1;
    }
	
	return 0; 
}


uint8_t XPT2046_TouchScan(void)
{			   
	if(TP_EN == 0)//有按键按下
	{
		// 进入临界区保护，关中断
		taskENTER_CRITICAL();
		
		XPT2046_ReadPoint(&TFTLCD_TouchObj.XAxis[0], &TFTLCD_TouchObj.YAxis[0]);
		
		// 退出临界区保护，开中断
		taskEXIT_CRITICAL();
		
		//将结果转换为屏幕坐标
		TFTLCD_TouchObj.XAxis[0] = TFTLCD_TouchObj.XFactor * TFTLCD_TouchObj.XAxis[0] + TFTLCD_TouchObj.XOffset;
		TFTLCD_TouchObj.YAxis[0] = TFTLCD_TouchObj.YFactor * TFTLCD_TouchObj.YAxis[0] + TFTLCD_TouchObj.YOffset; 
		
		//之前没有被按下
		if((TFTLCD_TouchObj.TouchState&TP_PRES_DOWN) == 0)
		{		 
			TFTLCD_TouchObj.TouchState = TP_PRES_DOWN | TP_CATH_PRES;//按键按下  
			TFTLCD_TouchObj.XAxis[4] = TFTLCD_TouchObj.XAxis[0];//记录第一次按下时的坐标
			TFTLCD_TouchObj.YAxis[4] = TFTLCD_TouchObj.YAxis[0];  	   			 
		}			   
	}
	else
	{
		if(TFTLCD_TouchObj.TouchState&TP_PRES_DOWN)//之前是被按下的
		{
			TFTLCD_TouchObj.TouchState&=~(1<<7);//标记按键松开	
		}
		else//之前就没有被按下
		{
			TFTLCD_TouchObj.XAxis[4] = 0;
			TFTLCD_TouchObj.YAxis[4] = 0;
			TFTLCD_TouchObj.XAxis[0] = 0xFFFF;
			TFTLCD_TouchObj.YAxis[0] = 0xFFFF;
		}	    
	}
	return TFTLCD_TouchObj.TouchState&TP_PRES_DOWN;//返回当前的触屏状态
}


// Mode为校准模式，为1则手动重新校准，为0则根据上次校准的数据来校准，若上次没校准则会开启校准
#define SAVE_ADDR_BASE 40
uint16_t AdjustAxisBuff[4][2];//坐标缓存值
uint32_t TimeCount = 0;
float DistRatio = 0;

void XPT2046_TouchAdjust(uint8_t Mode)
{
	uint32_t AdjustSaveData = 0;
	
	uint8_t AdjustStep = 0;
	uint8_t DrawPointFlag = 0;
	uint16_t AdjustPointColor = 0xFB00;
	uint16_t PointDist1, PointDist2;
	uint32_t PointXDistSqut, PointYDistSqut;
	
	
	AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+13, 1);//读取标记字,看是否校准过！
	
	for( ; ; )
	{
		// 判断之前是否校准过了
		if ((AdjustSaveData == 0X0A) && (Mode == 0))
		{
			// 已经校准过了
			// 得到X系数
			AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE, 4);		   
			TFTLCD_TouchObj.XFactor = (float)AdjustSaveData / 100000000;//得到x校准参数
			// 得到Y系数
			AdjustSaveData = AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+4, 4);			          
			TFTLCD_TouchObj.YFactor = (float)AdjustSaveData / 100000000;//得到y校准参数
			// 得到X偏移量
			TFTLCD_TouchObj.XOffset= AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+8, 2);			   	  
			// 得到Y偏移量
			TFTLCD_TouchObj.YOffset = AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+10, 2);				 	  
			TFTLCD_TouchObj.TouchType = AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+12, 1);//读取触屏类型标记
			
			if(TFTLCD_TouchObj.TouchType)//X,Y方向与屏幕相反
			{
				TFTLCD_TouchObj.ReadXAxisComd = TP_CMD_RDY;
				TFTLCD_TouchObj.ReadYAxisComd = TP_CMD_RDX;	 
			}
			else				   //X,Y方向与屏幕相同
			{
				TFTLCD_TouchObj.ReadXAxisComd = TP_CMD_RDX;
				TFTLCD_TouchObj.ReadYAxisComd = TP_CMD_RDY;	 
			}
			break;
		}
		else
		{
			// 画校准点
			// 未校准过，则校准
			//XPT2046_TouchScan(); replace___
			XPT2046_TouchScan();
			if (DrawPointFlag == 0)
			{
				switch(AdjustStep)
				{
					case 0:
						// 画第一个校准点
						TFTLCD_DrawBlock1(20, 20, 60, 60, AdjustPointColor); 
						break;
					case 1:
						// 画第二个校准点
						TFTLCD_DrawBlock1(MY_DISP_HOR_RES - 60, 20, MY_DISP_HOR_RES - 20, 60, AdjustPointColor); 
						break;
					case 2:
						// 画第三个校准点
						TFTLCD_DrawBlock1(20, MY_DISP_VER_RES - 60, 60, MY_DISP_VER_RES - 20, AdjustPointColor); 
						break;
					case 3:
						// 画第四个校准点
						TFTLCD_DrawBlock1(MY_DISP_HOR_RES - 60, MY_DISP_VER_RES - 60, MY_DISP_HOR_RES - 20, MY_DISP_VER_RES - 20, AdjustPointColor);
						break;
					default:
						AdjustStep = 0;
						// 画第一个校准点
						TFTLCD_DrawBlock1(0, 0, 40, 40, AdjustPointColor); 
						break;
				}
				DrawPointFlag = 1;
			}
			
			// 按键按下了一次(此时按键松开了)
			if((TFTLCD_TouchObj.TouchState&0xc0) == TP_CATH_PRES)
			{	
				// 清除倒计时计数器
				TimeCount = 0;
				//标记按键已经被处理过了				
				TFTLCD_TouchObj.TouchState &= ~(1<<6);
										   
				AdjustAxisBuff[AdjustStep][0] = TFTLCD_TouchObj.XAxis[0];
				AdjustAxisBuff[AdjustStep][1] = TFTLCD_TouchObj.YAxis[0];
				
				TFTLCD_Clear(0xFFFF);
				
				AdjustStep++;
				DrawPointFlag = 0;
				
				// 所有点都被点击完了，则判断校准
				if (AdjustStep > 3)
				{
					// 计算1、2两点的距离
					PointXDistSqut = pow(AdjustAxisBuff[0][0] - AdjustAxisBuff[1][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[0][1] - AdjustAxisBuff[1][1], 2);
					PointDist1 = sqrt(PointXDistSqut + PointYDistSqut);
					// 计算3、4两点的距离
					PointXDistSqut = pow(AdjustAxisBuff[2][0] - AdjustAxisBuff[3][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[2][1] - AdjustAxisBuff[3][1], 2);
					PointDist2 = sqrt(PointXDistSqut + PointYDistSqut);
					// 计算两个距离之间的比例
					DistRatio = ((float)PointDist1)/PointDist2;
					
					if((DistRatio<0.95f) || (DistRatio>1.05f))//不合格
					{
						AdjustStep = 0;
						continue;
					}
					// 47.8852   9604
					// 计算1、3两点的距离
					PointXDistSqut = pow(AdjustAxisBuff[0][0] - AdjustAxisBuff[2][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[0][1] - AdjustAxisBuff[2][1], 2);
					PointDist1 = sqrt(PointXDistSqut + PointYDistSqut);
					// 计算2、4两点的距离
					PointXDistSqut = pow(AdjustAxisBuff[1][0] - AdjustAxisBuff[3][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[1][1] - AdjustAxisBuff[3][1], 2);
					PointDist2 = sqrt(PointXDistSqut + PointYDistSqut);
					// 计算两个距离之间的比例
					DistRatio = ((float)PointDist1)/PointDist2;
					
					if((DistRatio<0.95f) || (DistRatio>1.05f))//不合格
					{
						AdjustStep = 0;
						continue;
					}
					
					// 计算1、4两点的距离
					PointXDistSqut = pow(AdjustAxisBuff[0][0] - AdjustAxisBuff[3][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[0][1] - AdjustAxisBuff[3][1], 2);
					PointDist1 = sqrt(PointXDistSqut + PointYDistSqut);
					// 计算2、3两点的距离
					PointXDistSqut = pow(AdjustAxisBuff[1][0] - AdjustAxisBuff[2][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[1][1] - AdjustAxisBuff[2][1], 2);
					PointDist2 = sqrt(PointXDistSqut + PointYDistSqut);
					// 计算两个距离之间的比例
					DistRatio = ((float)PointDist1)/PointDist2;
					
					if((DistRatio<0.95f) || (DistRatio>1.05f))//不合格
					{
						AdjustStep = 0;
						continue;
					}
					
					TFTLCD_TouchObj.XFactor = (float)(MY_DISP_HOR_RES - 80) / (AdjustAxisBuff[1][0]-AdjustAxisBuff[0][0]);//XFactor		 
					TFTLCD_TouchObj.XOffset = (MY_DISP_HOR_RES - TFTLCD_TouchObj.XFactor * (AdjustAxisBuff[1][0]+AdjustAxisBuff[0][0])) / 2;//得到XOffset
						  
					TFTLCD_TouchObj.YFactor = (float)(MY_DISP_VER_RES - 80) / (AdjustAxisBuff[2][1]-AdjustAxisBuff[0][1]);//得到YFactor
					TFTLCD_TouchObj.YOffset = (MY_DISP_VER_RES - TFTLCD_TouchObj.YFactor * (AdjustAxisBuff[2][1]+AdjustAxisBuff[0][1])) / 2;//得到YOffset 
					
					break;
				}
				
			}
		}
		
		Delay_Ms(10);
		TimeCount++;
		
		if (TimeCount > 1000)
			break;
	}
}
