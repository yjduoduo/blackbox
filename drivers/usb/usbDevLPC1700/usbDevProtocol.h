/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevProtocol.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC2400 固件程序协议层头文件
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
** Descriptions:            修改部分代码以适合SmartARM2400系列开发板
**
*********************************************************************************************************/

#ifndef __USBDEVPROTOCOL_H
#define __USBDEVPROTOCOL_H

#define MSB(x)              (((x) >> 8) & 0xFF)

#define MAX_ENDPOINTS       (UINT8)0x3

/*********************************************************************************************************
  The value of Get Status command
*********************************************************************************************************/

#define USB_GETSTATUS_SELF_POWERED                0x01
#define USB_GETSTATUS_REMOTE_WAKEUP_ENABLED       0x02

/*********************************************************************************************************
  USB Status value
*********************************************************************************************************/

#define USB_IDLE           0
#define USB_TRANSMIT       1
#define USB_RECEIVE        2

/*********************************************************************************************************
  usbDevProtocol.c  used structure
*********************************************************************************************************/

typedef struct _device_request
{
    UINT8 bmRequestType;
    UINT8 bRequest;
    UINT16 wValue;
    UINT16 wIndex;
    UINT16 wLength;

} DEVICE_REQUEST;

/*********************************************************************************************************
  Control Transfer Structure
*********************************************************************************************************/

#define MAX_CONTROLDATA_SIZE    64

typedef struct _control_xfer
{
    DEVICE_REQUEST DeviceRequest;
    UINT16 wLength;
    UINT16 wCount;
    UINT8 * pData;
    UINT8 dataBuffer[MAX_CONTROLDATA_SIZE];

} CONTROL_XFER;

/*********************************************************************************************************
  USB standard device request service program
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevEp0Rxdone
** Descriptions:        通过端点索引 0 接收数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void __usbDevEp0RxDone(void);

/*********************************************************************************************************
** Function name:       __usbDevEp0Txdone
** Descriptions:        通过物理端点 0 发送数据
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void __usbDevEp0TxDone(void);

/*********************************************************************************************************
** Function name:       __usbDevControlHandler
** Descriptions:        控制传输
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void __usbDevControlHandler (void);


#endif                                                                  /*  __USBDEVPROTOCOL_H          */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

