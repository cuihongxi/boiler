/**
 *  数码管显示
 *  2020年11月14日12:47:31
 * 
 */
#ifndef __SMG__H
#define __SMG__H

#include "uhead.h"

#define		LED_MOD1	1
#define		LED_MOD2	2
#define		LED_MOD3	3
#define		LED_MOD4	4
#define		LED_MOD5	5
#define		LED_MOD6	6
#define		LED_MOD7	7
#define		LED_MOD8	8
#define		LED_MOD9	9
#define		LED_MOD10	10
#define		LED_MOD11	11
#define		LED_MOD12	12

#define		LED_PROOF	14
#define		LED_AUX		15
#define		LED_MOTO	16
#define		LED_STEAM	17
#define		LED_P		18
#define		LED_START	19
#define		LED_STEAMBOX	20
#define		LED_TEMP_SET	21
#define		LED_TEMP_ACT	22
#define		LED_STEAM_SET	23
#define		LED_STEAM_ACT	24

#define		LED_ONOFF		26
#define		LED_LED			27
#define		LED_NOWATER		28
#define		LED_FAN0		29
#define		LED_FAN1		30
#define		LED_FAN2		31
#define		LED_FAN3		32
#define		LED_F0			33
#define		LED_F1			34
#define		LED_F2			35
#define		LED_F3			36
#define		LED_F4			37



extern u8 smgbuf[16];


void Temp_Show(u16 temp);			// 温度显示
void Steam_Show(u16 seconds);		// 蒸汽时间
void Time_Show(u16 seconds); 		// 时间显示,以秒为单位
void LED_Show(u8 num,u8 on_off);	// LED指示灯
void Key_led_off();					// 按键LED全灭
void ProLED_Show(u8 on_off);		// 编程指示灯控制
void FAndFanLED_Show(u8 f,u8 fan);  // F 和风速 LED 显示
void LED_MOD_Select(u8 keyval);// 点亮相应的MOD LED
void LedAlloff();
#endif