/**
 *  数码管控制芯片TM1640驱动
 *  2020年8月16日16:57:29 
 * 
 */
#ifndef __TM1640__H
#define __TM1640__H
#include "uhead.h"
sbit    DIN = P2^2;
sbit    SCLK = P2^1;

#define     DIN_0       DIN = 0
#define     DIN_1       DIN = 1
#define     SCLK_0      SCLK = 0
#define     SCLK_1      SCLK = 1
#define     DEFAULT_LIGHT   7   // 默认亮度



#define     CMD_DATA    0X40    // 数据命令设置
#define     CMD_SHOW    0X80    // 显示命令设置
#define     CMD_ADDR    0XC0    // 地址命令设置

// 数据命令设置
#define     DATCMD_ATU          (CMD_DATA|0X00) // 地址自动加1
#define     DATCMD_FIXADDR      (CMD_DATA|0X04) // 固定地址

// 地址命令设置
#define     ADDRSET(addr)       (CMD_ADDR|addr) // addr 0~0x0f,选择地址段

// 显示命令设置
#define     SHOW_ON     (CMD_SHOW|0x08|DEFAULT_LIGHT)       // 开显示          
#define     SHOW_OFF    (CMD_SHOW)                          // 关显示  

/* 函数 */

void TM1640_AtoShow(u8 startaddr,u8* buf,u8 num);

#endif // !__TM1640__H
