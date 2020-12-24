#include "uhead.h"
#include "keyscan.h"
#include "key.h"
#include "TM1640.H"
#include "smg.h"
#include "eeprom.h"
#include "myflash.h"
#include "spi.h"
#include "interface.h"
#include "addprocessTask.h"
#include "List4_Malloc.H"


sbit 	beep = P1^2;	
sbit 	led2 = P2^4;		// �����ϵ�LED
u32 	sys_time = 0;		// ϵͳʱ��
bit 	flag_tm1640 = 0;	// ���������оƬ��ʾ��־
bit		flag_second = 0;	// ���ʱ
u16 	t = 0;
u8 		ledi = 0;
bit 	flag_led = 1;

KEY_STATE_T key = {0};
u8 key_val = 0;

//sysParam param = {0};		// ϵͳ��ز�����������


void main(void)
{
	beep = 0;
	Key_GPIO_Init();
	UartStartIT();						// 115200,TIM1�������ʷ�����
	delay_ms(500);
	
	malloc_init();
	debug("start:\r\n");
	Timer0Init();
	SPI_Init();
	pa.pInterface = &faceClose;
	GroupDatInit();				// ��EEPROM�������

	while(1)
	{
		pa.keyval = Key_Scan(&key);					// ��ü�ֵ
		if(key.val_led != 0 && (key.val_led & 0x7f)<LED_FAN0)
		{
			if(key.val_led & 0x80)// ̧��
			{
				LED_Show((key.val_led & 0x7f),0);
				key.val_led = 0;
			}else LED_Show(key.val_led,1);
	
		}
		
		((interface*)pa.pInterface)->keyfun(&pa);							// ����������
		((interface*)pa.pInterface)->timeShow(&pa);							// ʱ���������ʾ����
		((interface*)pa.pInterface)->tempShow(&pa);							// �¶��������ʾ����
		((interface*)pa.pInterface)->steamShow(&pa);						// �����������ʾ����
		SPI_commProcess();								// ���¶�
		
		pa.temp = tc[0].measPV;						// �����¶�ƽ��ֵ

		if(flag_tm1640)									// �����оƬ��ʾ	
		{
			flag_tm1640 = 0;
			TM1640_AtoShow(0, smgbuf,0x0f);
		}
		
//		if(flag_second)
//		{
//			flag_second = 0;
//			t ++;
//		}
	}
}

// 1ms �ж�//led2 = !led2;
void Timer0() interrupt 1
{
	static u16 tm1640_c = 0;		// 1640�����������ʾ���	
	static u16 time = 0;
	time++;
	if(time >= 1000)		// 1s���
	{
		time = 0;
		flag_second = 1;
	}
	
	tm1640_c ++;
	if(tm1640_c > 130)	// ���������ʾ���
	{
		tm1640_c = 0;
		flag_tm1640 = 1;
	}
	sys_time ++;	// ϵͳʱ�Ӽ�ʱ
		//////SPI��ؼ�ʱ/////
	if(spi.enable&&spi.status==0)
		spi.idleTime++;	//SPIͨ�ſ��м�ʱ
	else
		spi.idleTime=0;
	
}



