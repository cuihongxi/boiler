/*
	与MPU430通讯读取温度的文件
	2020年11月15日08:44:10移植
	tc[i].measPV 是测量的温度 ，roomTemp是板子上的芯片测量的室温
	
*/
#ifndef	_SPI_H
#define	_SPI_H

#include "uhead.h"

#define 	TCHSNUM  		2     		// 温控通道总数量
#define 	TC_NC_ERRCODE  	0x8000 		// TC断线故障码


#define	PIN0		0x01	//IO引脚 Px.0
#define	PIN1		0x02	//IO引脚 Px.1
#define	PIN2		0x04	//IO引脚 Px.2
#define	PIN3		0x08	//IO引脚 Px.3
#define	PIN4		0x10	//IO引脚 Px.4
#define	PIN5		0x20	//IO引脚 Px.5
#define	PIN6		0x40	//IO引脚 Px.6
#define	PIN7		0x80	//IO引脚 Px.7

#define RST_MSP430()         P5 &=~PIN4 //复位MSP430
#define STOP_RST_MSP430()    P5 |=PIN4  //停止复位MSP430

//sbit  SPI_SS    = P1^2;
sbit  SPI_MOSI  = P1^3;
sbit  SPI_MISO  = P1^4;
sbit  SPI_SCLK  = P1^5;

//sfr SPCTL  = 0xCE;	SPI控制寄存器
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00

#define	SPI_SSIG_None()		SPCTL |=  (1<<7)		/* 1: 忽略SS脚	*/
#define	SPI_SSIG_Enable()	SPCTL &= ~(1<<7)		/* 0: SS脚用于决定主从机	*/
#define	SPI_Enable()		SPCTL |=  (1<<6)		/* 1: 允许SPI	*/
#define	SPI_Disable()		SPCTL &= ~(1<<6)		/* 0: 禁止SPI	*/
#define	SPI_LSB_First()		SPCTL |=  (1<<5)		/* 1: LSB先发	*/
#define	SPI_MSB_First()		SPCTL &= ~(1<<5)		/* 0: MSB先发	*/
#define	SPI_Master()		SPCTL |=  (1<<4)		/* 1: 设为主机	*/
#define	SPI_Slave()			SPCTL &= ~(1<<4)		/* 0: 设为从机	*/
#define	SPI_SCLK_NormalH()	SPCTL |=  (1<<3)		/* 1: 空闲时SCLK为高电平	*/
#define	SPI_SCLK_NormalL()	SPCTL &= ~(1<<3)		/* 0: 空闲时SCLK为低电平	*/
#define	SPI_PhaseH()		SPCTL |=  (1<<2)		/* 1: 	*/
#define	SPI_PhaseL()		SPCTL &= ~(1<<2)		/* 0: 	*/
#define	SPI_Speed(n)		SPCTL = (SPCTL & ~3) | (n)	/*设置速度, 0 -- fosc/4, 1 -- fosc/8, 2 -- fosc/16, 3 -- fosc/32	*/

//sfr SPDAT  = 0xCF; //SPI Data Register                                                     0000,0000
//sfr SPSTAT  = 0xCD;	//SPI状态寄存器
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
//#define	SPIF	0x80		/* SPI传输完成标志。写入1清0。*/
//#define	WCOL	0x40		/* SPI写冲突标志。写入1清0。  */


#define		SPI_INT_ENABLE()		IE2 |=  2	/* 允许SPI中断		*/
#define		SPI_INT_DISABLE()		IE2 &= ~2	/* 禁止SPI中断		*/

///////////////////////////////////////////////////////////////////
#define WR_CMD      0x80      //写命令
#define WRDATA_CMD  0xC0      //写命令（数据）
#define RDCRCL_CMD  0x7E      //读CRCL命令
#define RDCRCH_CMD  0x7F      //读CRCH命令
#define WREND_CMD   0xEF      //写数据结束
#define END_CMD     0xFA      //帧结束命令
#define BYTE_INTV_T   1   //字节发送间隔时间ms
#define FRAME_INTV_T  10  //帧发送间隔时间ms
#define MAX_IDLE_T    100 //最大空闲等待时间ms(最大空闲时间到则进行读数据)
#define	SPI_BUFLEN	  13  //指定缓存长度，13字节（最多连续读10字节或写5字节数据） 
#define RDREG_ADDR    0   //读寄存器首地址（字节地址）
#define WRREG_ADDR    0   //写寄存器首地址（字节地址）
#define COMM_LEN      50  //通信表总字节长度
#define RD_MAXADDR  (COMM_LEN-1)            //读数据最大地址
#define WD_MAXADDR  (WRREG_ADDR+4-1)        //写数据最大地址

typedef struct
{
	uchar enable;  //通讯使能
	uchar commSel; //通讯选择：0-无，1-读数据,2-写数据（校准数据及命令）
	//uchar ordComm; //顺序通讯：0-无，1-顺序通讯1，2-顺序通讯2
	//uchar commSn;  //顺序通讯序号
	uchar status;  //SPI通信状态，0-闲，1-读常用温度数据，2-写校准数据及命令，3-读芯片温度及满温等
	uchar addr;    //通信数据地址
	uchar length;  //通信数据长度
	uchar txCnt;   //发送计数
	uchar txEnd;   //发送结束
	uchar error;
	uchar retry;   //重发标志
	uchar retryCnt; //重发计数
	uchar idleTime; //空闲计时
	uchar txBuf[SPI_BUFLEN];
	uchar rxBuf[SPI_BUFLEN];
	 ////校准控制////
  int calibDat; //校准数据
  int calibCmd; //校准命令,执行完成清零
}SPI_str_t;

typedef struct
{
	uchar chEnable,run,heat,reachSta,error,heatStart; //overTemp
	uchar heatRlyCtr;
	uchar filtSta,SVReach,PVDetCnt,PVDetNum,PVDecAdjCnt,PVAccAdjCnt,PVAdjSta,PVAdjCnt,PVContChg,filtPVContChg,filtPVFastChg,PVAverSta;//滤波用
	uint PVDetSum,PVDetAver;//滤波用
	uint heatCtrTime,outPeriod_ms,outOnTime;
	int PV,measPV,prePV,aftFiltPV,oldSV;
	int SV,PVComp2;    //用户参数
	int tolerance,PVMax;
	int SVMax,heatBackDev,heatDead,powerOut,powerLimit,outPeriod,outTimeMini;    //系统参数
	int PVComp,PVGain,filtEn,SVFiltFac; //工程参数
}TempCtrl_t;

void SPI_Init();
void SPI_RdTrigTx(uchar addr,uchar len);
void SPI_WrTrigTx(uchar addr,uchar len,uchar *pt);
void SPI_commProcess();
void InitTimer3(void);	// 用于SPI通讯

extern SPI_str_t spi;
extern	int  roomTemp;
extern	int  fullTemp;
extern	TempCtrl_t tc[TCHSNUM];
#endif



