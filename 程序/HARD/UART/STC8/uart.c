#include<uart.h>

bit busy = 0;               //清忙标志

void UartInit(void)		//115200bps@24.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xCC;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}
void UartStartIT(void)	// 用STC助手生成的代码UartInit，启动
{
	UartInit();
	ES = 1;                     //使能串口1中断
	EA = 1; 					//允许总中断
}

void UARTSendByte(unsigned char byte)
{	
   // TB8 = 0;                //设置校验位为0
    busy = 1;
	SBUF = byte;
	while(busy);
}
void UARTSendStr(unsigned char* str)
{	
    while (*str)     //遇到停止符0结束
    {
       UARTSendByte(*str ++);
    }
 }

//========================================================================
// 函数: void UART1_int (void) interrupt UART1_VECTOR
// 描述: UART1中断函数。
// 参数: nine.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void UART1_int (void) interrupt 4
{
    if(RI)
    {
        RI = 0;
    }

    if(TI)
    {
        TI = 0;
		busy = 0;               //清忙标志
    }
}
