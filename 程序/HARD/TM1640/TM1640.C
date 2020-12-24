#include "TM1640.H"

// CLK高电平时，DAT拉低为开始
void Start_1640()
{
	DIN_1; 
	SCLK_1;
    DIN_0;
    SCLK_0;
}

// CLK高电平时，DAT拉高为结束
void Stop_1640()
{
    SCLK_0;
    DIN_0;
    SCLK_1;
    DIN_1; 
}

// 普通字节传输
void Byte_1640(u8 dat)
{
    u8 i = 0;
    for(i=0;i<8;i++)
    {
        if(dat&(0x01<<i))
            DIN_1;
        else
            DIN_0;
        SCLK_1;
        SCLK_0;
    }
}

// 地址自增函数
// startaddr 起始地址
// num，显示的数量
void TM1640_AtoShow(u8 startaddr,u8* buf,u8 num)
{
    u8 i = 0;
    Start_1640();
    Byte_1640(DATCMD_ATU);  // 自增命令
    Stop_1640();

    Start_1640();
    Byte_1640(ADDRSET(startaddr));  // 设置起始命令
    for(i=0;i<num;i++)
    {
        Byte_1640(buf[i]);
    }
    Stop_1640();

    Start_1640();
    Byte_1640(SHOW_ON);    
    Stop_1640();
}