/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2009-05-12
** Last Version:        V1.01
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Chengmingji
** Created date:        2009-07-24
** Version:             V1.00
** Descriptions:        添加用户应用程序
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         LiuWeiyun
** Modified date:       2009-07-27
** Version:             V1.01
** Descriptions:        编写usbDevSimple例程
**
** Rechecked by:        Zhang Ningbo
*********************************************************************************************************/
#include <string.h>
#include "..\usbDevLPC1700\usbDevConfig.h"
#include "..\config.h"

/*********************************************************************************************************
  变量与宏定义
*********************************************************************************************************/
#define     BEEP           (1ul << 7)                                   /*  蜂鸣器P0.7                  */
#define     KEY1           (1ul << 10)                                  /*  P2.10连接KEY1               */
#define     BEEP_INIT()     FIO0DIR |= BEEP                             /*  蜂鸣器引脚输出              */
#define     BEEP_OFF()      FIO0SET = BEEP                              /*  蜂鸣器关                    */
#define     BEEP_ON()       FIO0CLR = BEEP                              /*  蜂鸣器开                    */
/*********************************************************************************************************
** Function name:       usbDeviceDemo
** Descriptions:        简单的数据传输
** Input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void  usbDeviceDemo (void)
{

    zyIsrSet(NVIC_USB, (unsigned long)usbDevException, PRIO_EIGHT);     /* 设置外部中断优先级并使能     */

    if (0 != usbDevInit()) {                                            /* 初始化USB控制器              */
        while(1);                                                       /* 初始化失败，程序停止执行     */
    }

    while(1) {

        usbDevServe();                                                  /* 处理USB事件                  */

        if (bEPPflags.bits.configuration == 1) {                        /* USB Device配置完成           */

            if (bEPPflags.bits.ep1_rxdone == 1) {                       /* 逻辑端点1收到数据            */
                zyIrqDisable();
                bEPPflags.bits.ep1_rxdone = 0;                          /* 清除端点1收到数据标志        */
                usbDevWriteEndpoint(3, GenEpBuf, 10);                   /* 发送10个字节到PC             */
                zyIrqEnable();
            }
        }
    }
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        PC机通过逻辑端点1发送10个字节到下位机，下位机接到这是个字节后再通过端点1返回到PC
**                      实验前需用跳线帽短接以下端口.
**                      JP2(VBUS和P1.30,U1CON和P2.9,DEV_D-和P0.30,DEV_D+和P0.29,USB DEV和P1.18)
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int  main (void)
{
    targetInit();                                                       /* 初始化目标板，切勿删除       */
    usbDeviceDemo();
    while (1) {

    }
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
