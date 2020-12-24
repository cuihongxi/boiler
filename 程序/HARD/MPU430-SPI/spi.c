#include <INTRINS.H>

#include "STC8.h"
#include "spi.h"
#include "crc.h"
extern	u32 Get_sys_time(void);  // 1msΪ��λ

TempCtrl_t tc[TCHSNUM] = {0};
int  roomTemp = 0;
int  fullTemp = 0;

bit rst430TimeEn = 0;
u32 rst_430time = 0;
SPI_str_t spi = {0};
uchar restart430;

//�����������0|0|a|a|a|a|a|a:�����a��ָ��6λ��ַ��0~63��
//              0|1|1|1|1|1|1|0:�������ȡCRCֵ�����ֽڣ�
//              0|1|1|1|1|1|1|1:�������ȡCRCֵ�����ֽڣ�
//              1|0|a|a|a|a|a|a:д���a��ָ��6λ��ַ��0~63�������ֽ�д��ʱΪ�׵�ַ��
//              1|1|0|0|d|d|d|d:д���d��4λ����
//              1|1|1|0|1|1|1|1:д���д���ݽ������ڽ������һ����������
//              1|1|1|1|1|0|1|0:֡��������
//                         ��1���ֽ�    ��2���ֽ�         ~ ~ ~
//����֡��ʽ�����������ݣ�00+����1��ַ|00+����2��ַ|00+����n��ַ|0x7E    |0x7F     |0xFA��������
//            �ӻ������ݣ�      xx    | ����1      | ����2      | ����n  |CRC���ֽ�|CRC���ֽ�   
//            ����д���ݣ�10+д���ַ |1100+����1��4λ|1100+����1��4λ|1100+����n��4λ|1100+����n��4λ|0xEF           |0xFA��������
//            �ӻ������ݣ�xx          |10+д���ַ    |1100+����1��4λ|1100+����1��4λ|1100+����n��4λ|1100+����n��4λ|0xEF 
//������ʱ����CRCУ�飨�ӻ��Խ��պͷ��͵�������Ч���ݽ���CRCУ������ٷ��͸���������������ȡ���ݺ�CRCֵ������ݽ���CRC����У�飬��У�鲻��ȷ�����¶�ȡ���ݣ������ط���������5����������󱨾���
//д����ʱ�ӻ��յ���������������д�룬�����Խ��յ������뷢�͵����ݽ��бȶ�У�飬��У�鲻��ȷ������д���ݣ������ط���������5����������󱨾���
//Ϊ�˱������ݴ�����ң��ӻ�����ÿ�ֽ�����ʱ��Ӧ�ж������Ƿ���ȷ
//��д������ע��ӻ�����ΪС�˸�ʽ

#define TIMER3_RELOAD (65536UL-MAIN_Fosc/12/1000*BYTE_INTV_T) //Timer 3����ֵ,12Tģʽ,BYTE_INTV_T(ms)ʱ�� 


/*********Timer3��ʼ������ *************/
void InitTimer3(void)
{
	//T4T3M |=T3x12;  //1Tģʽ
  IE2   |=ET3;     //Timer3 interrupt enable
  T3H = (uchar)(TIMER3_RELOAD/256);
  T3L = (uchar)(TIMER3_RELOAD%256);
}

void SPI_Init()  //SPI��ʼ��
{
	InitTimer3();
	SPI_SSIG_None();	//disable SS
	SPI_Enable();
	SPI_MSB_First();
	SPI_Master(); //SPI��Ϊ����ģʽ
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



//////SPI�����ݴ�������//////
void SPI_RdTrigTx(uchar addr,uchar len) //addr�ӻ��Ĵ����׵�ַ���ֽڵ�ַ����len�����ݳ���
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

//////SPIд���ݴ�������//////
void SPI_WrTrigTx(uchar addr,uchar len,uchar *pt) //addr�ӻ��Ĵ����׵�ַ���ֽڵ�ַ����lenд���ݳ��ȣ�ptָ���д�����ݵ�ָ��
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

//////SPIͨ�Ŵ�����//////
void SPI_commProcess()
{
	uchar i,j;
	uint crcVal;
  int tmp16;	
	
	if(spi.txEnd==1) //֡���ͻ������ɴ���
	{
		if(spi.status==1||spi.status==3) //������
		{
			crcVal=0xFFFF;
			for(i=0;i<spi.length;i++) //CRCУ����㣬��ӻ�����˳�򱣳�һ��
			{
				crcVal=CRC16_2(&spi.txBuf[i],1,crcVal);
				crcVal=CRC16_2(&spi.rxBuf[i+1],1,crcVal);
			}
			if(spi.rxBuf[spi.length+1]==(uchar)crcVal&&spi.rxBuf[spi.length+2]==(uchar)(crcVal>>8)) //CRCУ����ȷ
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
								  tc[i].measPV=(tmp16+5)/10; //��������
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
					if(restart430<5) //�ط������ﵽһ��ֵʱ����MSP430
					{
						RST_MSP430(); //��λMSP430
						restart430++;
						spi.retryCnt=0;
						for(i=0;i<10;i++)
							_nop_();
						STOP_RST_MSP430(); //ֹͣ��λMSP430
						//rst430Time=0;
						rst_430time = Get_sys_time();
						rst430TimeEn=1; //��ʼ�ȴ���ʱ
					}
					else
					{
						spi.error=1; //�������󱨾�
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
			if(spi.status==2) //д����
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
	else //���ڽ�������
	{
		if(spi.status==0) //ͨ�ſ���
		{
			if(spi.idleTime>=FRAME_INTV_T) //���м�ʱ���ڵ���֡���ͼ��ʱ����������
			{
				if(spi.calibCmd!=0)
				{
					spi.commSel=2;
				}
				if(spi.idleTime>=MAX_IDLE_T&&spi.commSel==0) //�����м�ʱ������ж�����
					spi.commSel=1;
				
				switch (spi.commSel)
				{
					case 0:
						break;
					case 1:
						if(1)//(interface!=17)//����У׼����
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
	
	if(!spi.enable) //��ֹͨ��
	{
		spi.retry=0;
		spi.retryCnt=0;
		spi.error=0;
		spi.status=0;
		spi.commSel=0;
	}
	if(spi.retry==1) //�����ط�
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
	 else //�ӻ�430��λ��ȴ���ʱ
	 {
		 if((u32)(Get_sys_time() - rst_430time) >= 3000) //�ȴ�3s
			 rst430TimeEn=0;
	 }
	}
}

//////SPI�жϺ���//////
void SPI_IntProcess(void) interrupt 9 
{
	uchar i,nextFlag=0;
//	debug("SPI_IntProcess\r\n");
	SPSTAT = SPIF + WCOL;	//��0 SPIF��WCOL��־
	spi.rxBuf[spi.txCnt]=SPDAT;
	spi.txCnt++;
  if(spi.status==1||spi.status==3) //�ڶ�����
	{
		nextFlag=1; 
	}
	else
		if(spi.status==2) //��д����
		{
			if(spi.txCnt>1)
			{
				if(spi.rxBuf[spi.txCnt-1]!=spi.txBuf[spi.txCnt-2]&&spi.txBuf[spi.txCnt-2]!=WREND_CMD)
				{
					if(spi.retryCnt<5)
						spi.retryCnt++;
					else 
					{
						if(restart430<5) //�ط������ﵽһ��ֵʱ����MSP430
					    {
						    RST_MSP430(); //��λMSP430
						    restart430++;
						    spi.retryCnt=0;
						    for(i=0;i<10;i++)
							    _nop_();
						    STOP_RST_MSP430(); //ֹͣ��λMSP430
							rst_430time = Get_sys_time();
							rst430TimeEn=1; //��ʼ�ȴ���ʱ
						}
						else
						{
							spi.error=1; //�������󱨾�
							if(spi.retryCnt<200)
							  spi.retryCnt++;
							else
							  restart430=4;
						}
					}
					spi.retry=1;
				}
				else //���ݱȶ���ȷ
					nextFlag=1;
			}
			else
        nextFlag=1; 				
		}
	if(nextFlag)
	{
		if(spi.txBuf[spi.txCnt-1]!=END_CMD) //�շ��������ݲ��ǽ����������������
		{
			T4T3M |=T3R; //Timer3��ʼ��ʱ��������
		}
		else		
		  spi.txEnd=1;
	}
}

/********************** Timer3�жϺ���������SPIͨ�� ***************/
void Timer3IntProcess()  interrupt 19
{
	T4T3M &=~T3R; //Timer3ֹͣ����
	AUXINTIF &=~T3IF; //���жϱ�־
	SPDAT = spi.txBuf[spi.txCnt];
}