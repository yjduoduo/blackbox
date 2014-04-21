#include "includeall.h"
#include "ops_w25qxx.h"
#include "w25q256.h"



//must be 0|1 as 1bit
#define OPSMODE_3BYTE 0
#define OPSMODE_4BYTE 1
typedef union OpsFlash{
    uint8 Word;
    struct{
        uint8 bytem:1;//3bytes |  4bytes

    }ByteMode;
}OpsFlash;
OpsFlash vOpsFlash;


//初始化操作变量
void Init_OpsFlash(void)
{
    Clr_OpsFlash();
    set_bytemode(OPSMODE_4BYTE);

}
void Clr_OpsFlash(void)
{
    vOpsFlash.Word = 0;
}

void set_bytemode(uint8 bytemode)
{
    vOpsFlash.ByteMode.bytem = bytemode;
}

uint8 get4BytesMode(void)
{
    if(OPSMODE_4BYTE == vOpsFlash.ByteMode.bytem)
        return TRUE;
    else
        return FALSE;
}



//it's the same for 3&4 bytes mode
void w25qxx_read_id(uint8 *buffer)
{
    if(get4BytesMode())
    {
        w25qxx_read_id_4(buffer);
    }else{
        w25qxx_read_id_3(buffer);
    }
}
//待进一步验证
void w25qxx_read_unique_id(uint8 *buffer)
{
    if(get4BytesMode())
    {
        w25qxx_read_unique_id_4(buffer);
    }else{
        w25qxx_read_unique_id_3(buffer);
    }
}


void w25qxx_sector_write(uint32 sector, uint8 * dstbuf, uint8 sector_number)
{
    if(get4BytesMode())
    {
        w25qxx_sector_write_4(sector,dstbuf,sector_number);
    }else{
        w25qxx_sector_write_3(sector,dstbuf,sector_number);
    }
}
void w25qxx_sector_read(uint32 sector, uint8 * buffer, uint8 sector_number)
{
    if(get4BytesMode())
    {
        w25qxx_sector_read_4(sector,buffer,sector_number);
    }else{
        w25qxx_sector_read_3(sector,buffer,sector_number);
    }
}
void w25qxx_sector_erase(uint32 sector)
{
    if(get4BytesMode())
    {
//        Debug("~~4-bytes mode\n");
        w25qxx_sector_erase_4(sector);
    }else{
        w25qxx_sector_erase_3(sector);
    }
}

