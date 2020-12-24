
#ifndef  __KEYSCAN_H   
#define  __KEYSCAN_H 

#include "uhead.h"
#include "SMG.h"



/* 移植 */

sbit SH1 = P4^3; 
sbit SH2 = P0^3; 
sbit SH3 = P0^4; 
sbit SH4 = P0^5; 
sbit SH5 = P0^6; 
sbit SH6 = P0^7; 

sbit SL1 = P2^6;
sbit SL2 = P2^7;
sbit SL3 = P0^0;
sbit SL4 = P0^1;
sbit SL5 = P0^2;

#define     FILTERMM          5     // 滤波延时
#define     LONGPRESS_VAL     2000  //长按阀值
#define	LONG_PRESS	0X80	// 长按标志

// 按键的结构体
typedef struct 
{
      u8    keyval;     // 键值
      u8    backval;    // 后备键值，松手后赋值给keyval
      u8    timer;      // 计时长按
      u8    timer_sw;   // 计时开关，松手时才会进行下一次的计时 
      u8    flag_long;  // 长按标志

}keyStr;

typedef enum{

	GPIO_MODE_OUT,
	GPIO_MODE_IN,
}keyMOde;

// 键值
#define		KEY_NULL		0
#define		KEY_MOD1		LED_MOD1
#define		KEY_MOD2		LED_MOD2
#define		KEY_MOD3		LED_MOD3
#define		KEY_MOD4		LED_MOD4
#define		KEY_MOD5		LED_MOD5
#define		KEY_MOD6		LED_MOD6
#define		KEY_MOD7		LED_MOD7
#define		KEY_MOD8		LED_MOD8
#define		KEY_MOD9		LED_MOD9
#define		KEY_MOD10		LED_MOD10
#define		KEY_MOD11		LED_MOD11
#define		KEY_MOD12		LED_MOD12
#define		KEY_FAN			40
#define		KEY_PROOF		LED_PROOF
#define		KEY_AUX			LED_AUX
#define		KEY_F			41
#define		KEY_MOTO		LED_MOTO		// MOTO其实是排气，我理解错了
#define		KEY_STEAM		LED_STEAM
#define		KEY_P			LED_P
#define		KEY_START		LED_START
#define		KEY_STEAMBOX	LED_STEAMBOX
#define		KEY_SETSMG1_UP			42
#define		KEY_SETSMG1_DOWN		43
#define		KEY_SETSMG2_UP			44
#define		KEY_SETSMG2_DOWN		45
#define		KEY_SETSMG3_UP			46
#define		KEY_SETSMG3_DOWN		47
#define		KEY_POWER				LED_ONOFF
#define		KEY_LED					LED_LED
#define		KEY_WATER				LED_NOWATER



// 按键扫描
// 最高位是长按标志
// 返回非零代表有按键结果
// 松手检测短按，不松手检测长按
u8 KeyScan(void);
void Key_GPIO_Init(void);
u8 KeyRead();	// 判断有没有按键按下

#endif
