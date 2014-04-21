#include "test_hw.h"

#include "includeall.h"
#include "ops_w25qxx.h"
#include "w25q256.h"

static uint8 buf[256];
uint32 deviceid=0;




void test_w25qxx_uniqueID(void)
{
    w25qxx_read_unique_id(buf);
    printbuf(buf,3);
//    Debug();
}


static void get_uniqueID_w25q256(void)
{
//    get_all_status_reg();
    Debug("\n\n===>>get_sector_num\n");
    deviceid = w25qxx_get_sector_num();
    w25qxx_read_id(buf);
    printbuf(buf,3);


}

void flash_read_test(void)
{

    Debug("~~~flash read test\n");
    w25qxx_sector_read(1,buf,1);
    printbuf(buf,sizeof(buf));
}
#define ERASENUMS (2000)


void flash_write_test(uint32 id)
{
    const uint32 addr=0x30000;
    uint8 data=0;
    memset(buf,id,sizeof(buf));
#if 1
//    void w25qxx_write_1byte_4(uint32 addr, uint8 data);
//    uint8 w25qxx_read_1byte_4(uint32 addr);
//    void w25qxx_section_erase_4(uint32 addr);



    w25qxx_section_erase_4(addr);
    w25qxx_write_1byte_4(addr,id%256);
    data = w25qxx_read_1byte_4(addr);
    Debug("data:%d\n",data);

#else
    //error,why?大批量的写数据存在问题
    w25qxx_sector_erase(ERASENUMS);
//    w25qxx_sector_write(ERASENUMS,buf,1);
    w25qxx_sector_read(ERASENUMS,buf,1);
#endif
    Debug("\n");
//    printbuf(buf,3);
}
void test_write_statusreg(void)
{
//    get_all_status_reg();

    w25qxx_write_status_reg1(0x7C);
//    w25qxx_write_status_reg2();
//    w25qxx_write_status_reg3();

//    get_all_status_reg();
}




void flash_total_test(void)
{
    uint32 id;
    const uint32 nums=20;
    for(id=0;id<nums;id++)
        flash_write_test(id);

//    flash_read_test();
}



void testall(void)
{
//    test_w25qxx_uniqueID();
//    get_uniqueID_w25q256();
#if 0
    w25qxx_read_id(buf);
    printbuf(buf,3);
#else
    flash_total_test();

#endif

//    test_write_statusreg();

}
