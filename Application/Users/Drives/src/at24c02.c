#include "at24c02.h"


// 发送一个字符串给从设备
bool AT24C02_Tx_Data(uint8_t* Data,
		uint16_t Addr, uint8_t TxLen) 
{
	bool 	Res;						// 应答响应
	uint8_t Index;						// 发送字节数索引
	
	// 当接收的数据超出最大限制或者设备地址超过127时，发送失败
	if (sizeof(Data)>IIC_DATA_SIZE_MAX) 
		return false;
	
	Res = true, Index = 0;
	
	// 当遇到结束字符或者应答失效后停止发送

	while((Index < TxLen) && (Res == true)) 
	{	
		IIC_Start();						// 发送IIC开始信号
		Res = IIC_Send_Byte(0xA0+(((Addr+Index)/256)<<1));// 发送寄存器高地址
		Res = IIC_Send_Byte((Addr+Index)%256);		// 发送寄存器低地址
		Res = IIC_Send_Byte(*(Data+Index));	// 发送一个字节数据
		IIC_Stop();							// 发送IIC结束信号
		Delay_Ms(10);
		Index++;
	}
	return (bool)Res;
}


// 接收一个字符串
bool AT24C02_Rx_Data(uint8_t* Buff, 
		uint16_t Addr, uint8_t RxLen) 
{
	bool 	Res;	// 应答响应
	uint8_t Index;	// 接收数据索引
	
	// 当接收的数据超出最大限制或者设备地址超过127时，退出接收
	if (RxLen>IIC_DATA_SIZE_MAX) 
		return false;
	
	Res = true, Index = 0;
	
	while((Index < RxLen) && (Res == true)) 
	{
		IIC_Start();						// 发送IIC起始信号
		Res = IIC_Send_Byte(0xA0+(((Addr+Index)/256)<<1));// 发送寄存器地址高位
		Res = IIC_Send_Byte((Addr+Index)%256);// 发送寄存器地址低位
		
		/* 接收数据存入数组, 接收最后一个字节后无需应答 */
		IIC_Start();						// 发送IIC起始信号
		Res = IIC_Send_Byte(0xA1); 			// 发送读命令
		*(Buff+Index) = IIC_Read_Byte(0);	// 读取数据，不发送应答信号
		IIC_Stop();							// 停止发送
		Index++;
	}
	
	return (bool)Res;
}
