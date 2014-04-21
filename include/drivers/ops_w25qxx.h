#ifndef _OPS_W25QXX__H
#define _OPS_W25QXX__H

//2014/03/26 15:03:45
/*****************************************************************************
*author:
*time  :
*****************************************************************************/
//初始化操作变量
void Init_OpsFlash(void);
void Clr_OpsFlash(void);
void set_bytemode(uint8 bytemode);
uint8 get4BytesMode(void);
//it's the same for 3&4 bytes mode
void w25qxx_read_id(uint8 *buffer);
//待进一步验证
void w25qxx_read_unique_id(uint8 *buffer);
void w25qxx_sector_write(uint32 sector, uint8 * dstbuf, uint8 sector_number);
void w25qxx_sector_read(uint32 sector, uint8 * buffer, uint8 sector_number);
void w25qxx_sector_erase(uint32 sector);

#endif
