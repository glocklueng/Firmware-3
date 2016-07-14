#include "debug.h"

#if DEBUG_EN > 0

void InitDebug()
{
	GPIO_Init(DEBUG_TX_PORT, DEBUG_TX_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void SendDebugByte(u8 data)
{
	//start
	DEBUG_TX = 0;
	sys.delay_us(BIT_DELAY_TIME);
	
	for (u8 i=0; i<8; i++)
	{
		DEBUG_TX = data & 0x01;
		sys.delay_us(BIT_DELAY_TIME);
		data >>= 1;
	}
	
	//stop
	DEBUG_TX = 1;
	sys.delay_us(BIT_DELAY_TIME);
}

void SendDebugBuff(u8 *buff, u16 len)
{
	for (u8 i=0; i<len; i++)
		SendDebugByte(buff[i]);
}

#endif