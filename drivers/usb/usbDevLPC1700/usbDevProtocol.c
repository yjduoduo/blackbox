/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevProtocol.c
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 固件程序协议层源文件
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
**
*********************************************************************************************************/

#include "usbDevConfig.h"

CONTROL_XFER ControlData;                                               /*  定义传输控制结构变量        */

/*********************************************************************************************************
  USB 协议层函数
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbStallEp0
** Descriptions:        使控制端点处于停止状态
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevStallEp0 (void)
{
    __usbDevSetEndpointStatus(0, 1);                                    /*  停止控制OUT端点             */
    __usbDevSetEndpointStatus(1, 1);                                    /*  停止控制IN端点              */
}

/*********************************************************************************************************
** Function name:       __usbDevReserved
** Descriptions:        保留子程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevReserved (void)
{
    __usbDevStallEp0();
}

/*********************************************************************************************************
** Function name:       __usbDevInitUnconfig
** Descriptions:        进入地址状态,禁止 0 除外的所有端点
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevInitUnconfig (void)
{
    __usbDevSetEndpointEnable(0);
}

/*********************************************************************************************************
** Function name:       __usbInitConfig
** Descriptions:        配置处理,允许端点收发
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevInitConfig (void)
{
    __usbDevSetEndpointEnable(1);
}

/*********************************************************************************************************
** Function name:       __usbDevSingleTransmit
** Descriptions:        通过物理端点 1 发送数据
** input parameters:    pcSendbuf   发送数据指针
**                      ucSendlen   发送数据长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSingleTransmit (UINT8 *pucSendbuf,UINT8 ucSendLen)
{
    if( ucSendLen <= EP0_PACKET_SIZE) {                                 /*  不超过端点最大包大小        */

        usbDevWriteEndpoint(1,
                            pucSendbuf,
                            ucSendLen);                                 /*  向控制IN端点写入1字节数据   */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevCodeTransmit
** Descriptions:        通过物理端点 1 发送数据
** input parameters:    pucRomData  发送数据指针
**                      usSendLen   发送数据长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevCodeTransmit(UINT8 * pucRomData, UINT16 usSendLen)
{
    ControlData.wCount = 0;

    if (ControlData.wLength > usSendLen) {

        ControlData.wLength = usSendLen;                                /*  长度要小于usSendLen         */
    }

    ControlData.pData = pucRomData;

    if (ControlData.wLength >= EP0_PACKET_SIZE) {

        usbDevWriteEndpoint(1,
                            ControlData.pData,
                            EP0_PACKET_SIZE );

        ControlData.wCount += EP0_PACKET_SIZE;                          /*  计数器累加                  */

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.control_state = USB_TRANSMIT;                    /*  标识发送状态                */
        USB_EXIT_CRITICAL_SECTION();

    } else {

        usbDevWriteEndpoint(1,
                            pucRomData,
                            (UINT8)ControlData.wLength);                /*  将全部数据写入端点          */

        ControlData.wCount =(UINT16)(ControlData.wCount
                                     + ControlData.wLength);            /*  计数器累加                  */

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.control_state = USB_IDLE;                        /*  标识空闲状态                */
        USB_EXIT_CRITICAL_SECTION();
    }
}

/*********************************************************************************************************
  USB 标准设备请求服务程序
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevGetStatus
** Descriptions:        主机要求获取状态,设备返回16位的状态描述符给主机
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevGetStatus (void)
{
    UINT8 ucEpindex   = 0;
    UINT8 ucTxdata[2] = {0};
    UINT8 ucEpstat    = 0;

    UINT8 ucRecipient =(UINT8)(ControlData.DeviceRequest.bmRequestType & USB_RECIPIENT);
                                                                        /*  获取请求类型                */
    if (ucRecipient == USB_RECIPIENT_DEVICE) {

        if (bEPPflags.bits.remote_wakeup == 1) {
            ucTxdata[0] = 3;                                            /*  支持远程唤醒和自供电        */
        } else {

            ucTxdata[0] = 1;                                            /*  不支持以上两个功能          */
        }

        ucTxdata[1]=0;                                                  /*  高 8 位清 0                 */
        __usbDevSingleTransmit(ucTxdata, 2);                            /*  发送16ibt到USB主机          */

    } else if (ucRecipient == USB_RECIPIENT_INTERFACE) {

        ucTxdata[0]=0;
        ucTxdata[1]=0;
        __usbDevSingleTransmit(ucTxdata, 2);                            /*  发送16ibt到USB主机          */

    } else if (ucRecipient == USB_RECIPIENT_ENDPOINT) {

        ucEpindex = (UINT8)(ControlData.DeviceRequest.wIndex & MAX_ENDPOINTS);
        if (ControlData.DeviceRequest.wIndex & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {

            ucEpstat = __usbDevSelectEndpoint((UINT8)(ucEpindex * 2 + 1));
                                                                        /*  读 IN 端点状态              */
        } else {

            ucEpstat = __usbDevSelectEndpoint((UINT8)(ucEpindex * 2));  /*  读 OUT 端点状态             */
        }

        if (ucEpstat & USB_STALL) {
            ucTxdata[0] = 1;                                            /*  端点已被禁止                */

        } else {

            ucTxdata[0] = 0;                                            /*  端点已解禁                  */
        }

        ucTxdata[1] = 0;
        __usbDevSingleTransmit(ucTxdata, 2);                            /*  发送16ibt到USB主机          */

    } else {

        __usbDevStallEp0();                                             /*  非标准请求,禁止逻辑端点0    */
    }

}

/*********************************************************************************************************
** Function name:       __usbDevClearFeature
** Descriptions:        清除特性
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevClearFeature (void)
{
    UINT8 ucEpindex   = 0;
    UINT8 ucRecipient = (UINT8)(ControlData.DeviceRequest.bmRequestType & USB_RECIPIENT);
                                                                        /*  获取请求类型                */
    if (ucRecipient == USB_RECIPIENT_DEVICE
        && ControlData.DeviceRequest.wValue == USB_FEATURE_REMOTE_WAKEUP) {

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.remote_wakeup = 0;                               /*  清除远程唤醒特性            */
        USB_EXIT_CRITICAL_SECTION();
        __usbDevSingleTransmit(0, 0);                                   /*  返回一个空包                */

    } else if ((ucRecipient == USB_RECIPIENT_ENDPOINT)
               && (ControlData.DeviceRequest.wValue == USB_FEATURE_ENDPOINT_STALL)) {

        ucEpindex = (UINT8)(ControlData.DeviceRequest.wIndex & MAX_ENDPOINTS);
        if (ControlData.DeviceRequest.wIndex & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2 + 1), 0);   /*  解禁 IN 端点                */

        } else {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2), 0);       /*  解禁 OUT 端点               */
        }

        __usbDevSingleTransmit(0, 0);                                   /*  发送空包表示执行成功        */

    } else {

        __usbDevStallEp0();                                             /*  不成功,禁止逻辑端点0        */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevSetFeature
** Descriptions:        设置特性
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetFeature (void)
{
    UINT8 ucEpindex = 0;
    UINT8 ucRecipient =(UINT8)(ControlData.DeviceRequest.bmRequestType & USB_RECIPIENT);
                                                                        /*  获取请求类型                */
    if ((ucRecipient == USB_RECIPIENT_DEVICE)                           /*  请求设备                    */
        && (ControlData.DeviceRequest.wValue == USB_FEATURE_REMOTE_WAKEUP)) {

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.remote_wakeup = 1;                               /*  设置远程唤醒标志            */
        USB_EXIT_CRITICAL_SECTION();
        __usbDevSingleTransmit(0, 0);                                   /*  返回一个空包                */

    } else if ((ucRecipient == USB_RECIPIENT_ENDPOINT)                  /*  请求端点                    */
        && (ControlData.DeviceRequest.wValue == USB_FEATURE_ENDPOINT_STALL)) {

        ucEpindex = (UINT8)(ControlData.DeviceRequest.wIndex & MAX_ENDPOINTS);

        if (ControlData.DeviceRequest.wIndex & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2 + 1), 1);   /*  IN 端点被禁止               */

        } else {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2), 1);       /*  OUT端点被禁止               */
        }

        __usbDevSingleTransmit(0, 0);                                   /*  返回一个空包                */

    } else {

        __usbDevStallEp0();                                             /*  非标准请求,禁止逻辑端点0    */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevSetAddress
** Descriptions:        设置地址
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetAddress (void)
{
    __usbDevSetAddressEnable((UINT8)(ControlData.DeviceRequest.wValue & DEVICE_ADDRESS_MASK),1);

    __usbDevSingleTransmit(0, 0);                                       /*  返回一个空包                */
}

/*********************************************************************************************************
** Function name:       __usbDevGetDescriptor
** Descriptions:        获取描述符
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevGetDescriptor (void)
{
    UINT8 ucDescriptor =(UINT8)(MSB(ControlData.DeviceRequest.wValue)); /*  取得描述符类型              */

    if (ucDescriptor == USB_DEVICE_DESCRIPTOR_TYPE) {
                                                                        /*  获取设备描述符              */
        __usbDevCodeTransmit((UINT8 *)&DeviceDescr, sizeof(USB_DEVICE_DESCRIPTOR));

    } else if (ucDescriptor == USB_CONFIGURATION_DESCRIPTOR_TYPE) {

         if (ControlData.DeviceRequest.wLength > CONFIG_DESCRIPTOR_LENGTH) {

             ControlData.DeviceRequest.wLength = CONFIG_DESCRIPTOR_LENGTH;
         }
                                                                        /*  传输描述符内容              */
         __usbDevCodeTransmit((UINT8 *)&(usb_descr.ConfigDescr),ControlData.DeviceRequest.wLength);

    } else {

         __usbDevStallEp0();                                            /*  禁止逻辑端点0               */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevGetConfiguration
** Descriptions:        获取配置
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevGetConfiguration (void)
{
    UINT8 ucTmp = (UINT8)bEPPflags.bits.configuration;                  /*  取得配置值                  */

    __usbDevSingleTransmit(&ucTmp, 1);                                  /*  传输配置值                  */
}

/*********************************************************************************************************
** Function name:       __usbDevSetConfiguration
** Descriptions:        设置配置
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetConfiguration (void)
{
    if (ControlData.DeviceRequest.wValue == 0) {

        __usbDevSingleTransmit(0, 0);                                   /*  传输一个空包                */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.configuration = 0;                               /*  标识设备未配置              */
        USB_EXIT_CRITICAL_SECTION();
        __usbDevInitConfig();                                           /*  禁止除0外的所有逻辑端点     */

    } else if (ControlData.DeviceRequest.wValue == 1) {

        __usbDevSingleTransmit(0, 0);                                   /*  传输一个空包                */
        __usbDevInitUnconfig();                                         /*  禁止除0外的所有逻辑端点     */
        __usbDevInitConfig();                                           /*  使能全部端点                */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.configuration = 1;                               /*  标识设备已被配置            */
        USB_EXIT_CRITICAL_SECTION();

    } else {
         __usbDevStallEp0();                                            /*  禁止逻辑端点0               */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevGetInterface
** Descriptions:        获取接口信息
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevGetInterface (void)
{
    UINT8 ucTxData = 0;                                                 /*  只有一个接口                */

    __usbDevSingleTransmit(&ucTxData, 1);                               /*  传输一个字节                */
}

/*********************************************************************************************************
** Function name:       __usbDevSetInterface
** Descriptions:        设置接口信息
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetInterface (void)
{
    if (ControlData.DeviceRequest.wValue == 0 && ControlData.DeviceRequest.wIndex == 0) {

        __usbDevSingleTransmit(0, 0);                                   /*  返回一个空包,表示执行成功   */
    } else {

        __usbDevStallEp0();                                             /*  禁止逻辑端点0               */
    }
}

/*********************************************************************************************************
** Function name:       StandardDeviceRequest
** Descriptions:        USB 标准设备请求入口地址指针表
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void (*StandardDeviceRequest[]) (void) =
{
    __usbDevGetStatus,
    __usbDevClearFeature,
    __usbDevReserved,
    __usbDevSetFeature,
    __usbDevReserved,
    __usbDevSetAddress,
    __usbDevGetDescriptor,
    __usbDevReserved,
    __usbDevGetConfiguration,
    __usbDevSetConfiguration,
    __usbDevGetInterface,
    __usbDevSetInterface,
    __usbDevReserved,
    __usbDevReserved,
    __usbDevReserved,
    __usbDevReserved
};

/*********************************************************************************************************
** Function name:       __usbDevEp0RxDone
** Descriptions:        通过端点索引 0 接收数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevEp0RxDone (void)
{
    UINT8 ucEpState  = 0;
    UINT8 ucReadLen = 0;
    UINT8 req[sizeof(DEVICE_REQUEST)];

    ucEpState = __usbDevSelectClrIntEndpoint(0);                        /*  清除该端点的中断            */

    if (ucEpState & USB_SETUPPACKET) {                                  /*  接收到SETUP包               */

        ControlData.wLength = 0;
        ControlData.wCount = 0;

        if(usbDevReadEndpoint(0,req,sizeof(ControlData.DeviceRequest))
           != sizeof(DEVICE_REQUEST)) {                                 /*  从端点 0 读取数据           */

            __usbDevSetEndpointStatus(0, 1);                            /*  禁止控制端点0               */
            __usbDevSetEndpointStatus(1, 1);                            /*  禁止控制端点1               */
            bEPPflags.bits.control_state = USB_IDLE;                    /*  标识空闲状态                */
            return;
        }

        /*
         *  接收 SETUP 包成功
         */

        ControlData.DeviceRequest.bmRequestType = req[0];
        ControlData.DeviceRequest.bRequest      = req[1];
        ControlData.DeviceRequest.wValue        = (UINT16)(req[3] * 256 + req[2]);
        ControlData.DeviceRequest.wIndex        = (UINT16)(req[5] * 256 + req[4]);
        ControlData.DeviceRequest.wLength       = (UINT16)(req[7] * 256 + req[6]);
        ControlData.wLength = ControlData.DeviceRequest.wLength;

        if (ControlData.DeviceRequest.bmRequestType & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {
                                                                        /*  如果为控制读取              */
             bEPPflags.bits.setup_packet = 1;
             bEPPflags.bits.control_state = USB_TRANSMIT;

        } else {                                                        /*  如果为控制写                */

             if (ControlData.DeviceRequest.wLength == 0) {

                 bEPPflags.bits.setup_packet = 1;
                 bEPPflags.bits.control_state = USB_IDLE;

             } else {

                 if (ControlData.DeviceRequest.wLength > MAX_CONTROLDATA_SIZE) {

                    bEPPflags.bits.control_state = USB_IDLE;
                    __usbDevSetEndpointStatus(0, 1);                    /*  禁止控制端点0               */
                    __usbDevSetEndpointStatus(1, 1);                    /*  禁止控制端点1               */

                 } else {

                    bEPPflags.bits.control_state = USB_RECEIVE;         /*  标识接收状态                */
                 }

             }
        }
    }

    /*
     *  下面为控制接收数据阶段
     */

    else if (bEPPflags.bits.control_state == USB_RECEIVE) {
                                                                        /*  如果当前为接收状态          */
        ucReadLen = usbDevReadEndpoint(0,
                                       ControlData.dataBuffer + ControlData.wCount,
                                       EP0_PACKET_SIZE);                /*  从端点 0 读取数据           */

        ControlData.wCount = (UINT16)(ControlData.wCount+ucReadLen);

        if( ucReadLen != EP0_PACKET_SIZE || ControlData.wCount >= ControlData.wLength) {
                                                                        /*  完成接收数据                */
            bEPPflags.bits.setup_packet = 1;
            bEPPflags.bits.control_state = USB_IDLE;
        }

    } else {

        bEPPflags.bits.control_state = USB_IDLE;
    }
}

/*********************************************************************************************************
** Function name:       __usbDevEp0TxDone
** Descriptions:        通过物理端点 0 发送数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevEp0TxDone(void)
{
    SINT16 i = (SINT16)(ControlData.wLength - ControlData.wCount);

    __usbDevSelectClrIntEndpoint(1);                                    /*  清除端点中断标志            */

    if (bEPPflags.bits.control_state != USB_TRANSMIT) {
                                                                        /*  非发送状态                  */
        __usbDevSingleTransmit(0, 0);
        return;                                                         /*  直接返回                    */
    }

    if( i >= EP0_PACKET_SIZE) {
                                                                        /*  未发送字节数大于64          */
        usbDevWriteEndpoint(1,
                            ControlData.pData + ControlData.wCount,
                            EP0_PACKET_SIZE);

        ControlData.wCount += EP0_PACKET_SIZE;
        bEPPflags.bits.control_state = USB_TRANSMIT;

    } else if( i != 0) {
                                                                        /*  发送所有未发送的字节        */
        usbDevWriteEndpoint(1, ControlData.pData + ControlData.wCount,(UINT8)i);
        ControlData.wCount =(UINT16)(ControlData.wCount+i);
        bEPPflags.bits.control_state = USB_IDLE;                        /*  置状态为等待状态            */

    } else if (i == 0) {

        usbDevWriteEndpoint(1, 0, 0);                                   /*  完成发送,再发送一个空包     */
        bEPPflags.bits.control_state = USB_IDLE;
    }
}

/*********************************************************************************************************
** Function name:       __usbDevControlHandler
** Descriptions:        控制传输
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevControlHandler (void)
{
    UINT8 ucRequesttype  = 0;
    UINT8 ucRequestindex = 0;

    ucRequesttype  = (UINT8)(ControlData.DeviceRequest.bmRequestType & USB_REQUEST_TYPE_MASK);
                                                                        /*  读请求类型码                */

    ucRequestindex = (UINT8)(ControlData.DeviceRequest.bRequest & USB_REQUEST_MASK);

    if (ucRequesttype == USB_STANDARD_REQUEST)
        (*StandardDeviceRequest[ucRequestindex])();                     /*  标准请求                    */
    //else if (ucRequesttype == USB_VENDOR_REQUEST)                     /*  厂商请求                    */
    //  (*VendorDeviceRequest[ucRequestindex])();
    //else if(ucRequesttype == USB_CLASS_REQUEST)
    //  (*ClassDeviceRequest[ucRequestindex])();                        /*  类请求                      */
    else {

        __usbDevStallEp0();                                             /*  禁止逻辑端点0               */
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
