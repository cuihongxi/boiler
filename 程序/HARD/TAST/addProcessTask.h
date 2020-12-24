/*
	2020-8-2
	添加过程任务
	2020年11月22日14:38:23
	修改文件
	
*/
#include "uhead.h"

/* 移植 */
#include "myFlash.h"
#include "keyscan.h"
#include "SMG.H"



#define		flashSave(addr, pbuf,len)	Flash_Write(addr,pbuf,len);	// flash 保存
#define		flashRead(addr, pbuf,len)	Flash_Read(addr,pbuf,len)	// flash 读

#define	 	ShowTemp(temp)			Temp_Show(temp)		// 显示温度
#define	 	ShowTime(time)			Time_Show(time)		// 显示时间
#define		ShowSteam(time)			Steam_Show(time)	// 蒸汽时间

// 键值定义
#define		TIMEADD		KEY_SETSMG1_UP			// 时间数值增加按键
#define		TIMESUB		KEY_SETSMG1_DOWN		// 时间数值减少按键
#define		TEMPADD		KEY_SETSMG2_UP			// 温度数值增加按键
#define		TEMPSUB		KEY_SETSMG2_DOWN		// 温度数值减少按键
#define		SAVEDAT		LONG_PRESS|KEY_P	// 数值保存按键
#define		GROUP_1		KEY_MOD1	// 设定1套按键
#define		GROUP_2		KEY_MOD2	// 设定2套按键
#define		GROUP_3		KEY_MOD3	// 设定3套按键
#define		GROUP_4		KEY_MOD4	// 设定4套按键
#define		GROUP_5		KEY_MOD5	// 设定5套按键
#define		GROUP_6		KEY_MOD6	// 设定6套按键
#define		GROUP_7		KEY_MOD7	// 设定7套按键
#define		GROUP_8		KEY_MOD8	// 设定8套按键
#define		GROUP_9		KEY_MOD9	// 设定9套按键
#define		GROUP_10	KEY_MOD10	// 设定10套按键
#define		GROUP_11	KEY_MOD11	// 设定11套按键
#define		GROUP_12	KEY_MOD12	// 设定12套按键
#define		STEAMADD	KEY_SETSMG3_UP			// 蒸汽数值增加按键
#define		STEAMSUB	KEY_SETSMG3_DOWN		// 蒸汽数值减少按键
#define		FUNADD		KEY_FAN		// 风速数值增加按键
#define		F_ADD		KEY_F		// F按键


// EEPROM 地址定义
#define		INIT_DAT		0X9A	// 初始化值

#define		ADDR_INIT		0	// 保存初始化值的地址
#define 	ADDR_NORMAL		512		// 一般显示保存的地址,扇区起始地址
#define		ADDR_GROUP1		1024	// 扇区起始地址，1套保存地址,一套占用40个字节,12套占用600字节



/* 函数 */

void GroupDatInit( );			// 初始化
void Fun_KeyFun( u8 keyval);	// 按键扫描，根据键值执行对应的函数
void Fun_ShowFun( );			// 显示函数



