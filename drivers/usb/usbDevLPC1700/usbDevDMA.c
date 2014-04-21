/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDMA.c
** Latest modified Date:    2009-01-07
** Latest Version:          1.2
** Descriptions:            usbDevice �̼����� DMA ��Դ�ļ�
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
** Modified date:           2009-06-02
** Version:                 1.2
** Descriptions:            Modify for LPC1700
*********************************************************************************************************/
#define __USB_RAM_ALLOCATE                                              /*  ���붨�������������USB RAM */

#include "usbDevConfig.h"

#if DMA_ENGINE_EN > 0

/*********************************************************************************************************
** Function name:       __usbDevDMAInitUdcaTable
** Descriptions:        ��ʼ��DMA�����UDCAͷ�Ĵ�����UDCA��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevDMAInitUdcaTable (void)
{
    UINT32 *pulUDCA = NULL;

    __UDCAHead = __USB_RAM_ADDR;                                        /*  ��ʼ��UDCAͷ�Ĵ���          */
    pulUDCA    = (UINT32 *)__UDCAHead;                                  /*  ȡ��UDCA�Ĵ���ֵ            */

    pulUDCA[2] = (UINT32)__DD_ADDR_EP02;                                /*  ����UDCA��                  */
    pulUDCA[3] = (UINT32)__DD_ADDR_EP03;
    pulUDCA[4] = (UINT32)__DD_ADDR_EP04;
    pulUDCA[5] = (UINT32)__DD_ADDR_EP05;
}

/*********************************************************************************************************
** Function name:       __usbDevDMAGetDDPointer
** Descriptions:        ��ȡ�˵��DDָ��
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ָ��˵�DD��ָ��
*********************************************************************************************************/
DD_DESCRIPTOR *__usbDevDMAGetDDPointer (UINT8 ucEpIndex)
{
    UINT32 ulDdaddr = 0;

    ulDdaddr = __DD_BASE_ADDR + DD_SIZE * (ucEpIndex - 2);              /*  �����ַ                    */

    return (DD_DESCRIPTOR *)((UINT32 *)ulDdaddr);                       /*  ת��ָ������                */
}

/*********************************************************************************************************
** Function name:       __usbDevDMAInitEndpointDD
** Descriptions:        ��ʼ���˵��DD
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevDMAInitEndpointDD (UINT8 ucEpIndex)
{
    DD_DESCRIPTOR *pstDD = __usbDevDMAGetDDPointer(ucEpIndex);          /*  ȡ�� DD ָ��                */

    pstDD->next_dd_addr = 0;                                             /*  ����������Ա               */
    pstDD->status       = 0;

    switch(ucEpIndex) {

    case 0x02:
        /**
         *  д�˵����ݰ���С + дDMA��������С
         */
        pstDD->control    = (EP1_PACKET_SIZE << 5) + (DMA_BUF_SIZE_EP02 << 16);
        pstDD->start_addr = __DMA_BUF_ADDR_EP02;                        /*  дDMA��������ʼ��ַ         */
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
** Descriptions:        DMA��ʽ�˵㴫�����ݽ����жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevDMATransferEnd (void)
{
    UINT8  ucEpIndex = 0;
    unsigned long ulEpIntState = 0;
    DD_DESCRIPTOR *pstDD = NULL;

    ulEpIntState = __EOTIntSt;                                          /*  ����������жϼĴ���        */

    for (ucEpIndex = 2; ucEpIndex <= 5; ucEpIndex++) {

        if (ulEpIntState & ((UINT32)(0x01 << ucEpIndex))) {
                                                                        /*  �˵��жϴ������            */
            pstDD = __usbDevDMAGetDDPointer(ucEpIndex);

            if ((pstDD->status & 0x1F) == DDS_OVER_RUN) {               /*  DMA����                     */
                __EPDMAEn = (UINT32)(0x01 << ucEpIndex);                /*  ����ʹ�ܸö˵��DMA����     */
            }

            if ((pstDD->status & 0x1F) == DDS_NORMAL_COMP) {            /*  DMA��������                 */

                if (ucEpIndex == 2) {
                    bEPPflags.bits.ep1_rxdma = 1;                       /*  ��־�߼��˵�1 DMA���ճɹ�   */
                }

                if (ucEpIndex == 4) {
                    bEPPflags.bits.ep2_rxdma = 1;                       /*  ��־�߼��˵�2 DMA���ճɹ�   */
                }
            }

            __EOTIntClr |= (UINT32)(0x01 << ucEpIndex);                 /*  �����������ж�״̬�Ĵ���  */

            __usbDevDMAInitEndpointDD(ucEpIndex);                       /*  ���³�ʼ��DD                */

            if ((ucEpIndex % 2) != 0) {
                __EPIntEn |= (UINT32)(0x01 << ucEpIndex);               /*  ����ʹ�ܸö˵�ӻ��ж�      */
            }

        }
    }

}

/*********************************************************************************************************
** Function name:       __usbDevDMASystemError
** Descriptions:        DMA ϵͳ�жϴ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevDMASystemError (void)
{
    unsigned long ulEpstat   = 0;
    UINT8         ucEpIndex  = 0;

    ulEpstat = __SysErrIntSt;                                           /*  ��ȡϵͳ�����ж�״̬�Ĵ���  */

    for (ucEpIndex = 2; ucEpIndex < 5; ucEpIndex++) {

        if(ulEpstat & ((UINT32)(0x01 << ucEpIndex))) {

            __usbDevDMAInitEndpointDD(ucEpIndex);                       /*  ���³�ʼ��DD                */

            __EPDMAEn = (UINT32)(0x01 << ucEpIndex);                    /*  ����ʹ�ܸö˵��DMA����     */

            __SysErrIntClr |= (UINT32)(0x01 << ucEpIndex);              /*  ���ϵͳ�����ж�            */

            if ((ucEpIndex % 2) != 0) {

                __EPIntEn |= (UINT32)(0x01 << ucEpIndex);               /*  ����ʹ�ܸ� IN �˵�ӻ��ж�  */
            }
        }
    }
}

/*********************************************************************************************************
** Function name:       __usbDevDMANewDDRequest
** Descriptions:        DMA �� DD �����жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevDMANewDDRequest (void)
{
    unsigned long ulEpstat   = 0;
    UINT8         ucEpIndex  = 0;

    ulEpstat = __NDDReqIntSt;                                           /*  ��ȡ��DD�����ж�״̬�Ĵ���  */

    for (ucEpIndex = 2; ucEpIndex <= 5; ucEpIndex++) {

        if(ulEpstat & ((UINT32)(0x01 << ucEpIndex))) {

            __usbDevDMAInitEndpointDD(ucEpIndex);                       /*  �������øö˵��DD          */

            __NDDReqIntClr = (UINT32)(0x01 << ucEpIndex);               /*  �����DD�����ж�            */

            if ((ucEpIndex % 2) != 0) {

                __EPIntEn |= (0x01 << ucEpIndex);                       /*  ����ʹ�ܸö˵�ӻ��ж�      */
            }
        }
    }

}

/*********************************************************************************************************
** Function name:       usbDevDMAInit
** Descriptions:        ��ʼ��LPC23xx USB�豸��������DMA����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usbDevDMAInit (void)
{
    __usbDevDMAInitUdcaTable();                                         /*  ��ʼ��UDCA��                */

    __usbDevDMAInitEndpointDD(2);                                       /*  ��ʼ�����˵��DD            */
    __usbDevDMAInitEndpointDD(3);
    __usbDevDMAInitEndpointDD(4);
    __usbDevDMAInitEndpointDD(5);

    __DMAIntEn = USBDMA_EOT_INT |                                       /*  ʹ��DMA�Ĵ�������ж�       */
                 USBDMA_NDD_INT |                                       /*  ʹ��DMA����DD�����ж�       */
                 USBDMA_ERR_INT;                                        /*  ʹ��DMA��ϵͳ�����ж�       */

    __EPDMAEn  = (0x01 << 2) |                                          /*  ʹ�ܶ˵�DMA����             */
                 (0x01 << 3) |
                 (0x01 << 4) |
                 (0x01 << 5);
}
/*********************************************************************************************************
** Function name:       usbDevDMAException
** Descriptions:        DMA �жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usbDevDMAException (void)
{
    unsigned long ulDmastat = 0;

    ulDmastat = __DMAIntSt;                                             /*  ��DMA�ж�״̬�Ĵ���         */

    if (ulDmastat & USBDMA_EOT_INT) {

        __usbDevDMATransferEnd();                                       /*  ��������ж�                */
    }

    if (ulDmastat & USBDMA_NDD_INT) {

        __usbDevDMANewDDRequest();                                      /*  ��DD�����ж�                */
    }

    if (ulDmastat & USBDMA_ERR_INT) {

        __usbDevDMASystemError();                                       /*  ϵͳ�����ж�                */
    }
}
/*********************************************************************************************************
** Function name:       usbDevDMAGetBuffer
** Descriptions:        ��ȡDMA�������׵�ַ
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ����˵�Ļ�����ָ��
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
** Descriptions:        ����DMA���䳤��
** input parameters:    ucEpIndex   ����˵�����
**                      ulSetlen    ���ô����ֽڳ���
** output parameters:   ��
** Returned value:      ���óɹ��Ĵ����ֽڳ���
*********************************************************************************************************/
UINT32 usbDevDMASetTransLength (UINT8 ucEpIndex, UINT32 ulSetlen)
{
    DD_DESCRIPTOR *pDD = __usbDevDMAGetDDPointer(ucEpIndex);            /*  ȡ�� DD ָ��                */

    __usbDevDMAInitEndpointDD(ucEpIndex);                               /*  ��ʼ��DD                    */

	if (ulSetlen > (pDD->control >> 16)) {								/*  ���ȳ������˵�DMA������ 	*/
		ulSetlen = pDD->control >> 16;
	}
	pDD->control &= 0x0000FFFF;
	pDD->control |= (ulSetlen << 16);									/*  �����ó���д��DD	 		*/

    return ulSetlen;                                                    /*  �������óɹ����ֽڳ���      */
}

/*********************************************************************************************************
** Function name:       usbDevDMAStartIN
** Descriptions:        ���� IN �˵�� DMA ����
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usbDevDMAStartIN (UINT8 ucEpIndex)
{
    __EPDMAEn = (UINT32)(0x01 << ucEpIndex);                            /*  ʹ�ܶ˵��DMA����           */
    __EPIntEn &= (UINT32)(~(0x01 << ucEpIndex));                        /*  ����IN�˵㹤����DMAģʽ     */
}

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
