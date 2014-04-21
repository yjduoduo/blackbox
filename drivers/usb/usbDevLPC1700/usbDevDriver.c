/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDriver.c
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 固件程序应用层源文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            修改部分代码以适合SmartARM3250系列开发板
**--------------------------------------------------------------------------------------------------------
** Modified by:             LiuWeiyun
** Modified date:           2009-01-05
** Version:                 1.2
** Descriptions:            LPC3200移植
*********************************************************************************************************/
#include "usbDevConfig.h"

UINT8 GenEpBuf[EP1_PACKET_SIZE];                                        /*  逻辑端点 1 接收缓冲区       */
UINT8 EpBuf[EP2_PACKET_SIZE];                                           /*  逻辑端点 2 接收缓冲区       */

EPPFLAGS bEPPflags;                                                     /*  定义 USB 事件标志变量       */

/*********************************************************************************************************
** Function name:       __usbDevConfigEndpoint
** Descriptions:        配置端点的最大包大小
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevConfigEndpoint (void)
{
    __usbDevConfigMaxPaketSize(0,EP0_PACKET_SIZE);                      /*  配置各物理端点的最大包大小  */
    __usbDevConfigMaxPaketSize(1,EP0_PACKET_SIZE);

    __usbDevConfigMaxPaketSize(2,EP1_PACKET_SIZE);
    __usbDevConfigMaxPaketSize(3,EP1_PACKET_SIZE);

    __usbDevConfigMaxPaketSize(4,EP2_PACKET_SIZE);
    __usbDevConfigMaxPaketSize(5,EP2_PACKET_SIZE);
}
/*********************************************************************************************************
** Function name:       __usbDevEp1TxDone
** Descriptions:        逻辑端点1发送数据处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevEp1TxDone (void)
{
    __usbDevSelectClrIntEndpoint(3);                                    /*  选择端点并清除中断          */
}
/*********************************************************************************************************
** Function name:       __usbDevEp1RxDone
** Descriptions:        逻辑端点1接收数据处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevEp1RxDone (void)
{
    UINT8 len;

    __usbDevSelectClrIntEndpoint(2);                                    /*  选择端点并清除中断          */

    len = usbDevReadEndpoint(2,GenEpBuf,sizeof(GenEpBuf));              /*  从接收缓冲区中读出数据      */

    if(len != 0) {
        bEPPflags.bits.ep1_rxdone = 1;                                  /*  标识该端点收到数据          */
    }
}
/*********************************************************************************************************
** Function name:       __usbDevEp2TxDone
** Descriptions:        逻辑端点2发送数据处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevEp2TxDone (void)
{
    __usbDevSelectClrIntEndpoint(5);                                    /*  选择端点并清除中断          */

}
/*********************************************************************************************************
** Function name:       __usbDevEp2RxDone
** Descriptions:        逻辑端点2接收数据处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevEp2RxDone (void)
{
    UINT8 len;

    __usbDevSelectClrIntEndpoint(4);                                    /*  选择端点并清除中断          */

    len = usbDevReadEndpoint(4,GenEpBuf,sizeof(GenEpBuf));
    if(len != 0)
        bEPPflags.bits.ep2_rxdone = 1;                                  /* 标识该端点收到数据           */
}
/*********************************************************************************************************
** Function name:       __usbDevEpIntHandler
** Descriptions:        USB 端点中断处理程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevEpIntHandler (void)
{
    UINT32 epSt;

    epSt = (UINT32)__EPIntSt;                                           /*  读端点中断状态寄存器        */

    if(epSt & USB_ENDP00) {
        __usbDevEp0RxDone();                                            /*  控制端点接收数据处理        */
    }
    if(epSt & USB_ENDP01) {
        __usbDevEp0TxDone();                                            /*  控制端点发送数据处理        */
    }
    if(epSt & USB_ENDP02) {
        __usbDevEp1RxDone();                                            /*  逻辑端点1接收数据处理       */
    }
    if(epSt & USB_ENDP03) {
        __usbDevEp1TxDone();                                            /*  逻辑端点1发送数据处理       */
    }
    if(epSt & USB_ENDP04) {
        __usbDevEp2RxDone();                                            /*  逻辑端点2接收数据处理       */
    }
    if(epSt & USB_ENDP05) {
        __usbDevEp2TxDone();                                            /*  逻辑端点2发送数据处理       */
    }
}
/*********************************************************************************************************
** Function name:       __usbDevBusReset
** Descriptions:        USB 总线复位处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevBusReset (void)
{
    __usbDevIntConfig();
    __usbDevConfigEndpoint();                                           /*  重新配置所有端点最大包大小  */
}

/*********************************************************************************************************
** Function name:       __usbDevSuspendChange
** Descriptions:        USB 总线挂起改变
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSuspendChange (void)
{

}

/*********************************************************************************************************
** Function name:       _usbConnectChange
** Descriptions:        USB 总线连接改变
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevConnectChange (void)
{

}

/*********************************************************************************************************
** Function name:       usbDevInit
** Descriptions:        初始化 USB 设备控制器
** input parameters:    无
** output parameters:   无
** Returned value:      初始化成功  0
**                      初始化失败  错误码
*********************************************************************************************************/
UINT8 usbDevInit (void)
{
    __usbDevHardwareInit();                                             /*  初始化硬件                  */

    bEPPflags.value = 0;                                                /*  置USB事件标志为0            */

    if (__usbDevReadTestRegister() != 0xA50F) {                         /*  读测试寄存器                */
        return 1;                                                       /*  初始化失败                  */
    }

    __usbDevIntConfig();                                                /*  配置中断                    */
    __usbDevConfigEndpoint();                                           /*  配置端点                    */

#if DMA_ENGINE_EN > 0
    usbDevDMAInit();                                                    /*  初始化USB DMA               */
    __usbDevSetMode(0x28);
#else
    __usbDevSetMode(0x00);                                              /*  成功传输才产生中断          */
#endif

    //__usbDevSetAddressEnable(0, 1);

    __usbDevHadwareReconnect();
    __usbDevSoftReconnect();                                            /*  重新连接USB                 */

    return (0);                                                         /*  初始化USB成功               */
}

/*********************************************************************************************************
** Function name:       usbDevServe
** Descriptions:        USB 设备控制器事件处理函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void usbDevServe (void)
{
    if (bEPPflags.bits.setup_packet) {
                                                                        /*  收到 SETUP 包               */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.setup_packet = 0;
        __usbDevControlHandler();                                       /*  控制传输处理                */
        USB_EXIT_CRITICAL_SECTION();
    }

    if (bEPPflags.bits.bus_reset) {
                                                                        /*  USB总线复位处理             */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.bus_reset = 0;
        USB_EXIT_CRITICAL_SECTION();
    }

    if (bEPPflags.bits.suspend) {
                                                                        /*  USB总线挂起改变处理         */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.suspend= 0;
        USB_EXIT_CRITICAL_SECTION();
        __usbDevSuspendChange();
    }

    if (bEPPflags.bits.con_change) {
                                                                        /*  USB总线连接改变处理         */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.con_change = 0;
        USB_EXIT_CRITICAL_SECTION();
        __usbDevConnectChange();
    }
}

/*********************************************************************************************************
** Function name:       usbDevException
** Descriptions:        USB 中断服务程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void  usbDevException (void)
{
    UINT32 ulDevIntState = 0;
    UINT32 ulDevState    = 0;

    bEPPflags.bits.in_isr = 1;                                          /*  标识程序进入中断            */

    ulDevIntState = (UINT32)__DevIntSt;                                 /*  读设备中断状态寄存器        */

    if (ulDevIntState & DEVINT_STATUS_DEVSTAT) {

        ulDevState = __usbDevGetDevStatus();

        if ((ulDevState & 0x10) != 0) {
            __usbDevBusReset();                                         /*  总线复位处理                */
            bEPPflags.bits.bus_reset = 1;                               /*  标识总线复位                */
        }

        if ((ulDevState & 0x08) != 0) {
            bEPPflags.bits.suspend = 1;                                 /*  标识总线挂起改变            */
        }

        if ((ulDevState & 0x02) != 0) {
            bEPPflags.bits.con_change = 1;                              /*  标识总线连接改变            */
        }

        __DevIntClr = DevStatusInterrupt;
        goto USB_ISR_EXIT;                                              /*  退出中断服务程序           */
    }

    if (ulDevIntState & DEVINT_STATUS_EP_FAST) {
        __usbDevEpIntHandler();
        __DevIntClr = FASTINTERRUPT;
    }

    if (ulDevIntState & DEVINT_STATUS_EP_SLOW) {
        __usbDevEpIntHandler();
        __DevIntClr = SLOWINTERRUPT;
    }

#if DMA_ENGINE_EN
    if (__USBIntSt & USBINT_STATUS_DMA) {
        usbDevDMAException();
    }
#endif

USB_ISR_EXIT:
    bEPPflags.bits.in_isr = 0;                                          /*  标识程序退出中断            */
}

/*********************************************************************************************************
** Function name:       usbDevGetVer
** Descriptions:        得到 USB Device 软件包版本号
** input parameters:    无
** output parameters:   无
** Returned value:      软件包版本号
*********************************************************************************************************/
UINT16 usbDevGetVer(void)
{
    return 0x0113;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
