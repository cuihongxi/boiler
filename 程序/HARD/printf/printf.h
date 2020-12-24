#ifndef __PRINTF__H
#define __PRINTF__H

#include "uhead.h"
#include "uart.h"

typedef struct 
{
	unsigned char length;
	unsigned char type;
}structLenTyp;

void printf(const char*str,unsigned long dat);




#endif