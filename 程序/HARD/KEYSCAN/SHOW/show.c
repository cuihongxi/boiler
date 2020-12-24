#include "show.h"
#include "TM1640.H"

// 显示
// 在定时器中，定时刷新所有的数码管数据
void ShowInTimer(u8* buff,u8 num)
{
    TM1640_AtoShow(0,buff,num);
}