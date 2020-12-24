/*
2020年11月15日14:02:12
	界面显示及按键扫描接口
	该函数决定按键执行的函数，和数码管LED怎么显示
*/
#ifndef	__INTERFACE_H
#define	__INTERFACE_H
#include "uhead.h"
#include "key.h"
#include "smg.h"
#include "KEYSCAN.H"
#include "addprocessTask.h"



extern sysParam pa;
// 界面显示结构体
typedef struct{
	void(*keyfun)(sysParam*  param); 		// 键值处理函数
	void(*timeShow)(sysParam*  param);		// 时间数码管显示内容
	void(*tempShow)(sysParam*  param);		// 温度数码管显示内容
	void(*steamShow)(sysParam*  param);		// 蒸汽数码管显示内容
//	void(*fun)(sysParam*  param);			// 功能函数
}interface;





extern interface faceClose ;

void Null_Fun(sysParam*  param);// 啥也不做函数
void TwinkleProLed_Fun();// 闪烁编程指示灯
void Pro_keyfun(sysParam*  param);// 编程界面
void Main_keyfun(sysParam*  param);// 开机后主界面
void Show_WorkTime(sysParam*  param);// 显示工作时间
void Show_Temperature(sysParam*  param);// 显示温度
void Show_SteamTime(sysParam*  param);// 显示蒸汽时间
void Close_keyfun(sysParam*  param);
void Close_timeShow(sysParam*  param);
void ClosetempShow(sysParam*  param);
void ClosesteamShow(sysParam*  param);
void Pro_timeShow(sysParam*  param);
void Pro_tempShow(sysParam*  param);
void Pro_teamShow(sysParam*  param);


#endif

