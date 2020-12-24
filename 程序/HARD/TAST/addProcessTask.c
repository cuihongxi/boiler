#include "addProcessTask.h"
#include "eeprom.h"

extern sysParam pa;
twinkleSMG twinkel = {0};	// ��˸


// ��ֵ��Ӧ�����ṹ
typedef struct 
{
	u8 keyval;
	void(*keyFun)(void);
}keyValFun;

void ClearFlash(void)
{
	u16 i = 0;
	EEPROM_write_Zero(ADDR_NORMAL);
	EEPROM_write_Zero(ADDR_GROUP1);
	EEPROM_write_Zero(ADDR_GROUP1 + 512);
}
// ��ʼ��
void GroupDatInit()
{
    u8 initdat = 0;
	pa.prodat.groupnum = 0;
    pa.prodat.setnum = 0;
    flashRead(ADDR_INIT,&initdat,1);
    if(initdat != INIT_DAT)	// ��һ���ϵ磬δ����ʼ��
	{
		initdat = INIT_DAT;
		flashSave(ADDR_INIT,&initdat,1);
		// ȫ����ʼ��Ϊ0
		ClearFlash();	
		debug("INIT_DAT\r\n");
	}
	
	if(flashRead(ADDR_NORMAL,(u8*)&pa.prodat.set[0],SET_SIZE))
		debug("flashread ADDR_NORMAL erro\r\n");// ��flash����
}

// KEYTIME_ADD ����
void Fun_KeyTimeAdd( )
{
	pa.prodat.set[pa.prodat.setnum].worktime ++;
}

// KEYTIME_SUB ����
void Fun_KeyTimeSub( )
{
	if(pa.prodat.set[pa.prodat.setnum].worktime)
		pa.prodat.set[pa.prodat.setnum].worktime --;
}

// TEMPADD 
void Fun_KeyTempAdd( )
{
	pa.prodat.set[pa.prodat.setnum].settemp ++;
}

// TEMPSUB 
void Fun_KeyTempSub( )
{
	if(pa.prodat.set[pa.prodat.setnum].settemp)
		pa.prodat.set[pa.prodat.setnum].settemp --;
}

// STEAMADD 
void Fun_KeySteamAdd( )
{
	pa.prodat.set[pa.prodat.setnum].steamtime ++;
}

// STEAMSUB 
void Fun_KeySteamSub( )
{
	if(pa.prodat.set[pa.prodat.setnum].steamtime)
		pa.prodat.set[pa.prodat.setnum].steamtime --;

}

// FUNADD 
void Fun_KeyFunAdd( )
{
	pa.prodat.set[pa.prodat.setnum].funspeed ++;
	pa.prodat.set[pa.prodat.setnum].funspeed %= 4;
	printf("$ setnum = %d ",pa.prodat.setnum);
	printf("$ funspeed = %d\r\n",pa.prodat.set[pa.prodat.setnum].funspeed);
}



// ��ֵ���水��
void Fun_KeySave( )
{
	flashSave(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// �����趨����
	//pa.prodat.setnum = 0;
	//pa.prodat.groupnum = 0;
	debug("flash save\r\n");
}

// GROUP_1  �趨1�װ���
void Fun_KeyGroup1( )
{
	pa.prodat.groupnum = 0;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������

}

//   �趨2�װ���
void Fun_KeyGroup2( )
{
	pa.prodat.groupnum = 1;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}
//   �趨3�װ���
void Fun_KeyGroup3( )
{
	pa.prodat.groupnum = 2;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}
//   �趨4�װ���
void Fun_KeyGroup4( )
{
	pa.prodat.groupnum = 3;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}
//   �趨5�װ���
void Fun_KeyGroup5( )
{
	pa.prodat.groupnum = 4;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}
//   �趨6�װ���
void Fun_KeyGroup6( )
{
	pa.prodat.groupnum = 5;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}
//   �趨7�װ���
void Fun_KeyGroup7( )
{
	pa.prodat.groupnum = 6;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}
//   �趨8�װ���
void Fun_KeyGroup8( )
{
	pa.prodat.groupnum = 7;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}

//   �趨9�װ���
void Fun_KeyGroup9( )
{
	pa.prodat.groupnum = 8;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}

//   �趨10�װ���
void Fun_KeyGroup10( )
{
	pa.prodat.groupnum = 9;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}

//   �趨11�װ���
void Fun_KeyGroup11( )
{
	pa.prodat.groupnum = 10;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}

//   �趨12�װ���
void Fun_KeyGroup12( )
{
	pa.prodat.groupnum = 11;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// ������
}

// F����
void Fun_KeyF( )
{
	pa.prodat.setnum ++;
	pa.prodat.setnum %= SET_NUM;
	printf("# setnum = %d ",pa.prodat.setnum);
	printf("# funspeed = %d\r\n",pa.prodat.set[pa.prodat.setnum].funspeed);
}
static keyValFun keyArray[] = {
	TIMEADD,	Fun_KeyTimeAdd,
	TIMESUB,	Fun_KeyTimeSub,
	TEMPADD,	Fun_KeyTempAdd,
	TEMPSUB,	Fun_KeyTempSub,
	SAVEDAT,	Fun_KeySave,
	GROUP_1,	Fun_KeyGroup1,
	GROUP_2,	Fun_KeyGroup2,
	GROUP_3,	Fun_KeyGroup3,
	GROUP_4,	Fun_KeyGroup4,
	GROUP_5,	Fun_KeyGroup5,
	GROUP_6,	Fun_KeyGroup6,
	GROUP_7,	Fun_KeyGroup7,
	GROUP_8,	Fun_KeyGroup8,
	GROUP_9,	Fun_KeyGroup9,
	GROUP_10,	Fun_KeyGroup10,
	GROUP_11,	Fun_KeyGroup11,
	GROUP_12,	Fun_KeyGroup12,
	STEAMADD,	Fun_KeySteamAdd,
	STEAMSUB,	Fun_KeySteamSub,
	FUNADD,		Fun_KeyFunAdd,
	F_ADD,		Fun_KeyF,
	
};
// ����ɨ�裬���ݼ�ִֵ�ж�Ӧ�ĺ���
void Fun_KeyFun(u8 keyval)
{
	u16 i = 0;
	for(i=0;i<GETSIZE(keyArray);i++)
	{
		if(keyArray[i].keyval == keyval) keyArray[i].keyFun();
	}
}



// ��ʾ����
void Fun_ShowFun( )
{
	ShowTime(pa.prodat.set[pa.prodat.setnum].worktime);		// ��ʾ��ǰ��ʱ��
	ShowTemp(pa.prodat.set[pa.prodat.setnum].settemp);		// ��ʾ��ǰ�¶�
	ShowSteam(pa.prodat.set[pa.prodat.setnum].steamtime);	// ��ʾ����ʱ��

	// F �ͷ����������ʾ
	FAndFanLED_Show(pa.prodat.setnum,pa.prodat.set[pa.prodat.setnum].funspeed);
}


