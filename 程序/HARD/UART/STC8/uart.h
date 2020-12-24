#ifndef __UART_H
#define __UART_H

#include <STC8.H>
#include "uhead.h"


#define     Baudrate1       115200L

void UARTSendStr(unsigned char* str);
void UARTSendByte(unsigned char byte);
void UartStartIT(void);	// ��STC�������ɵĴ���UartInit������



#endif