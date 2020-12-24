/**
 * 2019��10��11��14:35:10
 * FLASH ����
 * ע�⣺����4�ֽ�Ϊ��λ���ж�д
 */

#ifndef __MYFLASH_H_
#define __MYFLASH_H_

#include "UHEAD.H"
#include "eeprom.h"

typedef unsigned char errorFlash;

#define		FLASH_MAX_SIZE			0XFFF        		// �ֽڣ�4K
#define     FLASH_SECTOR_SIZE   	512          		// һ������
#define     FLASH_ADDR_BEGIN    	0         			// ��ʼ��ַ


/*��ֲ����*/
#define	    FLASH_ERASE(a)  	                            EEPROM_SectorErase(a)   // FLASH��������
#define     FLASH_WRITE_SECTORS(des_addr,src_addr,length)   EEPROM_write_n(des_addr,src_addr,length)  // FLASHд����
#define     FLASH_READ_SECTORS(des_addr,src_addr,length)    EEPROM_read_n(des_addr,src_addr,length)   // FLASH������
#define     MemCopy                       					mymemcpy		// �ڴ濽��


/*���󱨾�*/
#define     ERRO_FLASH_OK             0    // ok
#define     ERRO_FLASH_RESULT_ERR     1    // RESULT_ERR
#define     ERRO_FLASH_TIMEOUT        2    // TIMEOUT
#define     ERRO_FLASH_OUTRANGE       0x10 // ����FLASH����
#define		ERRO_MALLOC_ZERO		3	// ��̬�����ڴ�ʧ��


/* ���� */

errorFlash Flash_Read(u16 des_addr , u8* src_addr,u16 size);
errorFlash Flash_Write(u16 des_addr , u8* src_addr,u16 size);
u8*  mymemcpy(void* pdat ,const void* psor,u32 len);

#endif

