#include "myFlash.h"
#include "List4_Malloc.H"
/***
 * 内存拷贝
 * 返回pd拷贝完的下一个指针
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
*   从某个地址读出数据，并保持到src_addr为首的地址中
*   des_addr为相对地址：（0~FLASH_MAX_SIZE）
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
	
    startSector = FLASH_ADDR_BEGIN + (des_addr/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;    // 计算起始扇区地址
    startoffset = des_addr&(FLASH_SECTOR_SIZE - 1);                                     // 计算扇区中的偏移量

    if((size + startoffset) > FLASH_SECTOR_SIZE)
    	globelBody = (size + startoffset - FLASH_SECTOR_SIZE)/FLASH_SECTOR_SIZE;             // 计算完整的扇区个数
     endOffset = (size + startoffset)&(FLASH_SECTOR_SIZE - 1);                           // 计算末尾字节偏移

    /* 读数据 */
	datbuf = malloc(FLASH_SECTOR_SIZE);
	if(datbuf == 0) return ERRO_MALLOC_ZERO;
     FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);    // 读扇区
    if((startoffset + size) > FLASH_SECTOR_SIZE)                // 超出一个扇区
    {
        MemCopy(src_addr,datbuf + startoffset,FLASH_SECTOR_SIZE - startoffset);
        src_addr += FLASH_SECTOR_SIZE - startoffset;
      //  printf("read : globelBody = %d\r\n",globelBody);
        while(globelBody --)
        {
            startSector += FLASH_SECTOR_SIZE;
            FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);    // 读扇区
            MemCopy((u8*)src_addr,(u8*)datbuf,FLASH_SECTOR_SIZE); 
            src_addr += FLASH_SECTOR_SIZE;
        }
        startSector += FLASH_SECTOR_SIZE;
        FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);    // 读扇区
        MemCopy((u8*)src_addr,(u8*)datbuf,endOffset); 
    }else
    {
        MemCopy(src_addr,datbuf + startoffset,size);
    }
    free(datbuf);
    return erro;
}

/**
 *  从src_addr地址读数据，写入des_addr
*   des_addr为FLASH相对地址：（0~FLASH_MAX_SIZE）
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
     startSector = FLASH_ADDR_BEGIN + (des_addr/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;    	// 计算起始扇区地址
     startoffset = des_addr&(FLASH_SECTOR_SIZE - 1);                                     	// 计算扇区中的偏移量
    
    if((size + startoffset) > FLASH_SECTOR_SIZE)
    	globelBody = (size + startoffset - FLASH_SECTOR_SIZE)/FLASH_SECTOR_SIZE;             // 计算完整的扇区个数
     endOffset = (size + startoffset)&(FLASH_SECTOR_SIZE - 1);                           	// 计算末尾字节偏移
	
	
    /* 写数据 */
    datbuf = malloc_zero(FLASH_SECTOR_SIZE);
	if(datbuf == 0) return ERRO_MALLOC_ZERO;
    FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);                // 读扇区
    FLASH_ERASE(startSector);                                                                   // 擦除扇区

    if((startoffset + size) > FLASH_SECTOR_SIZE)                                                // 超出一个扇区
    {
        MemCopy((u8*)datbuf + startoffset,(u8*)src_addr ,FLASH_SECTOR_SIZE - startoffset);      // 装载数据 
        erro |= FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);           // 写入数据
        src_addr += FLASH_SECTOR_SIZE - startoffset;
     //   printf("write : globelBody = %d\r\n",globelBody);
        while(globelBody --)
        {
            startSector += FLASH_SECTOR_SIZE;
            FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);          // 读扇区
            FLASH_ERASE(startSector);                                                            // 擦除扇区
            MemCopy((u8*)datbuf,(u8*)src_addr,FLASH_SECTOR_SIZE);                                // 装载数据  
            erro |= FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);        // 写入数据
            src_addr += FLASH_SECTOR_SIZE;
        }
        startSector += FLASH_SECTOR_SIZE;
        FLASH_READ_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);             // 读扇区
        FLASH_ERASE(startSector);                                                               // 擦除扇区
        MemCopy((u8*)datbuf,(u8*)src_addr,endOffset);                                           // 装载数据  
        erro |= FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);           // 写入数据

    }
    else
    {                                                               // 擦除扇区
        MemCopy((u8*)datbuf + startoffset,(u8*)src_addr ,size);                                  // 装载数据
		FLASH_WRITE_SECTORS(startSector,datbuf,FLASH_SECTOR_SIZE);            // 写入数据
//		printf("startSector = %d\r\n",startSector);

    }
    free(datbuf);
    return erro;

}
