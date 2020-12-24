#include "addProcessTask.h"
#include "eeprom.h"

extern sysParam pa;
twinkleSMG twinkel = {0};	// 闪烁


// 键值对应函数结构
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
// 初始化
void GroupDatInit()
{
    u8 initdat = 0;
	pa.prodat.groupnum = 0;
    pa.prodat.setnum = 0;
    flashRead(ADDR_INIT,&initdat,1);
    if(initdat != INIT_DAT)	// 第一次上电，未被初始化
	{
		initdat = INIT_DAT;
		flashSave(ADDR_INIT,&initdat,1);
		// 全部初始化为0
		ClearFlash();	
		debug("INIT_DAT\r\n");
	}
	
	if(flashRead(ADDR_NORMAL,(u8*)&pa.prodat.set[0],SET_SIZE))
		debug("flashread ADDR_NORMAL erro\r\n");// 读flash数据
}

// KEYTIME_ADD 函数
void Fun_KeyTimeAdd( )
{
	pa.prodat.set[pa.prodat.setnum].worktime ++;
}

// KEYTIME_SUB 函数
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



// 数值保存按键
void Fun_KeySave( )
{
	flashSave(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 保存设定参数
	//pa.prodat.setnum = 0;
	//pa.prodat.groupnum = 0;
	debug("flash save\r\n");
}

// GROUP_1  设定1套按键
void Fun_KeyGroup1( )
{
	pa.prodat.groupnum = 0;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数

}

//   设定2套按键
void Fun_KeyGroup2( )
{
	pa.prodat.groupnum = 1;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}
//   设定3套按键
void Fun_KeyGroup3( )
{
	pa.prodat.groupnum = 2;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}
//   设定4套按键
void Fun_KeyGroup4( )
{
	pa.prodat.groupnum = 3;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}
//   设定5套按键
void Fun_KeyGroup5( )
{
	pa.prodat.groupnum = 4;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}
//   设定6套按键
void Fun_KeyGroup6( )
{
	pa.prodat.groupnum = 5;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}
//   设定7套按键
void Fun_KeyGroup7( )
{
	pa.prodat.groupnum = 6;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}
//   设定8套按键
void Fun_KeyGroup8( )
{
	pa.prodat.groupnum = 7;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}

//   设定9套按键
void Fun_KeyGroup9( )
{
	pa.prodat.groupnum = 8;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}

//   设定10套按键
void Fun_KeyGroup10( )
{
	pa.prodat.groupnum = 9;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}

//   设定11套按键
void Fun_KeyGroup11( )
{
	pa.prodat.groupnum = 10;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}

//   设定12套按键
void Fun_KeyGroup12( )
{
	pa.prodat.groupnum = 11;
	pa.prodat.setnum = 0;
	flashRead(ADDR_GROUP1 + (SET_NUM*pa.prodat.groupnum)*SET_SIZE,(u8*)&pa.prodat.set[0],SET_SIZE * SET_NUM);// 读参数
}

// F按键
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
// 按键扫描，根据键值执行对应的函数
void Fun_KeyFun(u8 keyval)
{
	u16 i = 0;
	for(i=0;i<GETSIZE(keyArray);i++)
	{
		if(keyArray[i].keyval == keyval) keyArray[i].keyFun();
	}
}



// 显示函数
void Fun_ShowFun( )
{
	ShowTime(pa.prodat.set[pa.prodat.setnum].worktime);		// 显示当前的时间
	ShowTemp(pa.prodat.set[pa.prodat.setnum].settemp);		// 显示当前温度
	ShowSteam(pa.prodat.set[pa.prodat.setnum].steamtime);	// 显示蒸汽时间

	// F 和风速数码管显示
	FAndFanLED_Show(pa.prodat.setnum,pa.prodat.set[pa.prodat.setnum].funspeed);
}


