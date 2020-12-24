#include <delay.h>
#include <STC8.H>

void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

	
void delay_ms(unsigned int t)
{
	while(t --)Delay1ms();
}

void Timer0Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;			//设置定时初值
	TH0 = 0xA2;			//设置定时初值
	TF0 = 0;			//清除TF0标志
	TR0 = 1;			//定时器0开始计时
    ET0 = 1;           //使能定时器0中断
}
