#include "printf.h"

//double类型转换成ASCII码存到数组中保存正值的函数
//precision 表示小数点后面几位
void DoubletoStringSaveZheng(unsigned char* resultArray0,float dat,unsigned char precision)
{
		unsigned char i=0; 
		unsigned long power = 0;  		//	权
		unsigned long dat0 =0;
		unsigned long POWER = 1;		//10000权
		for(i = 0;i < precision; i++)
		{
			POWER *= 10;
		}	
		
		if((dat-(float)(unsigned long)dat)>0) 
			dat = (dat*POWER + 0.5f)/POWER;//如果是整数就不做0.5的加法
			dat0 = dat;
			for( power=1;dat0>=10;dat0/=10)  
							power*=10;  
			dat0 = dat;
			for(i=0;power>0;power/=10)  
			{  
						resultArray0[i] = '0'+dat0/power; 			
						dat0%=power;  
						i++;
			}           
        dat0 = dat;
        power = 0;
      //存小数部分		
			dat0 = POWER*(dat-dat0);
			power = POWER;			
			 if(power>=10)
			 {						
				 resultArray0[i] = '.';
				 i++; 
				 power/=10;
					for(;power>0;power/=10)  
					{  
								resultArray0[i] = '0'+dat0/power;							
								dat0 %= power;  
								i++;
					}
					resultArray0[i] = '\0';  //添加结束符
							
			 } else
			 resultArray0[i] = '\0';//添加结束符  
			 
}

structLenTyp finddat(unsigned char*str)
{
	structLenTyp LenTyp={0,0};
	unsigned char leng=0;
	while(str[leng] != '%')
	{
		leng++;
	}
	LenTyp.length = leng;	
	leng++;
	LenTyp.type = str[leng];//获取数据类型
	
	return LenTyp;
}



void Int2HexStr(unsigned char* buf,unsigned long dat)
{
	unsigned char dat0[8] = {0};
	unsigned char i = 0;
	unsigned char j = 0;
	char array[] = "0123456789ABCDEF";
	buf[0] = '0';
	buf[1] = 'x';

	
	while(dat)
	{
		dat0[i] = array[(unsigned char)dat & 0x0f];
		dat >>= 4;
		i++;
	}
	while(i)
	{
		i --;
		buf[2+j] = dat0[i];
		j++;
	}
	buf[2+j] = 0;
	
}
void printf(const char*str,unsigned long int dat)
{
	char xdata array[11];
	unsigned char xdata str1[30] = {0};
	unsigned char xdata str2[30] = {0};
	unsigned char i,j;
	structLenTyp strucdat;
	strucdat = finddat(str);
	if(strucdat.type == 'd')DoubletoStringSaveZheng(array,(float)dat,0);
	else if(strucdat.type == 'x') Int2HexStr(array,dat);
	//UART1_SendOneChar(strucdat.length);  
	//unsigned longtostr(dat,array);
	
	
	for(i=0;str[strucdat.length+2+i] != 0;i++)
	{
		str1[i] = str[strucdat.length+2+i];
	}

	for(i=0;i<strucdat.length;i++)
	{
		str2[i] = str[i];
	}
	
	for(j=0;array[j]!=0;j++)
	{
		str2[i] = array[j];
		i++;
	}
	for(j=0;str1[j]!=0;j++)
	{
		str2[i] = str1[j];
		i++;
	}
	str2[i] = 0;
	UARTSendStr(str2);
}