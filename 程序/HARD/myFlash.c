#include "myFlash.h"
#include "List4_Malloc.H"
/***
 * �ڴ濽��
 * ����pd���������һ��ָ��
 */
u8*  mymemcpy(void* pdat ,const void* psor,u32 len)
{
	u32 i = 0;
	u32 temp = len/sizeof(u32);
	u8* pd = (u8*)pdat;
	u8* ps = (u8*)psor;
	for(i=0;i<temp;i++) ((u32*)pd)[i] = ((u32*)ps)[i];
	i *= sizeof(u32);
	for(;i<len;i++) pd[i] = ps[i];
	return &pd[len];
}

/**
*   ��ĳ����ַ�������ݣ������ֵ�src_addrΪ�׵ĵ�ַ��
*   des_addrΪ��Ե�ַ����0~FLASH_MAX_SIZE��
*/ 
errorFlash   Flash_Read(u16 des_addr , u8* src_addr,u16 size)
{
    u8 erro = ERRO_FLASH_OK;
	u16 startSector = 0;
	u16 startoffset = 0;
	u16 globelBody = 0;
	u16 endOffset = 0;
	u8* datbuf;

    if((des_addr + size) > FLASH_MAX_SIZE) return ERRO_FLASH_OUTRANGE;
	
    startSector = FLASH_ADDR_BEGIN + (des_addr/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;    // ������ʼ������ַ
    startoffset = des_addr&(FLASH_SECTOR_SIZE - 1);                                     // ���������е�ƫ����

    if((size + startoffset) > FLASH_SECTOR_SIZE)
    	globelBody = (size + startoffset - FLASH_SECTOR_SIZE)/FLASH_SECTOR_SIZE;             // ������������������
     endOffset = (size + startoffset)&(FLASH_SECTOR_SIZE - 1);                           // ����ĩβ�ֽ�ƫ��

    /* ������ */
	datbuf = malloc(FLASH_SECTOR_SIZE);
	if(datbuf == 0) return ERRO_MALLOC_ZERO;
     FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);    // ������
    if((startoffset + size) > FLASH_SECTOR_SIZE)                // ����һ������
    {
        MemCopy(src_addr,datbuf + startoffset,FLASH_SECTOR_SIZE - startoffset);
        src_addr += FLASH_SECTOR_SIZE - startoffset;
      //  printf("read : globelBody = %d\r\n",globelBody);
        while(globelBody --)
        {
            startSector += FLASH_SECTOR_SIZE;
            FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);    // ������
            MemCopy((u8*)src_addr,(u8*)datbuf,FLASH_SECTOR_SIZE); 
            src_addr += FLASH_SECTOR_SIZE;
        }
        startSector += FLASH_SECTOR_SIZE;
        FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);    // ������
        MemCopy((u8*)src_addr,(u8*)datbuf,endOffset); 
    }else
    {
        MemCopy(src_addr,datbuf + startoffset,size);
    }
    free(datbuf);
    return erro;
}

/**
 *  ��src_addr��ַ�����ݣ�д��des_addr
*   des_addrΪFLASH��Ե�ַ����0~FLASH_MAX_SIZE��
*/ 
errorFlash   Flash_Write(u16 des_addr , u8* src_addr,u16 size)
{
    u8 erro = ERRO_FLASH_OK;
	u16 startSector = 0;
	u16 startoffset = 0;
	u16 globelBody = 0;
	u16 endOffset = 0;
	u8* datbuf;
    if((des_addr + size) > FLASH_MAX_SIZE) return ERRO_FLASH_OUTRANGE;
     startSector = FLASH_ADDR_BEGIN + (des_addr/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;    	// ������ʼ������ַ
     startoffset = des_addr&(FLASH_SECTOR_SIZE - 1);                                     	// ���������е�ƫ����
    
    if((size + startoffset) > FLASH_SECTOR_SIZE)
    	globelBody = (size + startoffset - FLASH_SECTOR_SIZE)/FLASH_SECTOR_SIZE;             // ������������������
     endOffset = (size + startoffset)&(FLASH_SECTOR_SIZE - 1);                           	// ����ĩβ�ֽ�ƫ��
	
	
    /* д���� */
    datbuf = malloc_zero(FLASH_SECTOR_SIZE);
	if(datbuf == 0) return ERRO_MALLOC_ZERO;
    FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);                // ������
    FLASH_ERASE(startSector);                                                                   // ��������

    if((startoffset + size) > FLASH_SECTOR_SIZE)                                                // ����һ������
    {
        MemCopy((u8*)datbuf + startoffset,(u8*)src_addr ,FLASH_SECTOR_SIZE - startoffset);      // װ������ 
        erro |= FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);           // д������
        src_addr += FLASH_SECTOR_SIZE - startoffset;
     //   printf("write : globelBody = %d\r\n",globelBody);
        while(globelBody --)
        {
            startSector += FLASH_SECTOR_SIZE;
            FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);          // ������
            FLASH_ERASE(startSector);                                                            // ��������
            MemCopy((u8*)datbuf,(u8*)src_addr,FLASH_SECTOR_SIZE);                                // װ������  
            erro |= FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);        // д������
            src_addr += FLASH_SECTOR_SIZE;
        }
        startSector += FLASH_SECTOR_SIZE;
        FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);             // ������
        FLASH_ERASE(startSector);                                                               // ��������
        MemCopy((u8*)datbuf,(u8*)src_addr,endOffset);                                           // װ������  
        erro |= FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);           // д������

    }
    else
    {                                                               // ��������
        MemCopy((u8*)datbuf + startoffset,(u8*)src_addr ,size);                                  // װ������
		FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);            // д������
//		printf("startSector = %d\r\n",startSector);

    }
    free(datbuf);
    return erro;

}
