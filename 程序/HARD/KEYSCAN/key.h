#ifndef  __KEY_H
#define  __KEY_H
#include "uhead.h"

typedef struct
{
	u32 Press_Time;	// ��¼��һ�ΰ��µ�ʱ��
	u8 val;
	u8 Status;
	u8 key_val;
	u8 val_led;	// ������һ��̧������
}KEY_STATE_T;


#define		KEY_NO 		0
#define		KEY_DOWN	1
#define		KEY_UP		2
#define		KEY_DOWN2	3

#define		TIME_PRESS_LONG 	2000	// ���峤����ʱ��
#define		TIME_PRESS_SHORT	5		// �̰�����


u8 Key_Scan(KEY_STATE_T* Key_State); // �ص�ǰ���µļ�ֵ

#endif // !1
