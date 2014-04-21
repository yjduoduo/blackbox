#include "hardware_sets.h"
#include "LPC17xx.h"
#include "uart0.h"
#include "led.h"
#include "w25q256.h"
#include "stdlib.h"
#include "string.h"
#include "CTimeDef.h"
#include "timer_all.h"
#include "ops_w25qxx.h"

void ops_init(void)
{
    Init_OpsFlash();


}

void hardware_init(void)
{
    SystemClockUpdate();
    uart0_init(UART0_BPS);
    spi_init(0);
    led_init();
//     USB_Init();
    timer_all_init(0,Get_Timer_1s());

    Debug("\n~~~~~~~~~~~~~~~~~~~~~~\n");
    Debug("~~~~~~~~black box~~~~~\n");
    Debug("~~~~~~~~~~~~~~~~~~~~~~\n");
    Debug("init done!\n");
    //操作类变量初始化
    ops_init();

}



