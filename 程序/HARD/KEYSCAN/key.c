#include "key.h"
#include "KEYSCAN.H"
#include "printf.h"


u32 Get_sys_time(void)  // 1ms为单位
{
  extern u32 sys_time;
  return sys_time;  		//全局变量，1ms中断
}
/*****************************************************************************
函数名称 : Get_Key_Press_Time
功能描述 : 获取按键按下时间
输入参数 : last_time:上次按下时间
返回参数 : 2次按键之间差值
使用说明 : 无
*****************************************************************************/
static u32 Get_Key_Press_Time(u32 last_time)
{
  return (u32)(Get_sys_time() - last_time);
}


// 回当前按下的键值
u8 Key_Scan(KEY_STATE_T* Key_State)
{
	u32 time = 0;
	u8 key = 0;
	static unsigned char  flag=0;
	
	switch(Key_State->Status)
	{
		case KEY_NO:
			Key_State->val = 0;
			key = KeyRead();  //查询有按键按下
			if(key)//说明有按键按下 此时的数值
			{
				flag=0;
				Key_State->Status = KEY_DOWN;
				Key_State->Press_Time = Get_sys_time();
			}				

			break;
		case KEY_DOWN:
			if(flag==0)  // 只做一次检测
			{  //检索是哪个按键按下
				flag=1;	
				Key_State->key_val = KeyScan();	
				Key_State->Status = KEY_DOWN2;
			}
			break;
		case KEY_DOWN2:
			key = KeyRead();  //查询有按键按下
			if(key==0)//说明当前的按键已被释放
			{
				Key_State->Status = KEY_UP;
			}
			break;
		default:
			break;
	}
	if((Key_State->Status == KEY_DOWN2))
	{
		Key_State->val_led = Key_State->key_val;
		time = Get_Key_Press_Time(Key_State->Press_Time);
		if(time >= TIME_PRESS_LONG && flag) //长按键
		{
			flag = 0;
			Key_State->val = Key_State->key_val | LONG_PRESS;
		}else Key_State->val = 0;
	 
	 }
	 else if(Key_State->Status == KEY_UP)
	 {
		Key_State->Status = KEY_NO;
		time = Get_Key_Press_Time(Key_State->Press_Time);
		Key_State->Press_Time = 0;
		if((time >= TIME_PRESS_SHORT) && (time < TIME_PRESS_LONG)) //短按键
		{			
			Key_State->val = Key_State->key_val;
			
		}
		Key_State->key_val = 0;
		Key_State->val_led |= 0x80;
	 }	
	

	return Key_State->val;
}


	
	
	
	
	