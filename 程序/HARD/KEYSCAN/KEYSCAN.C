#include "KEYSCAN.H"
#include "printf.h"
#include "delay.h"
u8 IO_KeyState, IO_KeyState1, IO_KeyHoldCnt;    //行列键盘变量
// 端口配置成弱上拉
void Key_GPIO_Init(void)
{
}


//读端口数据
u16    Read_Valu()
{
    u16  dat = 0;
  
    if(SL1 != 0) dat|=0x0001;
    if(SL2 != 0) dat|=0x0002;
    if(SL3 != 0) dat|=0x0004;
    if(SL4 != 0) dat|=0x0008;
    if(SL5 != 0) dat|=0x0010;

    if(SH1 != 0) dat|=0x0100;
    if(SH2 != 0) dat|=0x0200;
    if(SH3 != 0) dat|=0x0400;
    if(SH4 != 0) dat|=0x0800;
    if(SH5 != 0) dat|=0x1000; 
    if(SH6 != 0) dat|=0x2000; 

    return dat;
}

void GPIO_Heng_MOED_SET(keyMOde GPIOMODE)
{
  if (GPIOMODE == GPIO_MODE_IN)
  { 
    SH1 = 1;
    SH2 = 1;
    SH3 = 1;
    SH4 = 1;
    SH5 = 1;
    SH6 = 1;
	_nop_();  // 等待两个时钟
	_nop_(); 
  }else
  {
    SH1 = 0;
    SH2 = 0;
    SH3 = 0;
    SH4 = 0;
    SH5 = 0;
    SH6 = 0;    
  }
}

void GPIO_Lie_MOED_SET(keyMOde GPIOMODE)
{
  if (GPIOMODE == GPIO_MODE_IN)
  {  
    SL1 = 1;
    SL2 = 1;
    SL3 = 1;
    SL4 = 1;
    SL5 = 1;
	_nop_();  // 等待两个时钟
	_nop_();   
  }else
  {
    SL1 = 0;
    SL2 = 0;
    SL3 = 0;
    SL4 = 0;
    SL5 = 0;  
  }  
}

// 判断有没有按键按下
u8 KeyRead()
{
    GPIO_Heng_MOED_SET(GPIO_MODE_OUT);  //横发 0，高8位， 6
    GPIO_Lie_MOED_SET(GPIO_MODE_IN);    //列读数,低8位 ， 5
    if(Read_Valu() != 0X001F)
    {      
		_nop_();  // 等待两个时钟
		_nop_(); 
        if(Read_Valu()!= 0X001F)
        {
			return 1;
		}
	}
	return 0;
}
//检测哪个按键拿下，返回键值
u8  Keyscan()
{
    u16  KeyPort=0; 
    GPIO_Heng_MOED_SET(GPIO_MODE_OUT);  //横发 0，高8位， 6
    GPIO_Lie_MOED_SET(GPIO_MODE_IN);    //列读数,低8位 ， 5
    if(Read_Valu() != 0X001F)
    {      
        KeyPort = Read_Valu();

        if(KeyPort!= 0X001F)
        {  
          GPIO_Heng_MOED_SET(GPIO_MODE_IN);  //横读数
          GPIO_Lie_MOED_SET(GPIO_MODE_OUT);  //列发0 			
          switch (KeyPort)
          {
              case 0X000F:   				  
                  KeyPort = Read_Valu();
                  switch (KeyPort)
                  {								
                      case 0x3E00: return(LED_MOD5);	 
                          break;
                      case 0x3D00: return(LED_MOD10);
                          break;
                      case 0x3B00: return(LED_AUX);
                          break;
                      case 0x3700: return(LED_START);
                          break;
                      case 0x2F00: return(KEY_SETSMG2_DOWN);
                          break;
                      case 0x1F00: return(LED_NOWATER);
                          break;                                        
                  };
                  break;
              case 0X0017:   				  
                  KeyPort = Read_Valu();	
                  switch (KeyPort)
                  {
                      case 0x3E00: return(LED_MOD4);	 
                          break;
                      case 0x3D00: return(LED_MOD9);
                          break;
                      case 0x3B00: return(LED_PROOF);
                          break;
                      case 0x3700: return(LED_P);
                          break;
                      case 0x2F00: return(KEY_SETSMG2_UP);
                          break;
                      case 0x1F00: return(LED_LED);
                          break;     
                  };
                  break;
              case 0X001B:   				  
                  KeyPort = Read_Valu(); 			  
                  switch (KeyPort)
                  {
                      case 0x3E00: return(LED_MOD3);	 
                          break;
                      case 0x3D00: return(LED_MOD8);
                          break;
                      case 0x3B00: return(KEY_FAN);
                          break;
                      case 0x3700: return(LED_STEAM);
                          break;
                      case 0x2F00: return(KEY_SETSMG1_DOWN);
                          break;
                      case 0x1F00: return(LED_ONOFF);
                          break;
                  };
                  break;
              case 0X001D: 
                  KeyPort = Read_Valu();   			  
                  switch (KeyPort)
                  {
                      case 0x3E00: return(LED_MOD2);	 
                          break;
                      case 0x3D00: return(LED_MOD7);
                          break;
                      case 0x3B00: return(LED_MOD12);
                          break;
                      case 0x3700: return(LED_MOTO);
                          break;
                      case 0x2F00: return(KEY_SETSMG1_UP);
                          break;
                      case 0x1F00: return(KEY_SETSMG3_DOWN);
                          break;
                  };
              break;
              case 0X001E: 
                  KeyPort = Read_Valu(); 			  
                  switch (KeyPort)
                  {
                      case 0x3E00: return(LED_MOD1);	 
                          break;
                      case 0x3D00: return(LED_MOD6);
                          break;
                      case 0x3B00: return(LED_MOD11);
                          break;
                      case 0x3700: return(KEY_F);
                          break;
                      case 0x2F00: return(LED_STEAMBOX);
                          break;
                      case 0x1F00: return(KEY_SETSMG3_UP);
                          break;
                  };
              break;
          }
            
        }
    }
    return(KEY_NULL);
    
}







