#include "show.h"
#include "TM1640.H"

// ��ʾ
// �ڶ�ʱ���У���ʱˢ�����е����������
void ShowInTimer(u8* buff,u8 num)
{
    TM1640_AtoShow(0,buff,num);
}