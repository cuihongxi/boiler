/*
	��MPU430ͨѶ��ȡ�¶ȵ��ļ�
	2020��11��15��08:44:10��ֲ
	tc[i].measPV �ǲ������¶� ��roomTemp�ǰ����ϵ�оƬ����������
	
*/
#ifndef	_SPI_H
#define	_SPI_H

#include "uhead.h"

#define 	TCHSNUM  		2     		// �¿�ͨ��������
#define 	TC_NC_ERRCODE  	0x8000 		// TC���߹�����


#define	PIN0		0x01	//IO���� Px.0
#define	PIN1		0x02	//IO���� Px.1
#define	PIN2		0x04	//IO���� Px.2
#define	PIN3		0x08	//IO���� Px.3
#define	PIN4		0x10	//IO���� Px.4
#define	PIN5		0x20	//IO���� Px.5
#define	PIN6		0x40	//IO���� Px.6
#define	PIN7		0x80	//IO���� Px.7

#define RST_MSP430()         P5 &=~PIN4 //��λMSP430
#define STOP_RST_MSP430()    P5 |=PIN4  //ֹͣ��λMSP430

//sbit  SPI_SS    = P1^2;
sbit  SPI_MOSI  = P1^3;
sbit  SPI_MISO  = P1^4;
sbit  SPI_SCLK  = P1^5;

//sfr SPCTL  = 0xCE;	SPI���ƼĴ���
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00

#define	SPI_SSIG_None()		SPCTL |=  (1<<7)		/* 1: ����SS��	*/
#define	SPI_SSIG_Enable()	SPCTL &= ~(1<<7)		/* 0: SS�����ھ������ӻ�	*/
#define	SPI_Enable()		SPCTL |=  (1<<6)		/* 1: ����SPI	*/
#define	SPI_Disable()		SPCTL &= ~(1<<6)		/* 0: ��ֹSPI	*/
#define	SPI_LSB_First()		SPCTL |=  (1<<5)		/* 1: LSB�ȷ�	*/
#define	SPI_MSB_First()		SPCTL &= ~(1<<5)		/* 0: MSB�ȷ�	*/
#define	SPI_Master()		SPCTL |=  (1<<4)		/* 1: ��Ϊ����	*/
#define	SPI_Slave()			SPCTL &= ~(1<<4)		/* 0: ��Ϊ�ӻ�	*/
#define	SPI_SCLK_NormalH()	SPCTL |=  (1<<3)		/* 1: ����ʱSCLKΪ�ߵ�ƽ	*/
#define	SPI_SCLK_NormalL()	SPCTL &= ~(1<<3)		/* 0: ����ʱSCLKΪ�͵�ƽ	*/
#define	SPI_PhaseH()		SPCTL |=  (1<<2)		/* 1: 	*/
#define	SPI_PhaseL()		SPCTL &= ~(1<<2)		/* 0: 	*/
#define	SPI_Speed(n)		SPCTL = (SPCTL & ~3) | (n)	/*�����ٶ�, 0 -- fosc/4, 1 -- fosc/8, 2 -- fosc/16, 3 -- fosc/32	*/

//sfr SPDAT  = 0xCF; //SPI Data Register                                                     0000,0000
//sfr SPSTAT  = 0xCD;	//SPI״̬�Ĵ���
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
//#define	SPIF	0x80		/* SPI������ɱ�־��д��1��0��*/
//#define	WCOL	0x40		/* SPIд��ͻ��־��д��1��0��  */


#define		SPI_INT_ENABLE()		IE2 |=  2	/* ����SPI�ж�		*/
#define		SPI_INT_DISABLE()		IE2 &= ~2	/* ��ֹSPI�ж�		*/

///////////////////////////////////////////////////////////////////
#define WR_CMD      0x80      //д����
#define WRDATA_CMD  0xC0      //д������ݣ�
#define RDCRCL_CMD  0x7E      //��CRCL����
#define RDCRCH_CMD  0x7F      //��CRCH����
#define WREND_CMD   0xEF      //д���ݽ���
#define END_CMD     0xFA      //֡��������
#define BYTE_INTV_T   1   //�ֽڷ��ͼ��ʱ��ms
#define FRAME_INTV_T  10  //֡���ͼ��ʱ��ms
#define MAX_IDLE_T    100 //�����еȴ�ʱ��ms(������ʱ�䵽����ж�����)
#define	SPI_BUFLEN	  13  //ָ�����泤�ȣ�13�ֽڣ����������10�ֽڻ�д5�ֽ����ݣ� 
#define RDREG_ADDR    0   //���Ĵ����׵�ַ���ֽڵ�ַ��
#define WRREG_ADDR    0   //д�Ĵ����׵�ַ���ֽڵ�ַ��
#define COMM_LEN      50  //ͨ�ű����ֽڳ���
#define RD_MAXADDR  (COMM_LEN-1)            //����������ַ
#define WD_MAXADDR  (WRREG_ADDR+4-1)        //д��������ַ

typedef struct
{
	uchar enable;  //ͨѶʹ��
	uchar commSel; //ͨѶѡ��0-�ޣ�1-������,2-д���ݣ�У׼���ݼ����
	//uchar ordComm; //˳��ͨѶ��0-�ޣ�1-˳��ͨѶ1��2-˳��ͨѶ2
	//uchar commSn;  //˳��ͨѶ���
	uchar status;  //SPIͨ��״̬��0-�У�1-�������¶����ݣ�2-дУ׼���ݼ����3-��оƬ�¶ȼ����µ�
	uchar addr;    //ͨ�����ݵ�ַ
	uchar length;  //ͨ�����ݳ���
	uchar txCnt;   //���ͼ���
	uchar txEnd;   //���ͽ���
	uchar error;
	uchar retry;   //�ط���־
	uchar retryCnt; //�ط�����
	uchar idleTime; //���м�ʱ
	uchar txBuf[SPI_BUFLEN];
	uchar rxBuf[SPI_BUFLEN];
	 ////У׼����////
  int calibDat; //У׼����
  int calibCmd; //У׼����,ִ���������
}SPI_str_t;

typedef struct
{
	uchar chEnable,run,heat,reachSta,error,heatStart; //overTemp
	uchar heatRlyCtr;
	uchar filtSta,SVReach,PVDetCnt,PVDetNum,PVDecAdjCnt,PVAccAdjCnt,PVAdjSta,PVAdjCnt,PVContChg,filtPVContChg,filtPVFastChg,PVAverSta;//�˲���
	uint PVDetSum,PVDetAver;//�˲���
	uint heatCtrTime,outPeriod_ms,outOnTime;
	int PV,measPV,prePV,aftFiltPV,oldSV;
	int SV,PVComp2;    //�û�����
	int tolerance,PVMax;
	int SVMax,heatBackDev,heatDead,powerOut,powerLimit,outPeriod,outTimeMini;    //ϵͳ����
	int PVComp,PVGain,filtEn,SVFiltFac; //���̲���
}TempCtrl_t;

void SPI_Init();
void SPI_RdTrigTx(uchar addr,uchar len);
void SPI_WrTrigTx(uchar addr,uchar len,uchar *pt);
void SPI_commProcess();
void InitTimer3(void);	// ����SPIͨѶ

extern SPI_str_t spi;
extern	int  roomTemp;
extern	int  fullTemp;
extern	TempCtrl_t tc[TCHSNUM];
#endif



