#include <INTRINS.H>

#include "STC8.h"
#include "spi.h"
#include "crc.h"
extern	u32 Get_sys_time(void);  // 1ms为单位

TempCtrl_t tc[TCHSNUM] = {0};
int  roomTemp = 0;
int  fullTemp = 0;

bit rst430TimeEn = 0;
u32 rst_430time = 0;
SPI_str_t spi = {0};
uchar restart430;

//主机发送命令：0|0|a|a|a|a|a|a:读命令，a：指定6位地址（0~63）
//              0|1|1|1|1|1|1|0:读命令，读取CRC值（高字节）
//              0|1|1|1|1|1|1|1:读命令，读取CRC值（低字节）
//              1|0|a|a|a|a|a|a:写命令，a：指定6位地址（0~63）（多字节写入时为首地址）
//              1|1|0|0|d|d|d|d:写命令，d：4位数据
//              1|1|1|0|1|1|1|1:写命令，写数据结束用于接收最后一个回送数据
//              1|1|1|1|1|0|1|0:帧结束命令
//                         第1个字节    第2个字节         ~ ~ ~
//数据帧格式：主机读数据：00+数据1地址|00+数据2地址|00+数据n地址|0x7E    |0x7F     |0xFA（结束）
//            从机回数据：      xx    | 数据1      | 数据2      | 数据n  |CRC低字节|CRC高字节   
//            主机写数据：10+写入地址 |1100+数据1低4位|1100+数据1高4位|1100+数据n低4位|1100+数据n高4位|0xEF           |0xFA（结束）
//            从机回数据：xx          |10+写入地址    |1100+数据1低4位|1100+数据1高4位|1100+数据n低4位|1100+数据n高4位|0xEF 
//读数据时进行CRC校验（从机对接收和发送的所有有效数据进行CRC校验计算再发送给主机），主机获取数据和CRC值后对数据进行CRC计算校验，若校验不正确则重新读取数据，连续重发次数大于5次则产生错误报警。
//写数据时从机收到结束码后进行数据写入，主机对接收的数据与发送的数据进行比对校验，若校验不正确则重新写数据，连续重发次数大于5次则产生错误报警。
//为了避免数据传输混乱，从机接收每字节数据时都应判断命令是否正确
//读写字数据注意从机数据为小端格式

#define TIMER3_RELOAD (65536UL-MAIN_Fosc/12/1000*BYTE_INTV_T) //Timer 3重载值,12T模式,BYTE_INTV_T(ms)时间 


/*********Timer3初始化函数 *************/
void InitTimer3(void)
{
	//T4T3M |=T3x12;  //1T模式
  IE2   |=ET3;     //Timer3 interrupt enable
  T3H = (uchar)(TIMER3_RELOAD/256);
  T3L = (uchar)(TIMER3_RELOAD%256);
}

void SPI_Init()  //SPI初始化
{
	InitTimer3();
	SPI_SSIG_None();	//disable SS
	SPI_Enable();
	SPI_MSB_First();
	SPI_Master(); //SPI设为主机模式
	SPI_SCLK_NormalL();
	SPI_PhaseH();
	SPI_Speed(3);	
	SPI_INT_ENABLE();
	
	spi.status=0;
	spi.txEnd=0;
	spi.error=0;
	spi.retry=0;
	spi.retryCnt=0;
	spi.commSel=0;
	spi.calibDat=0;
	spi.calibCmd=0;
	spi.enable=1;
}



//////SPI读数据触发发送//////
void SPI_RdTrigTx(uchar addr,uchar len) //addr从机寄存器首地址（字节地址），len读数据长度
{
	uchar i,a=0;
	
	if(addr+len-1>RD_MAXADDR||len+3>SPI_BUFLEN)
		return;
	for(i=0;i<len;i++)
	{
		spi.txBuf[i]=addr;
		addr++;
	}
	spi.txBuf[len]=RDCRCL_CMD;
	spi.txBuf[len+1]=RDCRCH_CMD;
	spi.txBuf[len+2]=END_CMD;
	spi.txCnt=0;
	spi.txEnd=0;
	SPDAT = spi.txBuf[0];
}

//////SPI写数据触发发送//////
void SPI_WrTrigTx(uchar addr,uchar len,uchar *pt) //addr从机寄存器首地址（字节地址），len写数据长度，pt指向待写入数据的指针
{
	uchar i,a=0,val;
	
	if(addr+len-1>WD_MAXADDR||(len<<1)+3>SPI_BUFLEN)
		return;
	for(i=1;i<=(len<<1);i++)
	{
		if(a==0)
		{
			val=*pt&0x0F;
			a=1;
		}
		else
		{
			val=(*pt&0xF0)>>4;
			a=0;
			pt++;
		}
		spi.txBuf[i]=WRDATA_CMD+val;	
	}
	spi.txBuf[0]=WR_CMD+addr;
	spi.txBuf[i]=WREND_CMD;
	spi.txBuf[i+1]=END_CMD;
	spi.txCnt=0;
	spi.txEnd=0;
	SPDAT = spi.txBuf[0];
}

//////SPI通信处理函数//////
void SPI_commProcess()
{
	uchar i,j;
	uint crcVal;
  int tmp16;	
	
	if(spi.txEnd==1) //帧发送或接收完成处理
	{
		if(spi.status==1||spi.status==3) //读数据
		{
			crcVal=0xFFFF;
			for(i=0;i<spi.length;i++) //CRC校验计算，与从机计算顺序保持一致
			{
				crcVal=CRC16_2(&spi.txBuf[i],1,crcVal);
				crcVal=CRC16_2(&spi.rxBuf[i+1],1,crcVal);
			}
			if(spi.rxBuf[spi.length+1]==(uchar)crcVal&&spi.rxBuf[spi.length+2]==(uchar)(crcVal>>8)) //CRC校验正确
			{
				// debug("CRC OK\r\n");
				if(spi.status==1)
				{
					for(i=0;i<(spi.length>>1);i++)
					{
						j=i<<1;
						tmp16=(spi.rxBuf[2+j]<<8)+spi.rxBuf[1+j];
						switch(i)
						{
							case 0:
							case 1:
							case 2:
								//debug("TC_NC_ERRCODE\r\n");
								if(tmp16==TC_NC_ERRCODE)
								  tc[i].measPV=TC_NC_ERRCODE;
								else
								  tc[i].measPV=(tmp16+5)/10; //四舍五入
								//printf("tc[%d] = ",i);
								//printf("%d\r\n",tmp16);
								break;
							case 3:
								roomTemp=tmp16;
							  break;
							default:
								break;
					  }		
				  }
				}
				else
				{
					roomTemp=(spi.rxBuf[2]<<8)+spi.rxBuf[1];
					fullTemp=(spi.rxBuf[8]<<8)+spi.rxBuf[7];
				}
				spi.retryCnt=0;
				spi.error=0;
				spi.status=0;
				spi.commSel=0;
				restart430=0;
			}
      else
			{
				if(spi.retryCnt<5)
					spi.retryCnt++;
				else
				{
					if(restart430<5) //重发次数达到一定值时重启MSP430
					{
						RST_MSP430(); //复位MSP430
						restart430++;
						spi.retryCnt=0;
						for(i=0;i<10;i++)
							_nop_();
						STOP_RST_MSP430(); //停止复位MSP430
						//rst430Time=0;
						rst_430time = Get_sys_time();
						rst430TimeEn=1; //开始等待计时
					}
					else
					{
						spi.error=1; //产生错误报警
						if(spi.retryCnt<200)
						  spi.retryCnt++;
					    else
						  restart430=4;
					}
					  
				}
				spi.retry=1;
			}				
		}
		else
			if(spi.status==2) //写数据
			{
				spi.retryCnt=0;
				spi.error=0;
				spi.status=0;
				spi.commSel=0;
				spi.calibCmd=0;
				restart430=0;
			}			
		spi.txEnd=0;
	}
	else //不在结束处理
	{
		if(spi.status==0) //通信空闲
		{
			if(spi.idleTime>=FRAME_INTV_T) //空闲计时大于等于帧发送间隔时间则允许发送
			{
				if(spi.calibCmd!=0)
				{
					spi.commSel=2;
				}
				if(spi.idleTime>=MAX_IDLE_T&&spi.commSel==0) //最大空闲计时到则进行读数据
					spi.commSel=1;
				
				switch (spi.commSel)
				{
					case 0:
						break;
					case 1:
						if(1)//(interface!=17)//进入校准界面
						{
							spi.status=1;
							spi.addr=RDREG_ADDR;
							spi.length=8;
						}
						else
						{
							spi.status=3;
							spi.addr=RDREG_ADDR+6;
							spi.length=8;
						}
						SPI_RdTrigTx(spi.addr,spi.length);
						break;
					case 2:
						spi.status=2;
					  spi.addr=WRREG_ADDR;
					  spi.length=4;
					  SPI_WrTrigTx(spi.addr,spi.length,(uchar *)&spi.calibDat);
						break;
					default:
						break;
				}

			}
		}
	}
	
	if(!spi.enable) //禁止通信
	{
		spi.retry=0;
		spi.retryCnt=0;
		spi.error=0;
		spi.status=0;
		spi.commSel=0;
	}
	if(spi.retry==1) //错误重发
	{
		if(!rst430TimeEn)
		{
			switch (spi.commSel)
			{
				case 0:
					break;
				case 1:
					if(1)//(interface!=17)
					{
						spi.status=1;
						spi.addr=RDREG_ADDR;
						spi.length=8;
					}
					else
					{
						spi.status=3;
						spi.addr=RDREG_ADDR+6;
						spi.length=8;
					}
					SPI_RdTrigTx(spi.addr,spi.length);
					break;
				case 2:
					spi.status=2;
					spi.addr=WRREG_ADDR;
					spi.length=4;
					SPI_WrTrigTx(spi.addr,spi.length,(uchar *)&spi.calibDat);
					break;
				default:
					break;
			}
			spi.retry=0;
	 }
	 else //从机430复位后等待计时
	 {
		 if((u32)(Get_sys_time() - rst_430time) >= 3000) //等待3s
			 rst430TimeEn=0;
	 }
	}
}

//////SPI中断函数//////
void SPI_IntProcess(void) interrupt 9 
{
	uchar i,nextFlag=0;
//	debug("SPI_IntProcess\r\n");
	SPSTAT = SPIF + WCOL;	//清0 SPIF和WCOL标志
	spi.rxBuf[spi.txCnt]=SPDAT;
	spi.txCnt++;
  if(spi.status==1||spi.status==3) //在读数据
	{
		nextFlag=1; 
	}
	else
		if(spi.status==2) //在写数据
		{
			if(spi.txCnt>1)
			{
				if(spi.rxBuf[spi.txCnt-1]!=spi.txBuf[spi.txCnt-2]&&spi.txBuf[spi.txCnt-2]!=WREND_CMD)
				{
					if(spi.retryCnt<5)
						spi.retryCnt++;
					else 
					{
						if(restart430<5) //重发次数达到一定值时重启MSP430
					    {
						    RST_MSP430(); //复位MSP430
						    restart430++;
						    spi.retryCnt=0;
						    for(i=0;i<10;i++)
							    _nop_();
						    STOP_RST_MSP430(); //停止复位MSP430
							rst_430time = Get_sys_time();
							rst430TimeEn=1; //开始等待计时
						}
						else
						{
							spi.error=1; //产生错误报警
							if(spi.retryCnt<200)
							  spi.retryCnt++;
							else
							  restart430=4;
						}
					}
					spi.retry=1;
				}
				else //数据比对正确
					nextFlag=1;
			}
			else
        nextFlag=1; 				
		}
	if(nextFlag)
	{
		if(spi.txBuf[spi.txCnt-1]!=END_CMD) //刚发出的数据不是结束命令则继续发送
		{
			T4T3M |=T3R; //Timer3开始计时触发发送
		}
		else		
		  spi.txEnd=1;
	}
}

/********************** Timer3中断函数，用于SPI通信 ***************/
void Timer3IntProcess()  interrupt 19
{
	T4T3M &=~T3R; //Timer3停止计数
	AUXINTIF &=~T3IF; //清中断标志
	SPDAT = spi.txBuf[spi.txCnt];
}