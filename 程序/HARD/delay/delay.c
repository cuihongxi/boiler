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

void Timer0Init(void)		//1����@24.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x40;			//���ö�ʱ��ֵ
	TH0 = 0xA2;			//���ö�ʱ��ֵ
	TF0 = 0;			//���TF0��־
	TR0 = 1;			//��ʱ��0��ʼ��ʱ
    ET0 = 1;           //ʹ�ܶ�ʱ��0�ж�
}
