/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDMA.c
** Latest modified Date:    2009-01-07
** Latest Version:          1.2
** Descriptions:            usbDevice 固件程序 DMA 层源文件
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
** Modified date:           2009-06-02
** Version:                 1.2
** Descriptions:            Modify for LPC1700
*********************************************************************************************************/
#define __USB_RAM_ALLOCATE                                              /*  必须定义这个宏来分配USB RAM */

#include "usbDevConfig.h"

#if DMA_ENGINE_EN > 0

/*********************************************************************************************************
** Function name:       __usbDevDMAInitUdcaTable
** Descriptions:        初始化DMA引擎的UDCA头寄存器和UDCA表
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevDMAInitUdcaTable (void)
{
    UINT32 *pulUDCA = NULL;

    __UDCAHead = __USB_RAM_ADDR;                                        /*  初始化UDCA头寄存器          */
    pulUDCA    = (UINT32 *)__UDCAHead;                                  /*  取出UDCA寄存器值            */

    pulUDCA[2] = (UINT32)__DD_ADDR_EP02;                                /*  建立UDCA表                  */
    pulUDCA[3] = (UINT32)__DD_ADDR_EP03;
    pulUDCA[4] = (UINT32)__DD_ADDR_EP04;
    pulUDCA[5] = (UINT32)__DD_ADDR_EP05;
}

/*********************************************************************************************************
** Function name:       __usbDevDMAGetDDPointer
** Descriptions:        获取端点的DD指针
** input parameters:    ucEpIndex   物理端点索引
** output parameters:   无
** Returned value:      指向端点DD的指针
*********************************************************************************************************/
DD_DESCRIPTOR *__usbDevDMAGetDDPointer (UINT8 ucEpIndex)
{
    UINT32 ulDdaddr = 0;

    ulDdaddr = __DD_BASE_ADDR + DD_SIZE * (ucEpIndex - 2);              /*  计算地址                    */

    return (DD_DESCRIPTOR *)((UINT32 *)ulDdaddr);                       /*  转换指针类型                */
}

/*********************************************************************************************************
** Function name:       __usbDevDMAInitEndpointDD
** Descriptions:        初始化端点的DD
** input parameters:    ucEpIndex   物理端点索引
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevDMAInitEndpointDD (UINT8 ucEpIndex)
{
    DD_DESCRIPTOR *pstDD = __usbDevDMAGetDDPointer(ucEpIndex);          /*  取得 DD 指针                */

    pstDD->next_dd_addr = 0;                                             /*  清零两个成员               */
    pstDD->status       = 0;

    switch(ucEpIndex) {

    case 0x02:
        /**
         *  写端点数据包大小 + 写DMA缓冲区大小
         */
        pstDD->control    = (EP1_PACKET_SIZE << 5) + (DMA_BUF_SIZE_EP02 << 16);
        pstDD->start_addr = __DMA_BUF_ADDR_EP02;                        /*  写DMA缓冲区起始地址         */
        break;

    case 0x03:
        pstDD->control    = (EP1_PACKET_SIZE << 5) + (DMA_BUF_SIZE_EP03 << 16);
        pstDD->start_addr = __DMA_BUF_ADDR_EP03;
        break;

    case 0x04:
        pstDD->control    = (EP2_PACKET_SIZE << 5) + (DMA_BUF_SIZE_EP04 << 16);
        pstDD->start_addr = __DMA_BUF_ADDR_EP04;
        break;

    case 0x05:
        pstDD->control    = (EP2_PACKET_SIZE << 5) + (DMA_BUF_SIZE_EP05 << 16);
        pstDD->start_addr = __DMA_BUF_ADDR_EP05;
        break;

    default:
        break;
    }
}

/*********************************************************************************************************
** Function name:       __usbDevDMATransferEnd
** Descriptions:        DMA方式端点传输数据结束中断服务程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevDMATransferEnd (void)
{
    UINT8  ucEpIndex = 0;
    unsigned long ulEpIntState = 0;
    DD_DESCRIPTOR *pstDD = NULL;

    ulEpIntState = __EOTIntSt;                                          /*  读传输结束中断寄存器        */

    for (ucEpIndex = 2; ucEpIndex <= 5; ucEpIndex++) {

        if (ulEpIntState & ((UINT32)(0x01 << ucEpIndex))) {
                                                                        /*  端点中断传输结束            */
            pstDD = __usbDevDMAGetDDPointer(ucEpIndex);

            if ((pstDD->status & 0x1F) == DDS_OVER_RUN) {               /*  DMA错误                     */
                __EPDMAEn = (UINT32)(0x01 << ucEpIndex);                /*  重新使能该端点的DMA功能     */
            }

            if ((pstDD->status & 0x1F) == DDS_NORMAL_COMP) {            /*  DMA正常结束                 */

                if (ucEpIndex == 2) {
                    bEPPflags.bits.ep1_rxdma = 1;                       /*  标志逻辑端点1 DMA接收成功   */
                }

                if (ucEpIndex == 4) {
                    bEPPflags.bits.ep2_rxdma = 1;                       /*  标志逻辑端点2 DMA接收成功   */
                }
            }

            __EOTIntClr |= (UINT32)(0x01 << ucEpIndex);                 /*  清除传输结束中断状态寄存器  */

            __usbDevDMAInitEndpointDD(ucEpIndex);                       /*  重新初始化DD                */

            if ((ucEpIndex % 2) != 0) {
                __EPIntEn |= (UINT32)(0x01 << ucEpIndex);               /*  重新使能该端点从机中断      */
            }

        }
    }

}

/*********************************************************************************************************
** Function name:       __usbDevDMASystemError
** Descriptions:        DMA 系统中断错误处理
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevDMASystemError (void)
{
    unsigned long ulEpstat   = 0;
    UINT8         ucEpIndex  = 0;

    ulEpstat = __SysErrIntSt;                                           /*  读取系统错误中断状态寄存器  */

    for (ucEpIndex = 2; ucEpIndex < 5; ucEpIndex++) {

        if(ulEpstat & ((UINT32)(0x01 << ucEpIndex))) {

            __usbDevDMAInitEndpointDD(ucEpIndex);                       /*  重新初始化DD                */

            __EPDMAEn = (UINT32)(0x01 << ucEpIndex);                    /*  重新使能该端点的DMA功能     */

            __SysErrIntClr |= (UINT32)(0x01 << ucEpIndex);              /*  清除系统错误中断            */

            if ((ucEpIndex % 2) != 0) {

                __EPIntEn |= (UINT32)(0x01 << ucEpIndex);               /*  重新使能该 IN 端点从机中断  */
            }
        }
    }
}

/*********************************************************************************************************
** Function name:       __usbDevDMANewDDRequest
** Descriptions:        DMA 新 DD 请求中断服务程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevDMANewDDRequest (void)
{
    unsigned long ulEpstat   = 0;
    UINT8         ucEpIndex  = 0;

    ulEpstat = __NDDReqIntSt;                                           /*  读取新DD请求中断状态寄存器  */

    for (ucEpIndex = 2; ucEpIndex <= 5; ucEpIndex++) {

        if(ulEpstat & ((UINT32)(0x01 << ucEpIndex))) {

            __usbDevDMAInitEndpointDD(ucEpIndex);                       /*  重新设置该端点的DD          */

            __NDDReqIntClr = (UINT32)(0x01 << ucEpIndex);               /*  清除新DD请求中断            */

            if ((ucEpIndex % 2) != 0) {

                __EPIntEn |= (0x01 << ucEpIndex);                       /*  重新使能该端点从机中断      */
            }
        }
    }

}

/*********************************************************************************************************
** Function name:       usbDevDMAInit
** Descriptions:        初始化LPC23xx USB设备控制器的DMA引擎
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void usbDevDMAInit (void)
{
    __usbDevDMAInitUdcaTable();                                         /*  初始化UDCA表                */

    __usbDevDMAInitEndpointDD(2);                                       /*  初始化各端点的DD            */
    __usbDevDMAInitEndpointDD(3);
    __usbDevDMAInitEndpointDD(4);
    __usbDevDMAInitEndpointDD(5);

    __DMAIntEn = USBDMA_EOT_INT |                                       /*  使能DMA的传输结束中断       */
                 USBDMA_NDD_INT |                                       /*  使能DMA的新DD请求中断       */
                 USBDMA_ERR_INT;                                        /*  使能DMA的系统错误中断       */

    __EPDMAEn  = (0x01 << 2) |                                          /*  使能端点DMA功能             */
                 (0x01 << 3) |
                 (0x01 << 4) |
                 (0x01 << 5);
}
/*********************************************************************************************************
** Function name:       usbDevDMAException
** Descriptions:        DMA 中断服务程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void usbDevDMAException (void)
{
    unsigned long ulDmastat = 0;

    ulDmastat = __DMAIntSt;                                             /*  读DMA中断状态寄存器         */

    if (ulDmastat & USBDMA_EOT_INT) {

        __usbDevDMATransferEnd();                                       /*  传输结束中断                */
    }

    if (ulDmastat & USBDMA_NDD_INT) {

        __usbDevDMANewDDRequest();                                      /*  新DD请求中断                */
    }

    if (ulDmastat & USBDMA_ERR_INT) {

        __usbDevDMASystemError();                                       /*  系统错误中断                */
    }
}
/*********************************************************************************************************
** Function name:       usbDevDMAGetBuffer
** Descriptions:        获取DMA缓冲区首地址
** input parameters:    ucEpIndex   物理端点索引
** output parameters:   无
** Returned value:      物理端点的缓冲区指针
*********************************************************************************************************/
UINT8 *usbDevDMAGetBuffer (UINT8 ucEpIndex)
{
    UINT8 *p;

    switch (ucEpIndex) {

    case 2:
        p = ((UINT8 *)(__DMA_BUF_ADDR_EP02));
        break;

    case 3:
        p = ((UINT8 *)(__DMA_BUF_ADDR_EP03));
        break;

    case 4:
        p = ((UINT8 *)(__DMA_BUF_ADDR_EP04));
        break;

    case 5:
        p = ((UINT8 *)(__DMA_BUF_ADDR_EP05));
        break;

    default:
        p = NULL;
        break;
    }

    return (p);
}

/*********************************************************************************************************
** Function name:       usbDevDMASetTransLength
** Descriptions:        设置DMA传输长度
** input parameters:    ucEpIndex   物理端点索引
**                      ulSetlen    设置传输字节长度
** output parameters:   无
** Returned value:      设置成功的传输字节长度
*********************************************************************************************************/
UINT32 usbDevDMASetTransLength (UINT8 ucEpIndex, UINT32 ulSetlen)
{
    DD_DESCRIPTOR *pDD = __usbDevDMAGetDDPointer(ucEpIndex);            /*  取得 DD 指针                */

    __usbDevDMAInitEndpointDD(ucEpIndex);                               /*  初始化DD                    */

	if (ulSetlen > (pDD->control >> 16)) {								/*  长度超过本端点DMA缓冲区 	*/
		ulSetlen = pDD->control >> 16;
	}
	pDD->control &= 0x0000FFFF;
	pDD->control |= (ulSetlen << 16);									/*  将设置长度写入DD	 		*/

    return ulSetlen;                                                    /*  返回设置成功的字节长度      */
}

/*********************************************************************************************************
** Function name:       usbDevDMAStartIN
** Descriptions:        启动 IN 端点的 DMA 传输
** input parameters:    ucEpIndex   物理端点索引
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void usbDevDMAStartIN (UINT8 ucEpIndex)
{
    __EPDMAEn = (UINT32)(0x01 << ucEpIndex);                            /*  使能端点的DMA功能           */
    __EPIntEn &= (UINT32)(~(0x01 << ucEpIndex));                        /*  设置IN端点工作在DMA模式     */
}

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
