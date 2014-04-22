/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevConfig.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC2400 �̼���������ͷ�ļ�
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            �޸Ĳ��ִ������ʺ�SmartARM2400ϵ�п�����
**--------------------------------------------------------------------------------------------------------
** Modified by:             LiuWeiyun
** Modified date:           2009-01-04
** Version:                 1.2
** Descriptions:            Define "TRUE" and "FALSE"
*********************************************************************************************************/

#ifndef __USBDEVCONFIG_H
#define __USBDEVCONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE  (1 == 1)
#endif                                                                  /*  TRUE                        */

#ifndef FALSE
#define FALSE (1 == 0)
#endif                                                                  /*  FALSE                       */

/*********************************************************************************************************
  ������������޹ص���������
*********************************************************************************************************/
typedef unsigned char           BOOL;                                   /*  ��������                    */
typedef unsigned char           UINT8;                                  /*  �޷���8λ���ͱ���           */
typedef signed   char           SINT8;                                  /*  �з���8λ���ͱ���           */
typedef unsigned short          UINT16;                                 /*  �޷���16λ���ͱ���          */
typedef signed   short          SINT16;                                 /*  �з���16λ���ͱ���          */
typedef unsigned long           UINT32;                                 /*  �޷���32λ���ͱ���          */
typedef signed   long           SINT32;                                 /*  �з���32λ���ͱ���          */
#if 0
typedef float                   FP32;                                   /*  �����ȸ�������32λ���ȣ�    */
typedef double                  FP64;                                   /*  ˫���ȸ�������64λ���ȣ�    */
#endif

/*********************************************************************************************************
  IRQ �жϿ��غ궨��
*********************************************************************************************************/
#define USB_ENTER_CRITICAL_SECTION()        zyIrqDisable()
#define USB_EXIT_CRITICAL_SECTION()         zyIrqEnable()

/*********************************************************************************************************
  �û����ú궨��
*********************************************************************************************************/
#define DMA_ENGINE_EN           0                                       /*  �Ƿ�ʹ��USB Device DMA����  */
#define ATX_ISP1301_EN          0                                       /*  �Ƿ�ʹ���ⲿATX ISP1301     */

/*********************************************************************************************************
  ������ͷ�ļ�
*********************************************************************************************************/
//#include "intrinsics.h"

#include "usbReg.h"
#include "isp1301.h"
#include "usbDevHAL.h"
#include "usbDevCI.h"
#include "usbDevDescriptor.h"
#include "usbDevProtocol.h"
#include "usbDevDriver.h"
#include "usbDevDMA.h"

#include "..\config.h"

#ifdef __cplusplus
    }
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __USBDEVCONFIG_H            */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
