#include "SMG.H"


u8 smgbuf[16] = {0};

u8 smg[12] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0,0x71};// 0~9显示,"关闭显示"，“F”

#define	SMG_SHOW_OFF		10

#define	SMG_SHUTDOWN		0XFFFF	// 关显示传递的参数

// 时间显示,以秒为单位
void Time_Show(u16 seconds)
{
	if(seconds == 0xffff)
	{
		smgbuf[6] = 0;
		smgbuf[7] = 0;
		smgbuf[8] = 0;
		smgbuf[9] = 0;		
	}else
	{
		// 变换成分
		u8 minutes = seconds/60;
		u8 sec = seconds%60;
		//if(seconds == 0xffff)
		smgbuf[6] = smg[(minutes%100)/10];
		smgbuf[7] = smg[minutes%10]|0x80;
		smgbuf[8] = smg[sec/10];
		smgbuf[9] = smg[sec%10];	
	}
	
	
}

// 温度显示，buf 012位显示温度
void Temp_Show(u16 temp)
{
	if(temp == 0xffff)
	{
		smgbuf[0] = 0;
		smgbuf[1] = 0;
		smgbuf[2] = 0;	
	}else
	{
		smgbuf[0] = smg[temp/100];
		smgbuf[1] = smg[(temp%100)/10];
		smgbuf[2] = smg[temp%10];
	}
}

// 蒸汽时间
void Steam_Show(u16 seconds)
{
	if(seconds == 0xffff)
	{
		smgbuf[3] = 0;
		smgbuf[4] = 0;
		smgbuf[5] = 0;
	}else
	{
		smgbuf[3] = smg[(seconds%1000)/100];
		smgbuf[4] = smg[seconds%100/10];
		smgbuf[5] = smg[seconds%10];
	}		
}

// LED显示
// num是宏定义的第几个
// on_off 为 0,灭，为1亮
// smgbuf从【10】开始是led区域
void LED_Show(u8 num,u8 on_off)
{
	
	u8 bufpos;
	u8 bufbit;
	num --;
	// 计算第几位
	bufpos = 10 + num/8;
	bufbit = num&0x07;
	if(on_off)
	{
		smgbuf[bufpos] |= 0x01<<bufbit;
	}else
	{
		smgbuf[bufpos] &= ~(0x01<<bufbit);
	}
	
}

// 编程指示灯控制
void ProLED_Show(u8 on_off)
{
	if(on_off)
	{
		smgbuf[10] = 0xff;
		smgbuf[11] |= 0x0f;
	}else
	{
		smgbuf[10] = 0;
		smgbuf[11] &= 0xf0;
	}
}
// 按键LED全灭
void Key_led_off()
{
	smgbuf[10] = 0;
	smgbuf[11] = 0;
	smgbuf[12] = 0;
	smgbuf[13] = smgbuf[13]&0XF8;
}

void LedAlloff()
{
	u8 i = 0;
	for(i=0;i<16;i++)
	{
		smgbuf[i]  = 0;
	}
}

// F 和风速 LED 显示
void FAndFanLED_Show(u8 f,u8 fan)
{
	smgbuf[13] = (smgbuf[13] & 0x0f) | (0x10 << fan);
	smgbuf[14] = (smgbuf[14] & 0xe0) | (0x01<<f);

}

// 点亮相应的MOD LED
void LED_MOD_Select(u8 keyval)
{

	if(keyval <= LED_MOD12 && keyval != 0)
	{
		keyval --;
		if(keyval < 8)
		{
			smgbuf[10] = 0x01 << keyval;
			smgbuf[11] = 0;			
		}else
		{
			keyval -= 8;
			smgbuf[11] = 0x01 << keyval;
			smgbuf[10] = 0;				
		}
		

	}
}