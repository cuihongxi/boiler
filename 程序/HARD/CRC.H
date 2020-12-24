/*
2020年11月15日08:38:40
CRC校验函数

*/
#ifndef	__CRC_H
#define	__CRC_H

#include "uhead.h"



//unsigned int CRC16(uchar *puchMsg, uint usDataLen);
unsigned int CRC16_2(uchar *puchMsg, uint usDataLen,uint preCRCVal);


#endif
