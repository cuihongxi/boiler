/*
	2020-8-2
	��ӹ�������
	2020��11��22��14:38:23
	�޸��ļ�
	
*/
#include "uhead.h"

/* ��ֲ */
#include "myFlash.h"
#include "keyscan.h"
#include "SMG.H"



#define		flashSave(addr, pbuf,len)	Flash_Write(addr,pbuf,len);	// flash ����
#define		flashRead(addr, pbuf,len)	Flash_Read(addr,pbuf,len)	// flash ��

#define	 	ShowTemp(temp)			Temp_Show(temp)		// ��ʾ�¶�
#define	 	ShowTime(time)			Time_Show(time)		// ��ʾʱ��
#define		ShowSteam(time)			Steam_Show(time)	// ����ʱ��

// ��ֵ����
#define		TIMEADD		KEY_SETSMG1_UP			// ʱ����ֵ���Ӱ���
#define		TIMESUB		KEY_SETSMG1_DOWN		// ʱ����ֵ���ٰ���
#define		TEMPADD		KEY_SETSMG2_UP			// �¶���ֵ���Ӱ���
#define		TEMPSUB		KEY_SETSMG2_DOWN		// �¶���ֵ���ٰ���
#define		SAVEDAT		LONG_PRESS|KEY_P	// ��ֵ���水��
#define		GROUP_1		KEY_MOD1	// �趨1�װ���
#define		GROUP_2		KEY_MOD2	// �趨2�װ���
#define		GROUP_3		KEY_MOD3	// �趨3�װ���
#define		GROUP_4		KEY_MOD4	// �趨4�װ���
#define		GROUP_5		KEY_MOD5	// �趨5�װ���
#define		GROUP_6		KEY_MOD6	// �趨6�װ���
#define		GROUP_7		KEY_MOD7	// �趨7�װ���
#define		GROUP_8		KEY_MOD8	// �趨8�װ���
#define		GROUP_9		KEY_MOD9	// �趨9�װ���
#define		GROUP_10	KEY_MOD10	// �趨10�װ���
#define		GROUP_11	KEY_MOD11	// �趨11�װ���
#define		GROUP_12	KEY_MOD12	// �趨12�װ���
#define		STEAMADD	KEY_SETSMG3_UP			// ������ֵ���Ӱ���
#define		STEAMSUB	KEY_SETSMG3_DOWN		// ������ֵ���ٰ���
#define		FUNADD		KEY_FAN		// ������ֵ���Ӱ���
#define		F_ADD		KEY_F		// F����


// EEPROM ��ַ����
#define		INIT_DAT		0X9A	// ��ʼ��ֵ

#define		ADDR_INIT		0	// �����ʼ��ֵ�ĵ�ַ
#define 	ADDR_NORMAL		512		// һ����ʾ����ĵ�ַ,������ʼ��ַ
#define		ADDR_GROUP1		1024	// ������ʼ��ַ��1�ױ����ַ,һ��ռ��40���ֽ�,12��ռ��600�ֽ�



/* ���� */

void GroupDatInit( );			// ��ʼ��
void Fun_KeyFun( u8 keyval);	// ����ɨ�裬���ݼ�ִֵ�ж�Ӧ�ĺ���
void Fun_ShowFun( );			// ��ʾ����



