/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDMA.h
** Latest modified Date:    2009-01-08
** Latest Version:          1.2
** Descriptions:            usbDevice �̼����� DMA ��ͷ�ļ�
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            ������̰����ݵ�bug
**                          �޸Ĳ��ִ����Է��ϴ���淶v1.2
**--------------------------------------------------------------------------------------------------------
** Modified by:             LiuWeiyun
** Modified date:           2009-01-07
** Version:                 1.2
** Descriptions:            ��ֲ��LPC3200
** NOTE                     (1) UDCA ͷָ����128�ֽڶ���
**                          (2) LPC3200����DMA��USB RAM���������ڲ�RAM���ⲿRAM(���еĻ�)�п���
**                              �ҵ�ַ����(4�ֽ�)����
**                          (3) Ϊ�򻯲�������USB RAMҲ����128�ֽڶ���������UDCA ͷָ����128�ֽڶ���
*********************************************************************************************************/

#ifndef __USBDEVDMA_H
#define __USBDEVDMA_H

#if DMA_ENGINE_EN > 0

/*********************************************************************************************************
  LPC2400 DMA ���� DD �������ṹ��
*********************************************************************************************************/
typedef struct non_iso_dd_tag {
    UINT32 next_dd_addr;                                                /*  ��һ��DD��ַ                */
    UINT32 control;                                                     /*  DD������                    */
    UINT32 start_addr;                                                  /*  DMA ��������ʼ��ַ          */
    UINT32 status;                                                      /*  DMA ����״̬                */
} DD_DESCRIPTOR;

typedef struct __tagUsbStruct __CTRL_USB;                               /*  USB�����շ����ƽṹ������   */

/*********************************************************************************************************
    USB RAM ��С
*********************************************************************************************************/
#ifndef __USB_DMA_RAM_SIZE
#define __USB_DMA_RAM_SIZE      (8UL * 1024UL)                          /*  USB RAM size.Default 8KB    */
#endif
/*********************************************************************************************************
    ����USB RAM����128�ֽڶ���
*********************************************************************************************************/
#ifdef __USB_RAM_ALLOCATE
//__align(128)
#pragma data_alignment=128
static UINT8     __GpUsbDmaRam[__USB_DMA_RAM_SIZE];
#endif

/*********************************************************************************************************
    ���˵� DMA ��������С, �û��������޸�, ��ע���ܺͲ�Ҫ���� (__USB_DMA_RAM_SIZE - 640)Byte
*********************************************************************************************************/
#define DMA_BUF_SIZE_EP02       512                                     /*  ����˵�2 DMA��������С     */
#define DMA_BUF_SIZE_EP03       512                                     /*  ����˵�3 DMA��������С     */
#define DMA_BUF_SIZE_EP04       3072                                    /*  ����˵�4 DMA��������С     */
#define DMA_BUF_SIZE_EP05       3072                                    /*  ����˵�5 DMA��������С     */

/*********************************************************************************************************
    ���˵� DD ��ַ��������Ҫ����
*********************************************************************************************************/
#define __USB_RAM_ADDR          ((UINT32)__GpUsbDmaRam)                 /*  USB RAM ��ʼ��ַ            */

#define   DD_SIZE               sizeof(DD_DESCRIPTOR)                   /*  DD �ṹ���С               */

#define __DD_BASE_ADDR          (__USB_RAM_ADDR + (4 * 32))             /*  32Ϊ�˵�����                */
#define __DD_ADDR_EP02           __DD_BASE_ADDR                         /*  ����˵�2 DD��ַ            */
#define __DD_ADDR_EP03          (__DD_BASE_ADDR + (DD_SIZE * 1))        /*  ����˵�3 DD��ַ            */
#define __DD_ADDR_EP04          (__DD_BASE_ADDR + (DD_SIZE * 2))        /*  ����˵�4 DD��ַ            */
#define __DD_ADDR_EP05          (__DD_BASE_ADDR + (DD_SIZE * 3))        /*  ����˵�5 DD��ַ            */

#define DMA_BUF_ADDR_EP02       0                                       /*  ����˵�2 DMA��������Ե�ַ */
#define DMA_BUF_ADDR_EP03       (DMA_BUF_ADDR_EP02 + DMA_BUF_SIZE_EP02) /*  ����˵�3 DMA��������Ե�ַ */
#define DMA_BUF_ADDR_EP04       (DMA_BUF_ADDR_EP03 + DMA_BUF_SIZE_EP03) /*  ����˵�4 DMA��������Ե�ַ */
#define DMA_BUF_ADDR_EP05       (DMA_BUF_ADDR_EP04 + DMA_BUF_SIZE_EP04) /*  ����˵�5 DMA��������Ե�ַ */

#define __DMA_BUF_BASE_ADDR     (__DD_BASE_ADDR + (DD_SIZE * 32))
#define __DMA_BUF_ADDR_EP02     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP02)
#define __DMA_BUF_ADDR_EP03     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP03)
#define __DMA_BUF_ADDR_EP04     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP04)
#define __DMA_BUF_ADDR_EP05     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP05)

/*********************************************************************************************************
  DMA ״̬�궨��
*********************************************************************************************************/
#define DDS_NOT_SERV            0x00                                    /*  ָʾû�а�û����            */
#define DDS_BEING_SERV          0x01                                    /*  ָʾ������һ����������      */
#define DDS_UNDER_RUN           0x03                                    /*  ָʾ�յ��̰�����ֹ          */
#define DDS_SYS_ERR             0x09                                    /*  ָʾϵͳ���ߴ���            */
#define DDS_OVER_RUN            0x11                                    /*  ָʾ����������ĩβ����      */
#define DDS_NORMAL_COMP         0x05                                    /*  ָʾDMA������������         */


/*********************************************************************************************************
** Function name:       usbDevDMAInit
** Descriptions:        ��ʼ��LPC2400 USB�豸��������DMA����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void usbDevDMAInit (void);

/*********************************************************************************************************
** Function name:       usbDevDMAException
** Descriptions:        DMA �жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void usbDevDMAException (void);

/*********************************************************************************************************
** Function name:       usbDevDMAGetBuffer
** Descriptions:        ��ȡDMA�������׵�ַ
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ����˵�Ļ�����ָ��
*********************************************************************************************************/
extern UINT8* usbDevDMAGetBuffer (UINT8 ucEpIndex);

/*********************************************************************************************************
** Function name:       usbDevDMASetTransLength
** Descriptions:        ����DMA���䳤��
** input parameters:    ucEpIndex   ����˵�����
**                      ulSetlen    ���ô����ֽڳ���
** output parameters:   ��
** Returned value:      ���óɹ��Ĵ����ֽڳ���
*********************************************************************************************************/
extern UINT32 usbDevDMASetTransLength (UINT8 ucEpIndex, UINT32 ulSetlen);

/*********************************************************************************************************
** Function name:       usbDevDMAStartIN
** Descriptions:        ���� IN �˵�� DMA ����
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void usbDevDMAStartIN (UINT8 ucEpIndex);

#endif

#endif                                                                  /*  __USBDEVDMA_H               */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
