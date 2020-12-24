/**
 * 2019年10月11日14:35:10
 * FLASH 函数
 * 注意：必须4字节为单位进行读写
 */

#ifndef __MYFLASH_H_
#define __MYFLASH_H_

#include "UHEAD.H"
#include "eeprom.h"

typedef unsigned char errorFlash;

#define		FLASH_MAX_SIZE			0XFFF        		// 字节，4K
#define     FLASH_SECTOR_SIZE   	512          		// 一个扇区
#define     FLASH_ADDR_BEGIN    	0         			// 起始地址


/*移植定义*/
#define	    FLASH_ERASE(a)  	                            EEPROM_SectorErase(a)   // FLASH擦除扇区
#define     FLASH_WRITE_SECTORS(des_addr,src_addr,length)   EEPROM_write_n(des_addr,src_addr,length)  // FLASH写扇区
#define     FLASH_READ_SECTORS(des_addr,src_addr,length)    EEPROM_read_n(des_addr,src_addr,length)   // FLASH读扇区
#define     MemCopy                       					mymemcpy		// 内存拷贝


/*错误报警*/
#define     ERRO_FLASH_OK             0    // ok
#define     ERRO_FLASH_RESULT_ERR     1    // RESULT_ERR
#define     ERRO_FLASH_TIMEOUT        2    // TIMEOUT
#define     ERRO_FLASH_OUTRANGE       0x10 // 超出FLASH上限
#define		ERRO_MALLOC_ZERO		3	// 动态申请内存失败


/* 函数 */

errorFlash Flash_Read(u16 des_addr , u8* src_addr,u16 size);
errorFlash Flash_Write(u16 des_addr , u8* src_addr,u16 size);
u8*  mymemcpy(void* pdat ,const void* psor,u32 len);

#endif

