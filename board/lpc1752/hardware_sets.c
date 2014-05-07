#include "hardware_sets.h"
#include "LPC17xx.h"
#include "uart0.h"
#include "led.h"
#include "w25q256.h"
#include "stdlib.h"
#include "string.h"


void hardware_init(void)
{
    SystemClockUpdate();
    SysTickInit();
    uart0_init(UART0_BPS);
    SPI0_init();
    led_init();
    lcd_printf("===welcome to black box develop====\n");
    lcd_printf("init done!\n");

}
uint8 between_val(uint8 num,uint8 leftval,uint8 rightval)
{
    if((num >leftval) &&(num<rightval))
        return 1;
    else
        return 0;
}
void delay1ms(void)
{
    uint16 i,j;
    for(i=0;i<500;i++)
        for(j=0;j<200;j++);
}

void led_shrinking(void)
{
#if 1//led灯闪亮
    static uint8 num=0;
    static uint8 flag=0;
    if(between_val(num,0,128))
    {
        Led_USB_UP_On();
        if(flag){
            flag=0;
            lcd_printf("running...\n");
        }

    }else{
        flag=1;
        Led_USB_UP_Off();
    }
    delay1ms();
    num++;
#elif 0 //cs方波测试

    //cs配置成gpio，管脚可以正常拉低拉高，但SSEL0不可以正常拉低
    static uint8 num=0;
    static uint8 flag=0;
    if(between_val(num,0,128))
    {
        Led_USB_UP_On();
        SPI_CS_HIGH();
        if(flag){
            flag=0;
            //            lcd_printf("system is running...\n");
        }

    }else{
        SPI_CS_LOW();
        flag=1;
        Led_USB_UP_Off();
    }
    //    delay1ms();
    num++;
#else//下拉电平
    //    SPI_CS_LOW();
    SPI_CS_HIGH();
#endif
}
void printbuf(uint8 *buf,uint16 len)
{
    uint16 i;
    for(i=0;i<len;i++)
    {
        lcd_printf("buf[%d]:0x%x\n",i,*buf++);
    }
}
#define DELAYDEBUG Delay1Ms(1);

static void get_uniqueID_w25q256(void)
{
    //     uint32 deviceid=0;
    uint8 buf[3];

    lcd_printf("/************read device id test ***********/\n");

    // w25qxx_write_enable();
    //     lcd_printf("\n\n===>>read jedec ID\n");
    w25qxx_read_id(buf);
    printbuf(buf,3);



    //     DELAYDEBUG;
    //     lcd_printf("\n\n===>>get_sector_num\n");
    //     deviceid = w25qxx_get_sector_num();

    //     DELAYDEBUG;
    //     lcd_printf("\n\n===>>get_sector_num\n");
    //     deviceid = w25qxx_get_sector_num();

    lcd_printf("status_reg1:0x%x\n",w25qxx_read_status_reg1());
    lcd_printf("status_reg2:0x%x\n",w25qxx_read_status_reg2());
    lcd_printf("status_reg3:0x%x\n",w25qxx_read_status_reg3());

}
void flash_read_test(void)
{
    uint8 buf[256];
    lcd_printf("flash read test...\n");
    w25qxx_sector_read(1,buf,1);
    printbuf(buf,sizeof(buf));
}
void flash_write_test(void)
{
    uint8 buf[256];
    lcd_printf("flash_write_test\n");
    memset(buf,1,sizeof(buf));
    w25qxx_sector_erase(1);
    w25qxx_sector_write(1,buf,1);
    lcd_printf("write done...\n");
}





void flash_total_test(void)
{
    //     Delay1Ms(10000);
    
    


    get_uniqueID_w25q256();
//    flash_read_test();
//    flash_write_test();


    //    test_chip_erase();

//     test_erase();
}

