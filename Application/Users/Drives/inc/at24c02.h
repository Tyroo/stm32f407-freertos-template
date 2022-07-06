#ifndef __AT24C02_H__
#define __AT24C02_H__


#include "i2c.h"


bool AT24C02_Tx_Data(uint8_t* Data,
		uint16_t Addr, uint8_t TxLen);

bool AT24C02_Rx_Data(uint8_t* Buff, 
		uint16_t Addr, uint8_t RxLen);


#endif

