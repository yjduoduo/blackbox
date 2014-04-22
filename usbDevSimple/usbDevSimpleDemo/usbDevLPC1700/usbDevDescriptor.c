/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevConfig.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 �̼���������ͷ�ļ�
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
** Descriptions:            �޸Ĳ��ִ����Է��ϴ���淶v1.2
**
*********************************************************************************************************/

#include    "usbDevConfig.h"

/*********************************************************************************************************
  USB Device Class
*********************************************************************************************************/

#define USB_CLASS_CODE_TEST_CLASS_DEVICE                    0xdc        /*  �豸������: �����豸����    */

#define USB_SUBCLASS_CODE_TEST_CLASS_LPC3250                0xA0        /*  �ӿ�������: �������        */

#define USB_PROTOCOL_CODE_TEST_CLASS_LPC3250                0xB0        /*  �ӿ�������: Э�����        */

/*********************************************************************************************************
  USB Device descriptor
*********************************************************************************************************/
const USB_DEVICE_DESCRIPTOR DeviceDescr =
{
    sizeof(USB_DEVICE_DESCRIPTOR),                                      /*  �豸����������,= 12H        */
    USB_DEVICE_DESCRIPTOR_TYPE,                                         /*  �豸����������,= 01H        */
    0x00,0x01,                                                          /*  Э��汾,= 1.10             */
    USB_CLASS_CODE_TEST_CLASS_DEVICE,                                   /*  �����豸����, = 0DCH        */
    0, 0,                                                               /*  �豸����,�豸Э��           */
    EP0_PACKET_SIZE,                                                    /*  �˵�0������ݰ���С,= 40H   */
    USB_IDVENDOR_LOW,
    USB_IDVENDOR_HIGH,                                                  /*  PHILIPS��˾���豸ID         */
    USB_IDPRODUCT_LOW,
    USB_IDPRODUCT_HIGH,                                                 /*  �豸�����̶��Ĳ�ƷID        */
    0x00,0x01,                                                          /*  �豸ϵ�к�                  */
    0, 0, 0,                                                            /*  ����                        */
    1                                                                   /*  ���ܵ�������                */
};

const USB_DESCRIPTOR usb_descr =
{
//configuration descriptor                                              /*  ����������                  */
{
    sizeof(USB_CONFIGURATION_DESCRIPTOR),                               /*  ��������������,= 09H        */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,                                  /*  ��������������,= 02H        */
    CONFIG_DESCRIPTOR_LENGTH,0x00,                                      /*  �������ܳ���, = 002EH       */
    1,                                                                  /*  ֻ֧��1���ӿ�               */
    1,                                                                  /*  ����ֵ                      */
    0,                                                                  /*  �ַ���������ָ��(��)        */
    0x60,                                                               /*  �Թ���,֧��Զ�̻���         */
    0x32                                                                /*  ��󹦺�(100mA)             */
},
//interface descriptor                                                  /*  �ӿ�������                  */
{
    sizeof(USB_INTERFACE_DESCRIPTOR),                                   /*  �ӿ�����������,= 09H        */
    USB_INTERFACE_DESCRIPTOR_TYPE,                                      /*  �ӿ�����������,= 04H        */
    0,                                                                  /*  �ӿ���,ֻ��1��              */
    0,                                                                  /*  ��ѡ����,ֻ��1��            */
    NUM_ENDPOINTS,                                                      /*  ���˵�0�Ķ˵�������Ŀ,=04H  */
    USB_CLASS_CODE_TEST_CLASS_DEVICE,                                   /*  �����豸����,= 0DCH         */
    USB_SUBCLASS_CODE_TEST_CLASS_LPC3250,                               /*  �������,= 0A0H             */
    USB_PROTOCOL_CODE_TEST_CLASS_LPC3250,                               /*  Э�����,= 0B0H             */
    0                                                                   /*  �ַ�������������            */
},
//endpoint descriptor, logic endpoint 1 IN                              /*  �߼��˵� 1 ����             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  �˵�����������,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  �˵�����������,= 05H        */
    0x81,                                                               /*  �˵�1 IN                    */
    USB_ENDPOINT_TYPE_INTERRUPT,                                        /*  �жϴ���, = 03H             */
    EP1_PACKET_SIZE,0x00,                                               /*  �˵������Ĵ�С,= 0010H    */
    1                                                                   /*  ������ʱ��,= 10ms         */
},
//endpoint descriptor, logic endpoint 1 OUT                             /*  �߼��˵� 1 ���             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  �˵�����������,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  �˵�����������,= 05H        */
    0x1,                                                                /*  �˵�1 OUT                   */
    USB_ENDPOINT_TYPE_INTERRUPT,                                        /*  �жϴ���,= 03H              */
    EP1_PACKET_SIZE,0x00,                                               /*  �˵������Ĵ�С,= 0010H    */
    1                                                                   /*  ������ʱ��,= 10ms         */
},
//endpoint descriptor, logic endpoint 2 IN                              /*  �߼��˵� 2 ����             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  �˵�����������,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  �˵�����������,= 05H        */
    0x82,                                                               /*  �˵�2 IN                    */
    USB_ENDPOINT_TYPE_BULK,                                             /*  ��������,= 02H              */
    EP2_PACKET_SIZE,0x00,                                               /*  �˵������Ĵ�С,= 0040H    */
    10                                                                  /*  ��������ʱ��ֵ��Ч          */
},
//endpoint descriptor, logic endpoint 2 OUT                             /*  �߼��˵� 2 ���             */
{
    sizeof(USB_ENDPOINT_DESCRIPTOR),                                    /*  �˵�����������,= 07H        */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                                       /*  �˵�����������,= 05H        */
    0x2,                                                                /*  �˵�2 OUT                   */
    USB_ENDPOINT_TYPE_BULK,                                             /*  ��������,= 02H              */
    EP2_PACKET_SIZE,0x00,                                               /*  �˵������Ĵ�С,= 0040H    */
    10                                                                  /*  ��������ʱ��ֵ��Ч          */
}
};

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
