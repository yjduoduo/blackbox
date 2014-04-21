/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevConfig.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 固件程序配置头文件
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
** Descriptions:            修改部分代码以符合代码规范v1.2
**
*********************************************************************************************************/

#include    "usbDevConfig.h"

/*********************************************************************************************************
  USB Device Class
*********************************************************************************************************/

#define USB_CLASS_CODE_TEST_CLASS_DEVICE                    0xdc        /*  设备描述符: 测试设备类型    */

#define USB_SUBCLASS_CODE_TEST_CLASS_LPC3250                0xA0        /*  接口描述符: 子类代码        */

#define USB_PROTOCOL_CODE_TEST_CLASS_LPC3250                0xB0        /*  接口描述符: 协议代码        */

/*********************************************************************************************************
  USB Device descriptor
*********************************************************************************************************/
const USB_DEVICE_DESCRIPTOR DeviceDescr =
{
    sizeof(USB_DEVICE_DESCRIPTOR),                                      /*  设备描述符长度,= 12H        */
    USB_DEVICE_DESCRIPTOR_TYPE,                                         /*  设备描述符类型,= 01H        */
    0x00,0x01,                                                          /*  协议版本,= 1.10             */
    USB_CLASS_CODE_TEST_CLASS_DEVICE,                                   /*  测试设备类型, = 0DCH        */
    0, 0,                                                               /*  设备子类,设备协议           */
    EP0_PACKET_SIZE,                                                    /*  端点0最大数据包大小,= 40H   */
    USB_IDVENDOR_LOW,
    USB_IDVENDOR_HIGH,                                                  /*  PHILIPS公司的设备ID         */
    USB_IDPRODUCT_LOW,
    USB_IDPRODUCT_HIGH,                                                 /*  设备制造商定的产品ID        */
    0x00,0x01,                                                          /*  设备系列号                  */
    0, 0, 0,                                                            /*  索引                        */
    1                                                                   /*  可能的配置数                */
};

const USB_DESCRIPTOR usb_descr =
{
//configuration descriptor                                              /*  配置描述符                  */
{
    sizeof(USB_CONFIGURATION_DESCRIPTOR),                               /*  配置描述符长度,= 09H        */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,                                  /*  配置描述符类型,= 02H        */
    CONFIG_DESCRIPTOR_LENGTH,0x00,                                      /*  描述符总长度, = 002EH       */
    1,                                                                  /*  只支持1个接口               */
    1,                                                                  /*  配置值                      */
    0,                                                                  /*  字符串描述符指针(无)        */
    0x60,                                                               /*  自供电,支持远程唤醒         */
    0x32                                                                /*  最大功耗(100mA)             */
},
//interface descriptor                                                  /*  接口描述符                  */
{
    sizeof(USB_INTERFACE_DESCRIPTOR),                                   /*  接口描述符长度,= 09H        */
    USB_INTERFACE_DESCRIPTOR_TYPE,                                      /*  接口描述符类型,= 04H        */
    0,                                                                  /*  接口数,只有1个              */
    0,                                                                  /*  可选配置,只有1个            */
    NUM_ENDPOINTS,                                                      /*  除端点0的端点索引数目,=04H  */
    USB_CLASS_CODE_TEST_CLASS_DEVICE,                                   /*  测试设备类型,= 0DCH         */
    USB_SUBCLASS_CODE_TEST_CLASS_LPC3250,                               /*  子类代码,= 0A0H             */
    USB_PROTOCOL_CODE_TEST_CLASS_LPC3250,                               /*  协议代码,= 0B0H             */
    0                                                                   /*  字符串描述符索引            */
},
//endpoint descriptor, logic endpoint 1 IN                              /*  逻辑端点 1 输入             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x81,                                                               /*  端点1 IN                    */
    USB_ENDPOINT_TYPE_INTERRUPT,                                        /*  中断传输, = 03H             */
    EP1_PACKET_SIZE,0x00,                                               /*  端点最大包的大小,= 0010H    */
    1                                                                   /*  传输间隔时间,= 10ms         */
},
//endpoint descriptor, logic endpoint 1 OUT                             /*  逻辑端点 1 输出             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x1,                                                                /*  端点1 OUT                   */
    USB_ENDPOINT_TYPE_INTERRUPT,                                        /*  中断传输,= 03H              */
    EP1_PACKET_SIZE,0x00,                                               /*  端点最大包的大小,= 0010H    */
    1                                                                   /*  传输间隔时间,= 10ms         */
},
//endpoint descriptor, logic endpoint 2 IN                              /*  逻辑端点 2 输入             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x82,                                                               /*  端点2 IN                    */
    USB_ENDPOINT_TYPE_BULK,                                             /*  批量传输,= 02H              */
    EP2_PACKET_SIZE,0x00,                                               /*  端点最大包的大小,= 0040H    */
    10                                                                  /*  批量传输时该值无效          */
},
//endpoint descriptor, logic endpoint 2 OUT                             /*  逻辑端点 2 输出             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  端点描述符长度,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  端点描述符类型,= 05H        */
    0x2,                                                                /*  端点2 OUT                   */
    USB_ENDPOINT_TYPE_BULK,                                             /*  批量传输,= 02H              */
    EP2_PACKET_SIZE,0x00,                                               /*  端点最大包的大小,= 0040H    */
    10                                                                  /*  批量传输时该值无效          */
}
};

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
