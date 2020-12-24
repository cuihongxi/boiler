/*
2020��11��15��14:02:12
	������ʾ������ɨ��ӿ�
	�ú�����������ִ�еĺ������������LED��ô��ʾ
*/
#ifndef	__INTERFACE_H
#define	__INTERFACE_H
#include "uhead.h"
#include "key.h"
#include "smg.h"
#include "KEYSCAN.H"
#include "addprocessTask.h"



extern sysParam pa;
// ������ʾ�ṹ��
typedef struct{
	void(*keyfun)(sysParam*  param); 		// ��ֵ������
	void(*timeShow)(sysParam*  param);		// ʱ���������ʾ����
	void(*tempShow)(sysParam*  param);		// �¶��������ʾ����
	void(*steamShow)(sysParam*  param);		// �����������ʾ����
//	void(*fun)(sysParam*  param);			// ���ܺ���
}interface;





extern interface faceClose ;

void Null_Fun(sysParam*  param);// ɶҲ��������
void TwinkleProLed_Fun();// ��˸���ָʾ��
void Pro_keyfun(sysParam*  param);// ��̽���
void Main_keyfun(sysParam*  param);// ������������
void Show_WorkTime(sysParam*  param);// ��ʾ����ʱ��
void Show_Temperature(sysParam*  param);// ��ʾ�¶�
void Show_SteamTime(sysParam*  param);// ��ʾ����ʱ��
void Close_keyfun(sysParam*  param);
void Close_timeShow(sysParam*  param);
void ClosetempShow(sysParam*  param);
void ClosesteamShow(sysParam*  param);
void Pro_timeShow(sysParam*  param);
void Pro_tempShow(sysParam*  param);
void Pro_teamShow(sysParam*  param);


#endif

