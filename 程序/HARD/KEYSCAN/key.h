#ifndef  __KEY_H
#define  __KEY_H
#include "uhead.h"

typedef struct
{
	u32 Press_Time;	// 记录第一次按下的时间
	u8 val;
	u8 Status;
	u8 key_val;
	u8 val_led;	// 按下置一，抬起清零
}KEY_STATE_T;


#define		KEY_NO 		0
#define		KEY_DOWN	1
#define		KEY_UP		2
#define		KEY_DOWN2	3

#define		TIME_PRESS_LONG 	2000	// 定义长按的时间
#define		TIME_PRESS_SHORT	5		// 短按防抖


u8 Key_Scan(KEY_STATE_T* Key_State); // 回当前按下的键值

#endif // !1
