/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevConfig.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC2400 固件程序配置头文件
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
  定义与编译器无关的数据类型
*********************************************************************************************************/
typedef unsigned char           BOOL;                                   /*  布尔变量                    */
typedef unsigned char           UINT8;                                  /*  无符号8位整型变量           */
typedef signed   char           SINT8;                                  /*  有符号8位整型变量           */
typedef unsigned short          UINT16;                                 /*  无符号16位整型变量          */
typedef signed   short          SINT16;                                 /*  有符号16位整型变量          */
typedef unsigned long           UINT32;                                 /*  无符号32位整型变量          */
typedef signed   long           SINT32;                                 /*  有符号32位整型变量          */
#if 0
typedef float                   FP32;                                   /*  单精度浮点数（32位长度）    */
typedef double                  FP64;                                   /*  双精度浮点数（64位长度）    */
#endif

/*********************************************************************************************************
  IRQ 中断开关宏定义
*********************************************************************************************************/
#define USB_ENTER_CRITICAL_SECTION()        zyIrqDisable()
#define USB_EXIT_CRITICAL_SECTION()         zyIrqEnable()

/*********************************************************************************************************
  用户配置宏定义
*********************************************************************************************************/
#define DMA_ENGINE_EN           0                                       /*  是否使能USB Device DMA引擎  */
#define ATX_ISP1301_EN          0                                       /*  是否使用外部ATX ISP1301     */

/*********************************************************************************************************
  添加相关头文件
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
