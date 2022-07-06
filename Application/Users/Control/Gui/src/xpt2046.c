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
	
	// ʹ��GPIOB,C,Fʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF, ENABLE);

    // T_PEN��T_MISO��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;	//PB1/PB2 ����Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��
	
	// T_SCK��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PB0����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//���ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��
	
	// T_CS��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//PC13����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//���ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��ʼ��	
	
	// T_MOSI��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//PF11�����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//���ģʽ
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//��ʼ��	
	
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
		TP_CLK = 1;		//��������Ч	        
	}
}


uint16_t XPT2046_ReadData(uint8_t Comd)
{
	uint8_t  count = 0; 	  
	uint16_t Data = 0; 
	
	TP_CLK = 0;		//������ʱ�� 	 
	TP_MOSI = 0; 	//����������
	TP_CS = 0; 		//ѡ�д�����IC
	
	XPT2046_WriteData(Comd);//����������
	
	Delay_Us(6);	//ADS7846��ת��ʱ���Ϊ6us
	TP_CLK = 0; 	     	    
	Delay_Us(1);    	   
	TP_CLK = 1;		//��1��ʱ�ӣ����BUSY
	Delay_Us(1);    
	TP_CLK = 0;
	
	for(count=0; count<16; count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Data <<= 1; 	 
		TP_CLK = 0;	//�½�����Ч  	    	   
		Delay_Us(1);    
 		TP_CLK = 1;
		Data += TP_MISO;
	}  	
	Data >>= 4;   	//ֻ�и�12λ��Ч.
	TP_CS = 1;		//�ͷ�Ƭѡ
	
	return Data;  	
}


//��ȡһ������ֵ(x����y)
//������ȡ5������,����Щ������������,
//Ȼ��ȥ����ͺ����������,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
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
		
	
	for(i=0; i<4; i++)//����
	{
		for(j=i+1;j<5;j++)
		{
			if(PointBuff[i]>PointBuff[j])//��������
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


//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
uint8_t XPT2046_ReadPoint(uint16_t *pXAxis, uint16_t *pYAxis) 
{
	static uint8_t ErrorValue = 50;
	static uint16_t XAxis1 = 0, YAxis1 = 0;
 	static uint16_t XAxis2 = 0, YAxis2 = 0;
	
	// ��һ�β���
	XAxis1 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadXAxisComd);
	YAxis1 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadYAxisComd);
	
	// �ڶ��β���
	XAxis2 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadXAxisComd);
	YAxis2 = XPT2046_ReadAxis(TFTLCD_TouchObj.ReadYAxisComd);
	
	//ǰ�����β����ڵ��������������50֮��
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
	if(TP_EN == 0)//�а�������
	{
		// �����ٽ������������ж�
		taskENTER_CRITICAL();
		
		XPT2046_ReadPoint(&TFTLCD_TouchObj.XAxis[0], &TFTLCD_TouchObj.YAxis[0]);
		
		// �˳��ٽ������������ж�
		taskEXIT_CRITICAL();
		
		//�����ת��Ϊ��Ļ����
		TFTLCD_TouchObj.XAxis[0] = TFTLCD_TouchObj.XFactor * TFTLCD_TouchObj.XAxis[0] + TFTLCD_TouchObj.XOffset;
		TFTLCD_TouchObj.YAxis[0] = TFTLCD_TouchObj.YFactor * TFTLCD_TouchObj.YAxis[0] + TFTLCD_TouchObj.YOffset; 
		
		//֮ǰû�б�����
		if((TFTLCD_TouchObj.TouchState&TP_PRES_DOWN) == 0)
		{		 
			TFTLCD_TouchObj.TouchState = TP_PRES_DOWN | TP_CATH_PRES;//��������  
			TFTLCD_TouchObj.XAxis[4] = TFTLCD_TouchObj.XAxis[0];//��¼��һ�ΰ���ʱ������
			TFTLCD_TouchObj.YAxis[4] = TFTLCD_TouchObj.YAxis[0];  	   			 
		}			   
	}
	else
	{
		if(TFTLCD_TouchObj.TouchState&TP_PRES_DOWN)//֮ǰ�Ǳ����µ�
		{
			TFTLCD_TouchObj.TouchState&=~(1<<7);//��ǰ����ɿ�	
		}
		else//֮ǰ��û�б�����
		{
			TFTLCD_TouchObj.XAxis[4] = 0;
			TFTLCD_TouchObj.YAxis[4] = 0;
			TFTLCD_TouchObj.XAxis[0] = 0xFFFF;
			TFTLCD_TouchObj.YAxis[0] = 0xFFFF;
		}	    
	}
	return TFTLCD_TouchObj.TouchState&TP_PRES_DOWN;//���ص�ǰ�Ĵ���״̬
}


// ModeΪУ׼ģʽ��Ϊ1���ֶ�����У׼��Ϊ0������ϴ�У׼��������У׼�����ϴ�ûУ׼��Ὺ��У׼
#define SAVE_ADDR_BASE 40
uint16_t AdjustAxisBuff[4][2];//���껺��ֵ
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
	
	
	AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+13, 1);//��ȡ�����,���Ƿ�У׼����
	
	for( ; ; )
	{
		// �ж�֮ǰ�Ƿ�У׼����
		if ((AdjustSaveData == 0X0A) && (Mode == 0))
		{
			// �Ѿ�У׼����
			// �õ�Xϵ��
			AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE, 4);		   
			TFTLCD_TouchObj.XFactor = (float)AdjustSaveData / 100000000;//�õ�xУ׼����
			// �õ�Yϵ��
			AdjustSaveData = AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+4, 4);			          
			TFTLCD_TouchObj.YFactor = (float)AdjustSaveData / 100000000;//�õ�yУ׼����
			// �õ�Xƫ����
			TFTLCD_TouchObj.XOffset= AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+8, 2);			   	  
			// �õ�Yƫ����
			TFTLCD_TouchObj.YOffset = AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+10, 2);				 	  
			TFTLCD_TouchObj.TouchType = AT24C02_Rx_Data((uint8_t*)&AdjustSaveData, SAVE_ADDR_BASE+12, 1);//��ȡ�������ͱ��
			
			if(TFTLCD_TouchObj.TouchType)//X,Y��������Ļ�෴
			{
				TFTLCD_TouchObj.ReadXAxisComd = TP_CMD_RDY;
				TFTLCD_TouchObj.ReadYAxisComd = TP_CMD_RDX;	 
			}
			else				   //X,Y��������Ļ��ͬ
			{
				TFTLCD_TouchObj.ReadXAxisComd = TP_CMD_RDX;
				TFTLCD_TouchObj.ReadYAxisComd = TP_CMD_RDY;	 
			}
			break;
		}
		else
		{
			// ��У׼��
			// δУ׼������У׼
			//XPT2046_TouchScan(); replace___
			XPT2046_TouchScan();
			if (DrawPointFlag == 0)
			{
				switch(AdjustStep)
				{
					case 0:
						// ����һ��У׼��
						TFTLCD_DrawBlock1(20, 20, 60, 60, AdjustPointColor); 
						break;
					case 1:
						// ���ڶ���У׼��
						TFTLCD_DrawBlock1(MY_DISP_HOR_RES - 60, 20, MY_DISP_HOR_RES - 20, 60, AdjustPointColor); 
						break;
					case 2:
						// ��������У׼��
						TFTLCD_DrawBlock1(20, MY_DISP_VER_RES - 60, 60, MY_DISP_VER_RES - 20, AdjustPointColor); 
						break;
					case 3:
						// �����ĸ�У׼��
						TFTLCD_DrawBlock1(MY_DISP_HOR_RES - 60, MY_DISP_VER_RES - 60, MY_DISP_HOR_RES - 20, MY_DISP_VER_RES - 20, AdjustPointColor);
						break;
					default:
						AdjustStep = 0;
						// ����һ��У׼��
						TFTLCD_DrawBlock1(0, 0, 40, 40, AdjustPointColor); 
						break;
				}
				DrawPointFlag = 1;
			}
			
			// ����������һ��(��ʱ�����ɿ���)
			if((TFTLCD_TouchObj.TouchState&0xc0) == TP_CATH_PRES)
			{	
				// �������ʱ������
				TimeCount = 0;
				//��ǰ����Ѿ�����������				
				TFTLCD_TouchObj.TouchState &= ~(1<<6);
										   
				AdjustAxisBuff[AdjustStep][0] = TFTLCD_TouchObj.XAxis[0];
				AdjustAxisBuff[AdjustStep][1] = TFTLCD_TouchObj.YAxis[0];
				
				TFTLCD_Clear(0xFFFF);
				
				AdjustStep++;
				DrawPointFlag = 0;
				
				// ���е㶼��������ˣ����ж�У׼
				if (AdjustStep > 3)
				{
					// ����1��2����ľ���
					PointXDistSqut = pow(AdjustAxisBuff[0][0] - AdjustAxisBuff[1][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[0][1] - AdjustAxisBuff[1][1], 2);
					PointDist1 = sqrt(PointXDistSqut + PointYDistSqut);
					// ����3��4����ľ���
					PointXDistSqut = pow(AdjustAxisBuff[2][0] - AdjustAxisBuff[3][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[2][1] - AdjustAxisBuff[3][1], 2);
					PointDist2 = sqrt(PointXDistSqut + PointYDistSqut);
					// ������������֮��ı���
					DistRatio = ((float)PointDist1)/PointDist2;
					
					if((DistRatio<0.95f) || (DistRatio>1.05f))//���ϸ�
					{
						AdjustStep = 0;
						continue;
					}
					// 47.8852   9604
					// ����1��3����ľ���
					PointXDistSqut = pow(AdjustAxisBuff[0][0] - AdjustAxisBuff[2][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[0][1] - AdjustAxisBuff[2][1], 2);
					PointDist1 = sqrt(PointXDistSqut + PointYDistSqut);
					// ����2��4����ľ���
					PointXDistSqut = pow(AdjustAxisBuff[1][0] - AdjustAxisBuff[3][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[1][1] - AdjustAxisBuff[3][1], 2);
					PointDist2 = sqrt(PointXDistSqut + PointYDistSqut);
					// ������������֮��ı���
					DistRatio = ((float)PointDist1)/PointDist2;
					
					if((DistRatio<0.95f) || (DistRatio>1.05f))//���ϸ�
					{
						AdjustStep = 0;
						continue;
					}
					
					// ����1��4����ľ���
					PointXDistSqut = pow(AdjustAxisBuff[0][0] - AdjustAxisBuff[3][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[0][1] - AdjustAxisBuff[3][1], 2);
					PointDist1 = sqrt(PointXDistSqut + PointYDistSqut);
					// ����2��3����ľ���
					PointXDistSqut = pow(AdjustAxisBuff[1][0] - AdjustAxisBuff[2][0], 2);
					PointYDistSqut = pow(AdjustAxisBuff[1][1] - AdjustAxisBuff[2][1], 2);
					PointDist2 = sqrt(PointXDistSqut + PointYDistSqut);
					// ������������֮��ı���
					DistRatio = ((float)PointDist1)/PointDist2;
					
					if((DistRatio<0.95f) || (DistRatio>1.05f))//���ϸ�
					{
						AdjustStep = 0;
						continue;
					}
					
					TFTLCD_TouchObj.XFactor = (float)(MY_DISP_HOR_RES - 80) / (AdjustAxisBuff[1][0]-AdjustAxisBuff[0][0]);//XFactor		 
					TFTLCD_TouchObj.XOffset = (MY_DISP_HOR_RES - TFTLCD_TouchObj.XFactor * (AdjustAxisBuff[1][0]+AdjustAxisBuff[0][0])) / 2;//�õ�XOffset
						  
					TFTLCD_TouchObj.YFactor = (float)(MY_DISP_VER_RES - 80) / (AdjustAxisBuff[2][1]-AdjustAxisBuff[0][1]);//�õ�YFactor
					TFTLCD_TouchObj.YOffset = (MY_DISP_VER_RES - TFTLCD_TouchObj.YFactor * (AdjustAxisBuff[2][1]+AdjustAxisBuff[0][1])) / 2;//�õ�YOffset 
					
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