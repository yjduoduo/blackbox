/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDescriptor.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC2400 固件程序 描述符层头文件
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

#ifndef __USBDEVDESCRIPTOR_H
#define __USBDEVDESCRIPTOR_H

/*********************************************************************************************************
  USB Descriptor structure and their member value
*********************************************************************************************************/

#define USB_IDVENDOR_LOW        0x71                                    /*  USB设备的厂商ID号           */
#define USB_IDVENDOR_HIGH       0x04

#define USB_IDPRODUCT_LOW       0x00                                    /*  USB设备的产品ID号           */
#define USB_IDPRODUCT_HIGH      0x17

/*********************************************************************************************************
  USB  device descriptor structure
*********************************************************************************************************/

typedef struct _USB_DEVICE_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bcdUSB0;
    UINT8 bcdUSB1;
    UINT8 bDeviceClass;
    UINT8 bDeviceSubClass;
    UINT8 bDeviceProtocol;
    UINT8 bMaxPacketSize0;
    UINT8 idVendor0;
    UINT8 idVendor1;
    UINT8 idProduct0;
    UINT8 idProduct1;
    UINT8 bcdDevice0;
    UINT8 bcdDevice1;
    UINT8 iManufacturer;
    UINT8 iProduct;
    UINT8 iSerialNumber;
    UINT8 bNumConfigurations;
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;

/*********************************************************************************************************
  USB  configuration descriptor structure
*********************************************************************************************************/

typedef struct _USB_CONFIGURATION_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 wTotalLength0;
    UINT8 wTotalLength1;
    UINT8 bNumInterfaces;
    UINT8 bConfigurationValue;
    UINT8 iConfiguration;
    UINT8 bmAttributes;
    UINT8 MaxPower;
} USB_CONFIGURATION_DESCRIPTOR, *PUSB_CONFIGURATION_DESCRIPTOR;

/*********************************************************************************************************
  USB  interface descriptor structure
*********************************************************************************************************/
typedef struct _USB_INTERFACE_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bInterfaceNumber;
    UINT8 bAlternateSetting;
    UINT8 bNumEndpoints;
    UINT8 bInterfaceClass;
    UINT8 bInterfaceSubClass;
    UINT8 bInterfaceProtocol;
    UINT8 iInterface;
} USB_INTERFACE_DESCRIPTOR, *PUSB_INTERFACE_DESCRIPTOR;

/*********************************************************************************************************
  USB  endpoint descriptor structure
*********************************************************************************************************/
typedef struct _USB_ENDPOINT_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bEndpointAddress;
    UINT8 bmAttributes;
    UINT8 wMaxPacketSize0;
    UINT8 wMaxPacketSize1;
    UINT8 bInterval;
} USB_ENDPOINT_DESCRIPTOR, *PUSB_ENDPOINT_DESCRIPTOR;

/*********************************************************************************************************
  USB string descriptor structure
*********************************************************************************************************/
typedef struct _USB_STRING_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
    UINT8 bString[1];
} USB_STRING_DESCRIPTOR, *PUSB_STRING_DESCRIPTOR;

#define MAXIMUM_USB_STRING_LENGTH     255                               /*  USB max string length       */

/*********************************************************************************************************
  USB power descriptor structure
*********************************************************************************************************/
typedef __packed struct _USB_POWER_DESCRIPTOR {
    UINT8  bLength;
    UINT8  bDescriptorType;
    UINT8  bCapabilitiesFlags;
    UINT16 EventNotification;
    UINT16 D1LatencyTime;
    UINT16 D2LatencyTime;
    UINT16 D3LatencyTime;
    UINT8  PowerUnit;
    UINT16 D0PowerConsumption;
    UINT16 D1PowerConsumption;
    UINT16 D2PowerConsumption;
} USB_POWER_DESCRIPTOR, *PUSB_POWER_DESCRIPTOR;

/*********************************************************************************************************
  USB power descriptor value
*********************************************************************************************************/
#define USB_SUPPORT_D0_COMMAND      0x01
#define USB_SUPPORT_D1_COMMAND      0x02
#define USB_SUPPORT_D2_COMMAND      0x04
#define USB_SUPPORT_D3_COMMAND      0x08

#define USB_SUPPORT_D1_WAKEUP       0x10
#define USB_SUPPORT_D2_WAKEUP       0x20

/*********************************************************************************************************
  USB common descriptor structure
*********************************************************************************************************/
typedef struct _USB_COMMON_DESCRIPTOR {
    UINT8 bLength;
    UINT8 bDescriptorType;
} USB_COMMON_DESCRIPTOR, *PUSB_COMMON_DESCRIPTOR;

/*********************************************************************************************************
  USB HUB descriptor structure
*********************************************************************************************************/
typedef __packed struct _USB_HUB_DESCRIPTOR {
    UINT8        bDescriptorLength;                                     /*  Length of this descriptor   */
    UINT8        bDescriptorType;                                       /*  Hub configuration type      */
    UINT8        bNumberOfPorts;                                        /*  number of ports on this hub */
    UINT16       wHubCharacteristics;                                   /*  Hub Charateristics          */
    UINT8        bPowerOnToPowerGood;                                   /*  power on to good in 2ms     */
    UINT8        bHubControlCurrent;                                    /*  max current in mA           */
    //
    // room for 255 ports power control and removable bitmask
    UINT8        bRemoveAndPowerMask[64];
} USB_HUB_DESCRIPTOR, *PUSB_HUB_DESCRIPTOR;

/*********************************************************************************************************
  Common descriptor struture member value
*********************************************************************************************************/

/*********************************************************************************************************
  USB device request type mask
*********************************************************************************************************/

//(1) Receiver: D4...D0
#define USB_RECIPIENT            (UINT8)0x1F
#define USB_RECIPIENT_DEVICE     (UINT8)0x00
#define USB_RECIPIENT_INTERFACE  (UINT8)0x01
#define USB_RECIPIENT_ENDPOINT   (UINT8)0x02

//(2) Type: D6...D5
#define USB_REQUEST_TYPE_MASK    (UINT8)0x60
#define USB_STANDARD_REQUEST     (UINT8)0x00
#define USB_CLASS_REQUEST        (UINT8)0x20
#define USB_VENDOR_REQUEST       (UINT8)0x40

//(3) data direction: D7
#define DEVICE_ADDRESS_MASK      0x7F

//(4) USB request mask
#define USB_REQUEST_MASK         (UINT8)0x0F

//(5) USB endpoint direction mask
#define USB_ENDPOINT_DIRECTION_MASK               0x80
#define USB_ENDPOINT_DIRECTION_OUT(addr)          (!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define USB_ENDPOINT_DIRECTION_IN(addr)           ((addr) & USB_ENDPOINT_DIRECTION_MASK)

/*********************************************************************************************************
  USB device request type
*********************************************************************************************************/
#define USB_DEVICE_DESCRIPTOR_TYPE                0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE         0x02
#define USB_STRING_DESCRIPTOR_TYPE                0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE             0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE              0x05
#define USB_POWER_DESCRIPTOR_TYPE                 0x06

/*********************************************************************************************************
  Correct value based on USB1.0 specification
*********************************************************************************************************/
#define USB_REQUEST_GET_STATUS                    0x00
#define USB_REQUEST_CLEAR_FEATURE                 0x01

#define USB_REQUEST_SET_FEATURE                   0x03

#define USB_REQUEST_SET_ADDRESS                   0x05
#define USB_REQUEST_GET_DESCRIPTOR                0x06
#define USB_REQUEST_SET_DESCRIPTOR                0x07
#define USB_REQUEST_GET_CONFIGURATION             0x08
#define USB_REQUEST_SET_CONFIGURATION             0x09
#define USB_REQUEST_GET_INTERFACE                 0x0A
#define USB_REQUEST_SET_INTERFACE                 0x0B
#define USB_REQUEST_SYNC_FRAME                    0x0C

/*********************************************************************************************************
  USB feather select
*********************************************************************************************************/
#define USB_FEATURE_ENDPOINT_STALL                0x0000
#define USB_FEATURE_REMOTE_WAKEUP                 0x0001
#define USB_FEATURE_POWER_D0                      0x0002
#define USB_FEATURE_POWER_D1                      0x0003
#define USB_FEATURE_POWER_D2                      0x0004
#define USB_FEATURE_POWER_D3                      0x0005

#define USB_DESCRIPTOR_MAKE_TYPE_AND_INDEX(d, i) ((UINT16)((UINT16)d<<8 | i))

/*********************************************************************************************************
  USB configuration descriptor
*********************************************************************************************************/
#define USB_CONFIG_POWERED_MASK                   0xc0

#define USB_CONFIG_BUS_POWERED                    0x80
#define USB_CONFIG_SELF_POWERED                   0x40
#define USB_CONFIG_REMOTE_WAKEUP                  0x20

#define BUS_POWERED                               0x80
#define SELF_POWERED                              0x40
#define REMOTE_WAKEUP                             0x20

/*********************************************************************************************************
  USB endpoint descriptor structure: endpoint attribute
*********************************************************************************************************/
#define USB_ENDPOINT_TYPE_MASK                    0x03

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03

/*********************************************************************************************************
  USB  device class
*********************************************************************************************************/
#define USB_DEVICE_CLASS_RESERVED                 0x00
#define USB_DEVICE_CLASS_AUDIO                    0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS           0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE          0x03
#define USB_DEVICE_CLASS_MONITOR                  0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE       0x05
#define USB_DEVICE_CLASS_POWER                    0x06
#define USB_DEVICE_CLASS_PRINTER                  0x07
#define USB_DEVICE_CLASS_STORAGE                  0x08
#define USB_DEVICE_CLASS_HUB                      0x09
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC          0xFF

/*********************************************************************************************************
  LPC2400 USB various descriptor structure collect
*********************************************************************************************************/
typedef struct _USB_DESCRIPTOR {
    USB_CONFIGURATION_DESCRIPTOR ConfigDescr;       //configuration descriptor
    USB_INTERFACE_DESCRIPTOR InterfaceDescr;        //interface descriptor

    USB_ENDPOINT_DESCRIPTOR EP1_TXDescr;            //logic endpoint 1 IN  descriptor
    USB_ENDPOINT_DESCRIPTOR EP1_RXDescr;            //logic endpoint 1 OUT descriptor
    USB_ENDPOINT_DESCRIPTOR EP2_TXDescr;            //logic endpoint 2 IN  descriptor
    USB_ENDPOINT_DESCRIPTOR EP2_RXDescr;            //logic endpoint 2 OUT descriptor

} USB_DESCRIPTOR, *PUSB_DESCRIPTOR;

/*********************************************************************************************************
  In LPC2400 USB descriptor, the physical endpoint number except for logic enpoint 0
*********************************************************************************************************/
#define NUM_ENDPOINTS      4

/*********************************************************************************************************
  LPC2400 USB descriptor collect length
*********************************************************************************************************/
#define CONFIG_DESCRIPTOR_LENGTH    sizeof(USB_CONFIGURATION_DESCRIPTOR) \
                                    + sizeof(USB_INTERFACE_DESCRIPTOR) \
                                    + (NUM_ENDPOINTS * sizeof(USB_ENDPOINT_DESCRIPTOR))

//(1) logic endpoint 0 MaxPacketSize
#define EP0_TX_FIFO_SIZE   64
#define EP0_RX_FIFO_SIZE   64
#define EP0_PACKET_SIZE    64

//(2) logic endpoint 1 MaxPacketSize
#define EP1_TX_FIFO_SIZE   16
#define EP1_RX_FIFO_SIZE   16
#define EP1_PACKET_SIZE    16

//(3) logic endpoint 2 MaxPacketSize
#define EP2_TX_FIFO_SIZE   64
#define EP2_RX_FIFO_SIZE   64
#define EP2_PACKET_SIZE    64

extern const USB_DEVICE_DESCRIPTOR DeviceDescr;
extern const USB_DESCRIPTOR usb_descr;

#endif                                                                  /*  __USBDEVDESCRIPTOR_H        */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/


