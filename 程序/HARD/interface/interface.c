#include "interface.h"
#include "addProcessTask.h"
#define	SMG_SHOW_F			11
extern u8 smg[12];
extern u32 sys_time;	
sysParam pa = {0};		// 系统相关参数都在这里

interface waiteInerface = {Main_keyfun,Show_WorkTime,Show_Temperature,Show_SteamTime}; // 待机状态界面
interface faceClose = {Close_keyfun,Close_timeShow,ClosetempShow,ClosesteamShow};		// 关机界面
interface facePro = {Pro_keyfun,Pro_timeShow,Pro_tempShow,Pro_teamShow};				// 编程界面


//  闪烁函数
u16 Twinkle_SMG(u16 dat)
{
	static u32 time = 0;
	static u8 on_off = 0;
	if((sys_time - time) > 500)
	{
		time = sys_time;
		on_off = !on_off;
	}
	if(on_off) return dat;
	else return 0xffff;
}
void Pro_timeShow(sysParam*  param)
{
	// Time_Show(Twinkle_SMG(param->prodat.set[param->prodat.setnum].worktime));		// 显示当前的时间
	Time_Show(param->prodat.set[param->prodat.setnum].worktime);
}

void Pro_tempShow(sysParam*  param)
{
	Temp_Show(param->prodat.set[param->prodat.setnum].settemp);		// 显示当前温度
}

void Pro_teamShow(sysParam*  param)
{
	Steam_Show(param->prodat.set[param->prodat.setnum].steamtime);	// 显示蒸汽时间
}


// 闪烁编程指示灯
void TwinkleProLed_Fun()
{
	static u32 time = 0;
	static u8 on_off = 0;
	if((sys_time - time) > 500)
	{
		time = sys_time;
		on_off = !on_off;
		ProLED_Show(on_off);
	}
}

// 编程界面
void Pro_keyfun(sysParam*  param)
{
	static u8 flag_init = 0;
	if(param->keyval)	// 键值处理
	{
		printf("param.keyval = %d\r\n",param->keyval);
		if(flag_init == 0 && param->keyval <= KEY_MOD12)
		{
			flag_init = 1;
			ProLED_Show(0);	// 关闭编程指示灯
		}
			
		LED_MOD_Select(param->keyval);// 点亮MOD相应的LED
		switch(param->keyval)
		{
			case KEY_POWER:	
				flag_init = 0;
				Key_led_off();
				param->pInterface = &faceClose;
				//debug("faceClose\r\n");
			break;
		}

		
		if(flag_init)
		{
			Fun_KeyFun(param->keyval);	// 按键扫描，根据键值执行对应的函数
			FAndFanLED_Show(pa.prodat.setnum,pa.prodat.set[pa.prodat.setnum].funspeed);	// F 和风速数码管显示
			if((param->keyval & 0x7f) == KEY_P) // 长按短按P都返回
			{
				flag_init = 0;
				ProLED_Show(0);	// 关闭编程指示灯
				param->pInterface = &waiteInerface;// 返回待机界面
			}
		}
			
				
		param->keyval = 0;

	}else
	{
		if(flag_init == 0)
		{
			TwinkleProLed_Fun();	// 编程LED闪烁
		}
	}
}


/*******************************开机界面************************************/
// 开机后主界面
void Main_keyfun(sysParam*  param)
{
	if(param->keyval)	// 键值处理
	{
		if(param->keyval&LONG_PRESS)	// 长按
		{
			printf("waiteInerface L: %d\r\n",param->keyval&(~LONG_PRESS));			
		}
		else 					// 短按
		{
			printf("waiteInerface S: %d\r\n",param->keyval);
			switch(param->keyval)
			{
				case KEY_P:			
					if(param->timsta == off)	// 定时器未启动状态下，按P键，1~12闪烁
					{
						param->pInterface = &facePro;	// 进入编程界面
					}
					break;
				case KEY_POWER:	
					param->pInterface = &faceClose;
					debug("faceClose\r\n");
				break;
			}

		}
		
		param->keyval = 0;

	}
}

// 显示工作时间
void Show_WorkTime(sysParam*  param)
{
	Time_Show(param->workSeconds);
}

// 显示温度
void Show_Temperature(sysParam*  param)
{
	Temp_Show(param->temp);
}

// 显示蒸汽时间
void Show_SteamTime(sysParam*  param)
{
	Steam_Show(param->steamSeconds);
}


/******************************关机界面*************************************/
void Close_keyfun(sysParam*  param)
{
	//u8 dat = 0x88;
	//u8 a = 0;
	if(param->keyval)	// 键值处理
	{
		if(param->keyval&LONG_PRESS)	// 长按
		{
			printf("faceClose L: %d\r\n",param->keyval&(~LONG_PRESS));
			
		}
		else 					// 短按
		{
			printf("faceClose S: %d\r\n",param->keyval);
			switch(param->keyval)
			{
				case KEY_POWER:
					param->pInterface = &waiteInerface;// 开机流程
				break;
//				case KEY_MOD1:
//					debug("save dat\r\n");
//					flashSave(0, &dat,1);
//				break;
//				case KEY_MOD2:
//					flashRead(0, &a,1);
//					printf("a = %d\r\n",a);
//				break;
								
			}
		}
		
		param->keyval = 0;

	}
}

void Close_timeShow(sysParam*  param)	// 时间数码管显示内容
{
	param->keyval = param->keyval;
	smgbuf[6] = 0;
	smgbuf[7] = 0;
	smgbuf[8] = 0;
	smgbuf[9] = 0;	
}

void ClosetempShow(sysParam*  param)			// 温度数码管显示内容
{
	param->keyval = param->keyval;
	LedAlloff();
	smgbuf[0] = smg[0];
	smgbuf[1] = smg[SMG_SHOW_F];
	smgbuf[2] = smg[SMG_SHOW_F];
}

void ClosesteamShow(sysParam*  param)		// 蒸汽数码管显示内容
{
	param->keyval = param->keyval;
	smgbuf[3] = 0;
	smgbuf[4] = 0;
	smgbuf[5] = 0;
}























