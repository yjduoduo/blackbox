/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevCI.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC2400 �̼���������ӿڲ�ͷ�ļ�
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            �޸Ĳ��ִ����Է��ϴ���淶v1.2
*********************************************************************************************************/

#ifndef __USBDEVCI_H
#define __USBDEVCI_H

/*********************************************************************************************************
  The Mask of USB Device Interrupt Register
*********************************************************************************************************/

#define __FRAME                       0x00000001
#define __EP_FAST                     0x00000002
#define __EP_SLOW                     0x00000004
#define __DEV_STAT                    0x00000008
#define __CCEMPTY                     0x00000010
#define __CDFULL                      0x00000020
#define __RxENDPKT                    0x00000040
#define __TxENDPKT                    0x00000080
#define __EP_RLZED                    0x00000100
#define __ERR_INT                     0x00000200

/*********************************************************************************************************
  Interrupt Bit Definitions for EPs and Device Status Interrupt
*********************************************************************************************************/

#define FRAMEINTERRUPT              0x00000001
#define FASTINTERRUPT               0x00000002
#define SLOWINTERRUPT               0x00000004
#define DevStatusInterrupt          0x00000008
#define EP_RLZEDINT                 0x00000100

/*********************************************************************************************************
  The Mask of USB Device Interrupt Priority Register
*********************************************************************************************************/

#define FRAME_PRIO                  0x01
#define EP_FAST_PRIO                0x02

/*********************************************************************************************************
  Interrupt Bit Definition for DMA Interrupt
*********************************************************************************************************/

#define USB_DMA_INT                 0x00000040

/*********************************************************************************************************
  USB DMA Interrupt
*********************************************************************************************************/

#define USBDMA_EOT_INT              0x01
#define USBDMA_NDD_INT              0x02
#define USBDMA_ERR_INT              0x04

#define EP0MaxPacketSize            64
#define EP1MaxPacketSize            64
#define EP2MaxPacketSize            64
#define EP3MaxPacketSize            1023
#define EP4MaxPacketSize            64
#define EP5MaxPacketSize            64
#define EP6MaxPacketSize            1023
#define EP7MaxPacketSize            64
#define EP8MaxPacketSize            64
#define EP9MaxPacketSize            1023
#define EP10MaxPacketSize           64
#define EP11MaxPacketSize           64
#define EP12MaxPacketSize           1023
#define EP13MaxPacketSize           64
#define EP14MaxPacketSize           64
#define EP15MaxPacketSize           64

/*********************************************************************************************************
  USB Interrupt Status Register Mask
*********************************************************************************************************/

#define USBINT_STATUS_BLOCKINTS		0x80000000
#define USBINT_STATUS_DMA			0x00000004
#define USBINT_STATUS_HP			0x00000002
#define USBINT_STATUS_LP			0x00000001

/*********************************************************************************************************
  Device Interrupt Status Register Mask
*********************************************************************************************************/

#define DEVINT_STATUS_ERRINT        0x00000200
#define DEVINT_STATUS_EPRLZED       0x00000100
#define DEVINT_STATUS_CDFULL        0x00000020
#define DEVINT_STATUS_DDEMPTY       0x00000010
#define DEVINT_STATUS_DEVSTAT       0x00000008
#define DEVINT_STATUS_EP_FAST       0x00000002
#define DEVINT_STATUS_EP_SLOW       0x00000004

/*********************************************************************************************************
  All Endpoint Register Mask
*********************************************************************************************************/
#define USB_ENDP00                  0x00000001
#define USB_ENDP01                  0x00000002
#define USB_ENDP02                  0x00000004
#define USB_ENDP03                  0x00000008
#define USB_ENDP04                  0x00000010
#define USB_ENDP05                  0x00000020
#define USB_ENDP06                  0x00000040
#define USB_ENDP07                  0x00000080
#define USB_ENDP08                  0x00000100
#define USB_ENDP09                  0x00000200
#define USB_ENDP10                  0x00000400
#define USB_ENDP11                  0x00000800

/*********************************************************************************************************
  Receive Packet length Register Mask
*********************************************************************************************************/

#define REC_PACKETSIZE_PKTRDY       0x00000800
#define REC_PACKETSIZE_DV           0x00000400
#define REC_PACKETSIZE_PKT_LNGTH    0x000003FF

/*********************************************************************************************************
  Select Endpoint Mask
*********************************************************************************************************/

#define USB_STALL                   0x02
#define USB_SETUPPACKET             0x04

/*********************************************************************************************************
  Device Command
*********************************************************************************************************/

#define USBCMD_SET_ADDRESS          0x00D00500
#define USBDAT_SET_ADDRESS          0x00000100

#define USBCMD_CONFIG_DEV           0x00D80500
#define USBDAT_CONFIG_DEV           0x00000100

#define USBCMD_SET_MODE             0x00F30500
#define USBDAT_SET_MODE             0x00000100

#define USBCMD_RDCURRENT_FRAME      0x00F50500
#define USBDAT_RDCURRENT_FRAME      0x00F50200

#define USBCMD_RDTEST_REG           0x00FD0500
#define USBDAT_RDTEST_REG           0x00FD0200

#define USBCMD_READ_TESTREG         0x00FD0500
#define USBDAT_READ_TESTREG         0x00FD0200

#define USBCMD_SET_DEVSTATUS        0x00FE0500
#define USBDAT_SET_DEVSTATUS        0x00000100

#define USBCMD_GET_DEVSTATUS        0x00FE0500
#define USBDAT_GET_DEVSTATUS        0x00FE0200

#define USBCMD_GET_ERRCODE          0x00FF0500
#define USBDAT_GET_ERRCODE          0x00FF0200

#define USBCMD_READ_ERRSTATUS       0x00FB0500
#define USBDAT_READ_ERRSTATUS       0x00FB0200

/*********************************************************************************************************
  Select Endpoint
*********************************************************************************************************/

#define USBCMD_SEL_ENDP             0x00000500
#define USBDAT_SEL_ENDP             0x00000200

/*********************************************************************************************************
  Select Endpoint / Clear Interrupt
*********************************************************************************************************/

#define USBCMD_SEL_CLRINT_ENDP      0x00000500
#define USBDAT_SEL_CLRINT_ENDP      0x00000200

/*********************************************************************************************************
  Set Endpoint Status
*********************************************************************************************************/

#define USBCMD_SET_ENDP_STATUS      0x00000500
#define USBDAT_SET_ENDP_STATUS      0x00000100

/*********************************************************************************************************
  Cleare Buffer
*********************************************************************************************************/

#define USBCMD_CLEAR_BUFFER         0x00F20500
#define USBDAT_CLEAR_BUFFER         0x00F20200

/*********************************************************************************************************
  Validate Buffer
*********************************************************************************************************/

#define USBCMD_VALID_BUFFER         0x00FA0500


/*********************************************************************************************************
  usbDevCI.c  ��غ���
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevConfigMaxPaketSize
** Descriptions:        ���� USB Device �˵��������С
** input parameters:    ucEpIndex       ����˵��
**                      ulPacketSize    �ö˵��������С
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevConfigMaxPaketSize (UINT8 ucEpIndex, UINT32 ulPacketSize);

/*********************************************************************************************************
** Function name:       __usbDevReadTestRegister
** Descriptions:        �����ԼĴ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ���ԼĴ�����ֵ
*********************************************************************************************************/
extern UINT16 __usbDevReadTestRegister (void);

/*********************************************************************************************************
** Function name:       __usbDevSetAddressEnable
** Descriptions:        ���� USB �豸��ַ
** input parameters:    ucAddress   ��������ĵ�ֵַ
**                      ucEnable    USB�豸ʹ��λ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevSetAddressEnable (UINT8  ucAddress, UINT8 ucEnable);

/*********************************************************************************************************
** Function name:       __usbDevSetEndpointEnable
** Descriptions:        ���� USB �豸
** input parameters:    ucEnable        USB�˵�ʹ��λ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevSetEndpointEnable (UINT8  ucEnable);

/*********************************************************************************************************
** Function name:       __usbDevSetMode
** Descriptions:        ����ģʽ
** input parameters:    ucSetVal    ģʽֵ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevSetMode (UINT8  ucSetVal);

/*********************************************************************************************************
** Function name:       __usbDevSetDevStatus
** Descriptions:        �����豸״̬
** input parameters:    ucSetVal    ģʽֵ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevSetDevStatus (UINT8 ucSetVal);

/*********************************************************************************************************
** Function name:       __usbDevGetDevStatus
** Descriptions:        ��ȡ�豸״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern UINT8 __usbDevGetDevStatus (void);

/*********************************************************************************************************
** Function name:       __usbDevSelectEndpoint
** Descriptions:        ѡ��˵�,����ȡ�˵���Ϣ
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ��Ӧ�˵����Ϣ
*********************************************************************************************************/
extern UINT8 __usbDevSelectEndpoint (UINT8 ucEpIndex);

/*********************************************************************************************************
** Function name:       __usbDevSelectClrIntEndpoint
** Descriptions:        ѡ��˵�,����ȡ�˵���Ϣ�������Ӧ�˵���ж�
** input parameters:    ucEpIndex   ����˵��
** output parameters:   ��
** Returned value:      ��Ӧ�˵����Ϣ
*********************************************************************************************************/
extern UINT8 __usbDevSelectClrIntEndpoint (UINT8 ucEpIndex);

/*********************************************************************************************************
** Function name:       __usbDevSetEndpointStatus
** Descriptions:        ���ö˵�״̬
** input parameters:    ucEpIndex       ����˵��
**                      ucStalled   �˵�ֹͣλ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevSetEndpointStatus (UINT8  ucEpIndex, UINT8  ucStalled);

/*********************************************************************************************************
** Function name:       __usbDevClearBuffer
** Descriptions:        ���OUT�˵㻺����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern UINT8 __usbDevClearBuffer (void);

/*********************************************************************************************************
** Function name:       __usbDevValidBuffer
** Descriptions:        ʹ�� IN �˵㻺����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevValidBuffer (void);

/*********************************************************************************************************
** Function name:       __usbDevValidBuffer
** Descriptions:        ʹ�� IN �˵㻺����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevValidBuffer (void);

/*********************************************************************************************************
** Function name:       __usbDevSoftReconnect
** Descriptions:        ��������USB����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __usbDevSoftReconnect (void);

/*********************************************************************************************************
** Function name:       usbDevReadEndpoint
** Descriptions:        ͨ��ָ������˵��ȡ����
** input parameters:    ucEpIndex       ����˵��
**                      pucRcvBuf       ���ջ�����ָ��
**                      ucReadLen       Ҫ�����ֽڳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern UINT8 usbDevReadEndpoint (UINT8 ucEpIndex,
                                 UINT8 *pucRcvBuf,
                                 UINT8 ucReadLen);

/*********************************************************************************************************
** Function name:       usbDevWriteEndpoint
** Descriptions:        ��ָ������˵�д������
** input parameters:    ucEpIndex       ����˵��
**                      pucWriteBuf     ���ͻ�����
**                      ucWriteLen      Ҫд���ֽڳ���
** output parameters:   ��
** Returned value:      ʵ��д����ֽ���
*********************************************************************************************************/
extern UINT8 usbDevWriteEndpoint (UINT8 ucEpIndex,
                                  UINT8 *pucWriteBuf,
                                  UINT8 ucWriteLen);


#endif                                                                  /*  __USBDEVCI_H                */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/


