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
#include "usbDevConfig.h"
#include "config.h"



void USB_Init(void)
{

}
/*********************************************************************************************************
** Function name:       usbDeviceDemo
** Descriptions:        简单的数据传输
** Input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void  usbDeviceDemo (void)
{
//     Debug("usb init!!!\n");
//     zyIsrSet(NVIC_USB, (unsigned long)usbDevException, PRIO_EIGHT);     /* 设置外部中断优先级并使能     */

//     if (0 != usbDevInit()) {                                            /* 初始化USB控制器              */
//         while(1);                                                       /* 初始化失败，程序停止执行     */
//     }

//     while(1) {

//         usbDevServe();                                                  /* 处理USB事件                  */

//         if (bEPPflags.bits.configuration == 1) {                        /* USB Device配置完成           */

//             if (bEPPflags.bits.ep1_rxdone == 1) {                       /* 逻辑端点1收到数据            */
//                 zyIrqDisable();
//                 bEPPflags.bits.ep1_rxdone = 0;                          /* 清除端点1收到数据标志        */
//                 usbDevWriteEndpoint(3, GenEpBuf, 10);                   /* 发送10个字节到PC             */
//                 zyIrqEnable();
//             }
//         }
//     }
}



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
//    USB_Init();
    timer_all_init(0,Get_Timer_1s());

    Debug("\n~~~~~~~~~~~~~~~~~~~~~~\n");
    Debug("~~~~~~~~black box~~~~~\n");
    Debug("~~~~~~~~~~~~~~~~~~~~~~\n");
    Debug("init done!\n");
    //操作类变量初始化
    ops_init();

}



