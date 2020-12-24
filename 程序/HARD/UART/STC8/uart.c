#include<uart.h>

bit busy = 0;               //��æ��־

void UartInit(void)		//115200bps@24.000MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xCC;		//�趨��ʱ��ֵ
	TH1 = 0xFF;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
}
void UartStartIT(void)	// ��STC�������ɵĴ���UartInit������
{
	UartInit();
	ES = 1;                     //ʹ�ܴ���1�ж�
	EA = 1; 					//�������ж�
}

void UARTSendByte(unsigned char byte)
{	
   // TB8 = 0;                //����У��λΪ0
    busy = 1;
	SBUF = byte;
	while(busy);
}
void UARTSendStr(unsigned char* str)
{	
    while (*str)     //����ֹͣ��0����
    {
       UARTSendByte(*str ++);
    }
 }

//========================================================================
// ����: void UART1_int (void) interrupt UART1_VECTOR
// ����: UART1�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
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
		busy = 0;               //��æ��־
    }
}
