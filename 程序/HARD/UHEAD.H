#ifndef	__UHEAD_H
#define	__UHEAD_H
#include <STC8.H>
#include "uart.h"
#include "printf.h"
#include <delay.h>

#define			GETSIZE(x)		(sizeof(x)/sizeof(x[0]))			//获得数组大小
#define			debug(str)		UARTSendStr(str)

typedef	unsigned char 	    u8;
typedef	unsigned int 	    u16;
typedef	unsigned long 	    u32;

typedef unsigned int		uint;
typedef	unsigned char		uchar;
typedef	unsigned long 	    uint32;

#define     MAIN_Fosc       24000000L   //定义主时钟

#define		GROUP_NUM		12		// 定义套数
#define		SET_NUM			5		// 定义单个套内的数据个数
#define		SET_SIZE		9

/* 数据结构定义 */
typedef struct{
	u16 addr;				// 保存的地址
	u16 worktime;			// 工作时间
	u16 settemp;			// 设定温度
	u16	steamtime;			// 蒸汽时间
	u8	funspeed;			// 当前段的风速
}setStr;
// 套结构，SET_SIZE * SET_NUM = 45
typedef struct{
	setStr set[SET_NUM];			// 一套内的结构	
}groupStr;

// 所有套的结构,45 * 12 = 540
typedef struct{
	setStr 		set[SET_NUM];		//  一套
	u8 			setnum;				// 当前指向的套内的num
	u8 			groupnum;			// 当前指向的套num
}datStr;

// 开关机状态
typedef enum{
	off,
	on,
}btSta;

// 数码管闪烁结构体
typedef struct{
	u32 time;	
	u8 flag;
	u8 obj;
}twinkleSMG;

// 系统参数结构体
typedef struct{
	void* 		pInterface;
	u16 		workSeconds;	// 剩余时间
	u8 			temp;			// 箱内温度
	u8 			steamSeconds;	// 蒸汽时间
	u8 			keyval;			// 键值
	btSta		timsta;			// 定时状态
	datStr 		prodat;			// 运行显示的编程数据

}sysParam;

#endif
