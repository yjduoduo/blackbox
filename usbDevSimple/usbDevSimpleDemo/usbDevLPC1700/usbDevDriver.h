/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDriver.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC2400 固件程序应用层头文件
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            修改部分代码以适合SmartARM2400系列开发板
*********************************************************************************************************/

#ifndef __USBDRIVER_H
#define __USBDRIVER_H

extern UINT8 GenEpBuf[];                                                /*  buffer of logic endpoint 1  */
extern UINT8 EpBuf[];                                                   /*  buffer of logic endpoint 2  */


/*********************************************************************************************************
  USB 事件标志结构体
*********************************************************************************************************/

struct __stflags {

    unsigned bus_reset              : 1;    //总线复位
    unsigned suspend                : 1;    //总线挂起改变
    unsigned con_change             : 1;    //总线连接改变
    unsigned setup_packet           : 1;    //接收到SETUP包
    unsigned remote_wakeup          : 1;    //远程唤醒标志
    unsigned in_isr                 : 1;    //进入USB设备中断服务程序标志
    unsigned control_state          : 2;    //控制端点处理状态
                                                //0: 空闲状态
                                                //1: 数据发送状态
                                                //2: 数据接收状态
    unsigned configuration          : 1;    //USB设备配置完成标志(0:未配置;1:已配置)
    unsigned command                : 1;    //未使用
    unsigned ep1_rxdone             : 1;    //逻辑端点1接收到数据标志
    unsigned ep2_rxdone             : 1;    //逻辑端点2接收到数据标志
    unsigned ep1_rxdma              : 1;    //逻辑端点1(DMA)接收到数据标志
    unsigned ep2_rxdma              : 1;    //逻辑端点2(DMA)接收到数据标志
};

typedef struct __stflags    __FLAGS;                                    /*  重定义标志位结构体类型      */

union __unflags {

    __FLAGS bits;
    UINT16 value;
};

typedef union __unflags EPPFLAGS;                                       /*  重定义标志位共用体类型      */

extern EPPFLAGS bEPPflags;                                              /*  声明共用体标志位变量        */

/*********************************************************************************************************
  下面为向用户开放的 API 函数
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       usbDevInit
** Descriptions:        初始化 USB 设备控制器
** input parameters:    无
** output parameters:   无
** Returned value:      初始化成功  0
**                      初始化失败  错误码
*********************************************************************************************************/
extern UINT8 usbDevInit (void);

/*********************************************************************************************************
** Function name:       usbDevServe
** Descriptions:        USB 设备控制器事件处理函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usbDevServe (void);

/*********************************************************************************************************
** Function name:       usbDevException
** Descriptions:        USB 中断服务程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usbDevException (void);

/*********************************************************************************************************
** Function name:       usbDevGetVer
** Descriptions:        得到 USB Device 软件包版本号
** input parameters:    无
** output parameters:   无
** Returned value:      ZLG/USB23xx 软件包版本号
*********************************************************************************************************/
extern UINT16 usbDevGetVer(void);

#endif                                                                  /*  __USBDRIVER_H               */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
