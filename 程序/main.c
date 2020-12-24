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
sbit 	led2 = P2^4;		// 板子上的LED
u32 	sys_time = 0;		// 系统时钟
bit 	flag_tm1640 = 0;	// 数码管驱动芯片显示标志
bit		flag_second = 0;	// 秒计时
u16 	t = 0;
u8 		ledi = 0;
bit 	flag_led = 1;

KEY_STATE_T key = {0};
u8 key_val = 0;

//sysParam param = {0};		// 系统相关参数都在这里


void main(void)
{
	beep = 0;
	Key_GPIO_Init();
	UartStartIT();						// 115200,TIM1做波特率发生器
	delay_ms(500);
	
	malloc_init();
	debug("start:\r\n");
	Timer0Init();
	SPI_Init();
	pa.pInterface = &faceClose;
	GroupDatInit();				// 读EEPROM编程数据

	while(1)
	{
		pa.keyval = Key_Scan(&key);					// 获得键值
		if(key.val_led != 0 && (key.val_led & 0x7f)<LED_FAN0)
		{
			if(key.val_led & 0x80)// 抬起
			{
				LED_Show((key.val_led & 0x7f),0);
				key.val_led = 0;
			}else LED_Show(key.val_led,1);
	
		}
		
		((interface*)pa.pInterface)->keyfun(&pa);							// 处理按键函数
		((interface*)pa.pInterface)->timeShow(&pa);							// 时间数码管显示函数
		((interface*)pa.pInterface)->tempShow(&pa);							// 温度数码管显示函数
		((interface*)pa.pInterface)->steamShow(&pa);						// 蒸汽数码管显示函数
		SPI_commProcess();								// 读温度
		
		pa.temp = tc[0].measPV;						// 计算温度平均值

		if(flag_tm1640)									// 数码管芯片显示	
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

// 1ms 中断//led2 = !led2;
void Timer0() interrupt 1
{
	static u16 tm1640_c = 0;		// 1640数码管驱动显示间隔	
	static u16 time = 0;
	time++;
	if(time >= 1000)		// 1s间隔
	{
		time = 0;
		flag_second = 1;
	}
	
	tm1640_c ++;
	if(tm1640_c > 130)	// 数码管送显示间隔
	{
		tm1640_c = 0;
		flag_tm1640 = 1;
	}
	sys_time ++;	// 系统时钟计时
		//////SPI相关计时/////
	if(spi.enable&&spi.status==0)
		spi.idleTime++;	//SPI通信空闲计时
	else
		spi.idleTime=0;
	
}



