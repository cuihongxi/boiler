#include "interface.h"
#include "addProcessTask.h"
#define	SMG_SHOW_F			11
extern u8 smg[12];
extern u32 sys_time;	
sysParam pa = {0};		// ϵͳ��ز�����������

interface waiteInerface = {Main_keyfun,Show_WorkTime,Show_Temperature,Show_SteamTime}; // ����״̬����
interface faceClose = {Close_keyfun,Close_timeShow,ClosetempShow,ClosesteamShow};		// �ػ�����
interface facePro = {Pro_keyfun,Pro_timeShow,Pro_tempShow,Pro_teamShow};				// ��̽���


//  ��˸����
u16 Twinkle_SMG(u16 dat)
{
	static u32 time = 0;
	static u8 on_off = 0;
	if((sys_time - time) > 500)
	{
		time = sys_time;
		on_off = !on_off;
	}
	if(on_off) return dat;
	else return 0xffff;
}
void Pro_timeShow(sysParam*  param)
{
	// Time_Show(Twinkle_SMG(param->prodat.set[param->prodat.setnum].worktime));		// ��ʾ��ǰ��ʱ��
	Time_Show(param->prodat.set[param->prodat.setnum].worktime);
}

void Pro_tempShow(sysParam*  param)
{
	Temp_Show(param->prodat.set[param->prodat.setnum].settemp);		// ��ʾ��ǰ�¶�
}

void Pro_teamShow(sysParam*  param)
{
	Steam_Show(param->prodat.set[param->prodat.setnum].steamtime);	// ��ʾ����ʱ��
}


// ��˸���ָʾ��
void TwinkleProLed_Fun()
{
	static u32 time = 0;
	static u8 on_off = 0;
	if((sys_time - time) > 500)
	{
		time = sys_time;
		on_off = !on_off;
		ProLED_Show(on_off);
	}
}

// ��̽���
void Pro_keyfun(sysParam*  param)
{
	static u8 flag_init = 0;
	if(param->keyval)	// ��ֵ����
	{
		printf("param.keyval = %d\r\n",param->keyval);
		if(flag_init == 0 && param->keyval <= KEY_MOD12)
		{
			flag_init = 1;
			ProLED_Show(0);	// �رձ��ָʾ��
		}
			
		LED_MOD_Select(param->keyval);// ����MOD��Ӧ��LED
		switch(param->keyval)
		{
			case KEY_POWER:	
				flag_init = 0;
				Key_led_off();
				param->pInterface = &faceClose;
				//debug("faceClose\r\n");
			break;
		}

		
		if(flag_init)
		{
			Fun_KeyFun(param->keyval);	// ����ɨ�裬���ݼ�ִֵ�ж�Ӧ�ĺ���
			FAndFanLED_Show(pa.prodat.setnum,pa.prodat.set[pa.prodat.setnum].funspeed);	// F �ͷ����������ʾ
			if((param->keyval & 0x7f) == KEY_P) // �����̰�P������
			{
				flag_init = 0;
				ProLED_Show(0);	// �رձ��ָʾ��
				param->pInterface = &waiteInerface;// ���ش�������
			}
		}
			
				
		param->keyval = 0;

	}else
	{
		if(flag_init == 0)
		{
			TwinkleProLed_Fun();	// ���LED��˸
		}
	}
}


/*******************************��������************************************/
// ������������
void Main_keyfun(sysParam*  param)
{
	if(param->keyval)	// ��ֵ����
	{
		if(param->keyval&LONG_PRESS)	// ����
		{
			printf("waiteInerface L: %d\r\n",param->keyval&(~LONG_PRESS));			
		}
		else 					// �̰�
		{
			printf("waiteInerface S: %d\r\n",param->keyval);
			switch(param->keyval)
			{
				case KEY_P:			
					if(param->timsta == off)	// ��ʱ��δ����״̬�£���P����1~12��˸
					{
						param->pInterface = &facePro;	// �����̽���
					}
					break;
				case KEY_POWER:	
					param->pInterface = &faceClose;
					debug("faceClose\r\n");
				break;
			}

		}
		
		param->keyval = 0;

	}
}

// ��ʾ����ʱ��
void Show_WorkTime(sysParam*  param)
{
	Time_Show(param->workSeconds);
}

// ��ʾ�¶�
void Show_Temperature(sysParam*  param)
{
	Temp_Show(param->temp);
}

// ��ʾ����ʱ��
void Show_SteamTime(sysParam*  param)
{
	Steam_Show(param->steamSeconds);
}


/******************************�ػ�����*************************************/
void Close_keyfun(sysParam*  param)
{
	//u8 dat = 0x88;
	//u8 a = 0;
	if(param->keyval)	// ��ֵ����
	{
		if(param->keyval&LONG_PRESS)	// ����
		{
			printf("faceClose L: %d\r\n",param->keyval&(~LONG_PRESS));
			
		}
		else 					// �̰�
		{
			printf("faceClose S: %d\r\n",param->keyval);
			switch(param->keyval)
			{
				case KEY_POWER:
					param->pInterface = &waiteInerface;// ��������
				break;
//				case KEY_MOD1:
//					debug("save dat\r\n");
//					flashSave(0, &dat,1);
//				break;
//				case KEY_MOD2:
//					flashRead(0, &a,1);
//					printf("a = %d\r\n",a);
//				break;
								
			}
		}
		
		param->keyval = 0;

	}
}

void Close_timeShow(sysParam*  param)	// ʱ���������ʾ����
{
	param->keyval = param->keyval;
	smgbuf[6] = 0;
	smgbuf[7] = 0;
	smgbuf[8] = 0;
	smgbuf[9] = 0;	
}

void ClosetempShow(sysParam*  param)			// �¶��������ʾ����
{
	param->keyval = param->keyval;
	LedAlloff();
	smgbuf[0] = smg[0];
	smgbuf[1] = smg[SMG_SHOW_F];
	smgbuf[2] = smg[SMG_SHOW_F];
}

void ClosesteamShow(sysParam*  param)		// �����������ʾ����
{
	param->keyval = param->keyval;
	smgbuf[3] = 0;
	smgbuf[4] = 0;
	smgbuf[5] = 0;
}























