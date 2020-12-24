#include "key.h"
#include "KEYSCAN.H"
#include "printf.h"


u32 Get_sys_time(void)  // 1msΪ��λ
{
  extern u32 sys_time;
  return sys_time;  		//ȫ�ֱ�����1ms�ж�
}
/*****************************************************************************
�������� : Get_Key_Press_Time
�������� : ��ȡ��������ʱ��
������� : last_time:�ϴΰ���ʱ��
���ز��� : 2�ΰ���֮���ֵ
ʹ��˵�� : ��
*****************************************************************************/
static u32 Get_Key_Press_Time(u32 last_time)
{
  return (u32)(Get_sys_time() - last_time);
}


// �ص�ǰ���µļ�ֵ
u8 Key_Scan(KEY_STATE_T* Key_State)
{
	u32 time = 0;
	u8 key = 0;
	static unsigned char  flag=0;
	
	switch(Key_State->Status)
	{
		case KEY_NO:
			Key_State->val = 0;
			key = KeyRead();  //��ѯ�а�������
			if(key)//˵���а������� ��ʱ����ֵ
			{
				flag=0;
				Key_State->Status = KEY_DOWN;
				Key_State->Press_Time = Get_sys_time();
			}				

			break;
		case KEY_DOWN:
			if(flag==0)  // ֻ��һ�μ��
			{  //�������ĸ���������
				flag=1;	
				Key_State->key_val = KeyScan();	
				Key_State->Status = KEY_DOWN2;
			}
			break;
		case KEY_DOWN2:
			key = KeyRead();  //��ѯ�а�������
			if(key==0)//˵����ǰ�İ����ѱ��ͷ�
			{
				Key_State->Status = KEY_UP;
			}
			break;
		default:
			break;
	}
	if((Key_State->Status == KEY_DOWN2))
	{
		Key_State->val_led = Key_State->key_val;
		time = Get_Key_Press_Time(Key_State->Press_Time);
		if(time >= TIME_PRESS_LONG && flag) //������
		{
			flag = 0;
			Key_State->val = Key_State->key_val | LONG_PRESS;
		}else Key_State->val = 0;
	 
	 }
	 else if(Key_State->Status == KEY_UP)
	 {
		Key_State->Status = KEY_NO;
		time = Get_Key_Press_Time(Key_State->Press_Time);
		Key_State->Press_Time = 0;
		if((time >= TIME_PRESS_SHORT) && (time < TIME_PRESS_LONG)) //�̰���
		{			
			Key_State->val = Key_State->key_val;
			
		}
		Key_State->key_val = 0;
		Key_State->val_led |= 0x80;
	 }	
	

	return Key_State->val;
}


	
	
	
	
	