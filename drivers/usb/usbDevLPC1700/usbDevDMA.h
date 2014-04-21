/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDMA.h
** Latest modified Date:    2009-01-08
** Latest Version:          1.2
** Descriptions:            usbDevice 固件程序 DMA 层头文件
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            处理传输短包数据的bug
**                          修改部分代码以符合代码规范v1.2
**--------------------------------------------------------------------------------------------------------
** Modified by:             LiuWeiyun
** Modified date:           2009-01-07
** Version:                 1.2
** Descriptions:            移植到LPC3200
** NOTE                     (1) UDCA 头指针需128字节对齐
**                          (2) LPC3200用于DMA的USB RAM需自行在内部RAM或外部RAM(若有的话)中开辟
**                              且地址需字(4字节)对齐
**                          (3) 为简化操作，将USB RAM也进行128字节对齐来兼容UDCA 头指针需128字节对齐
*********************************************************************************************************/

#ifndef __USBDEVDMA_H
#define __USBDEVDMA_H

#if DMA_ENGINE_EN > 0

/*********************************************************************************************************
  LPC2400 DMA 引擎 DD 描述符结构体
*********************************************************************************************************/
typedef struct non_iso_dd_tag {
    UINT32 next_dd_addr;                                                /*  下一个DD地址                */
    UINT32 control;                                                     /*  DD控制字                    */
    UINT32 start_addr;                                                  /*  DMA 缓冲区起始地址          */
    UINT32 status;                                                      /*  DMA 引擎状态                */
} DD_DESCRIPTOR;

typedef struct __tagUsbStruct __CTRL_USB;                               /*  USB数据收发控制结构体类型   */

/*********************************************************************************************************
    USB RAM 大小
*********************************************************************************************************/
#ifndef __USB_DMA_RAM_SIZE
#define __USB_DMA_RAM_SIZE      (8UL * 1024UL)                          /*  USB RAM size.Default 8KB    */
#endif
/*********************************************************************************************************
    开辟USB RAM，需128字节对齐
*********************************************************************************************************/
#ifdef __USB_RAM_ALLOCATE
//__align(128)
#pragma data_alignment=128
static UINT8     __GpUsbDmaRam[__USB_DMA_RAM_SIZE];
#endif

/*********************************************************************************************************
    各端点 DMA 缓冲区大小, 用户可自行修改, 请注意总和不要大于 (__USB_DMA_RAM_SIZE - 640)Byte
*********************************************************************************************************/
#define DMA_BUF_SIZE_EP02       512                                     /*  物理端点2 DMA缓冲区大小     */
#define DMA_BUF_SIZE_EP03       512                                     /*  物理端点3 DMA缓冲区大小     */
#define DMA_BUF_SIZE_EP04       3072                                    /*  物理端点4 DMA缓冲区大小     */
#define DMA_BUF_SIZE_EP05       3072                                    /*  物理端点5 DMA缓冲区大小     */

/*********************************************************************************************************
    各端点 DD 地址，根据需要增加
*********************************************************************************************************/
#define __USB_RAM_ADDR          ((UINT32)__GpUsbDmaRam)                 /*  USB RAM 起始地址            */

#define   DD_SIZE               sizeof(DD_DESCRIPTOR)                   /*  DD 结构体大小               */

#define __DD_BASE_ADDR          (__USB_RAM_ADDR + (4 * 32))             /*  32为端点总数                */
#define __DD_ADDR_EP02           __DD_BASE_ADDR                         /*  物理端点2 DD地址            */
#define __DD_ADDR_EP03          (__DD_BASE_ADDR + (DD_SIZE * 1))        /*  物理端点3 DD地址            */
#define __DD_ADDR_EP04          (__DD_BASE_ADDR + (DD_SIZE * 2))        /*  物理端点4 DD地址            */
#define __DD_ADDR_EP05          (__DD_BASE_ADDR + (DD_SIZE * 3))        /*  物理端点5 DD地址            */

#define DMA_BUF_ADDR_EP02       0                                       /*  物理端点2 DMA缓冲区相对地址 */
#define DMA_BUF_ADDR_EP03       (DMA_BUF_ADDR_EP02 + DMA_BUF_SIZE_EP02) /*  物理端点3 DMA缓冲区相对地址 */
#define DMA_BUF_ADDR_EP04       (DMA_BUF_ADDR_EP03 + DMA_BUF_SIZE_EP03) /*  物理端点4 DMA缓冲区相对地址 */
#define DMA_BUF_ADDR_EP05       (DMA_BUF_ADDR_EP04 + DMA_BUF_SIZE_EP04) /*  物理端点5 DMA缓冲区相对地址 */

#define __DMA_BUF_BASE_ADDR     (__DD_BASE_ADDR + (DD_SIZE * 32))
#define __DMA_BUF_ADDR_EP02     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP02)
#define __DMA_BUF_ADDR_EP03     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP03)
#define __DMA_BUF_ADDR_EP04     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP04)
#define __DMA_BUF_ADDR_EP05     (__DMA_BUF_BASE_ADDR + DMA_BUF_ADDR_EP05)

/*********************************************************************************************************
  DMA 状态宏定义
*********************************************************************************************************/
#define DDS_NOT_SERV            0x00                                    /*  指示没有包没传输            */
#define DDS_BEING_SERV          0x01                                    /*  指示至少有一个包被传输      */
#define DDS_UNDER_RUN           0x03                                    /*  指示收到短包而中止          */
#define DDS_SYS_ERR             0x09                                    /*  指示系统总线错误            */
#define DDS_OVER_RUN            0x11                                    /*  指示缓冲区到达末尾错误      */
#define DDS_NORMAL_COMP         0x05                                    /*  指示DMA传输正常结束         */


/*********************************************************************************************************
** Function name:       usbDevDMAInit
** Descriptions:        初始化LPC2400 USB设备控制器的DMA引擎
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usbDevDMAInit (void);

/*********************************************************************************************************
** Function name:       usbDevDMAException
** Descriptions:        DMA 中断服务程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usbDevDMAException (void);

/*********************************************************************************************************
** Function name:       usbDevDMAGetBuffer
** Descriptions:        获取DMA缓冲区首地址
** input parameters:    ucEpIndex   物理端点索引
** output parameters:   无
** Returned value:      物理端点的缓冲区指针
*********************************************************************************************************/
extern UINT8* usbDevDMAGetBuffer (UINT8 ucEpIndex);

/*********************************************************************************************************
** Function name:       usbDevDMASetTransLength
** Descriptions:        设置DMA传输长度
** input parameters:    ucEpIndex   物理端点索引
**                      ulSetlen    设置传输字节长度
** output parameters:   无
** Returned value:      设置成功的传输字节长度
*********************************************************************************************************/
extern UINT32 usbDevDMASetTransLength (UINT8 ucEpIndex, UINT32 ulSetlen);

/*********************************************************************************************************
** Function name:       usbDevDMAStartIN
** Descriptions:        启动 IN 端点的 DMA 传输
** input parameters:    ucEpIndex   物理端点索引
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usbDevDMAStartIN (UINT8 ucEpIndex);

#endif

#endif                                                                  /*  __USBDEVDMA_H               */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
