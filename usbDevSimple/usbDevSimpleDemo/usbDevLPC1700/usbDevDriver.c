/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDriver.c
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 �̼�����Ӧ�ò�Դ�ļ�
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
** Descriptions:            �޸Ĳ��ִ������ʺ�SmartARM3250ϵ�п�����
**--------------------------------------------------------------------------------------------------------
** Modified by:             LiuWeiyun
** Modified date:           2009-01-05
** Version:                 1.2
** Descriptions:            LPC3200��ֲ
*********************************************************************************************************/
#include "usbDevConfig.h"

UINT8 GenEpBuf[EP1_PACKET_SIZE];                                        /*  �߼��˵� 1 ���ջ�����       */
UINT8 EpBuf[EP2_PACKET_SIZE];                                           /*  �߼��˵� 2 ���ջ�����       */

EPPFLAGS bEPPflags;                                                     /*  ���� USB �¼���־����       */

/*********************************************************************************************************
** Function name:       __usbDevConfigEndpoint
** Descriptions:        ���ö˵��������С
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevConfigEndpoint (void)
{
    __usbDevConfigMaxPaketSize(0,EP0_PACKET_SIZE);                      /*  ���ø�����˵��������С  */
    __usbDevConfigMaxPaketSize(1,EP0_PACKET_SIZE);

    __usbDevConfigMaxPaketSize(2,EP1_PACKET_SIZE);
    __usbDevConfigMaxPaketSize(3,EP1_PACKET_SIZE);

    __usbDevConfigMaxPaketSize(4,EP2_PACKET_SIZE);
    __usbDevConfigMaxPaketSize(5,EP2_PACKET_SIZE);
}
/*********************************************************************************************************
** Function name:       __usbDevEp1TxDone
** Descriptions:        �߼��˵�1�������ݴ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevEp1TxDone (void)
{
    __usbDevSelectClrIntEndpoint(3);                                    /*  ѡ��˵㲢����ж�          */
}
/*********************************************************************************************************
** Function name:       __usbDevEp1RxDone
** Descriptions:        �߼��˵�1�������ݴ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevEp1RxDone (void)
{
    UINT8 len;

    __usbDevSelectClrIntEndpoint(2);                                    /*  ѡ��˵㲢����ж�          */

    len = usbDevReadEndpoint(2,GenEpBuf,sizeof(GenEpBuf));              /*  �ӽ��ջ������ж�������      */

    if(len != 0) {
        bEPPflags.bits.ep1_rxdone = 1;                                  /*  ��ʶ�ö˵��յ�����          */
    }
}
/*********************************************************************************************************
** Function name:       __usbDevEp2TxDone
** Descriptions:        �߼��˵�2�������ݴ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevEp2TxDone (void)
{
    __usbDevSelectClrIntEndpoint(5);                                    /*  ѡ��˵㲢����ж�          */

}
/*********************************************************************************************************
** Function name:       __usbDevEp2RxDone
** Descriptions:        �߼��˵�2�������ݴ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevEp2RxDone (void)
{
    UINT8 len;

    __usbDevSelectClrIntEndpoint(4);                                    /*  ѡ��˵㲢����ж�          */

    len = usbDevReadEndpoint(4,GenEpBuf,sizeof(GenEpBuf));
    if(len != 0)
        bEPPflags.bits.ep2_rxdone = 1;                                  /* ��ʶ�ö˵��յ�����           */
}
/*********************************************************************************************************
** Function name:       __usbDevEpIntHandler
** Descriptions:        USB �˵��жϴ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevEpIntHandler (void)
{
    UINT32 epSt;

    epSt = (UINT32)__EPIntSt;                                           /*  ���˵��ж�״̬�Ĵ���        */

    if(epSt & USB_ENDP00) {
        __usbDevEp0RxDone();                                            /*  ���ƶ˵�������ݴ���        */
    }
    if(epSt & USB_ENDP01) {
        __usbDevEp0TxDone();                                            /*  ���ƶ˵㷢�����ݴ���        */
    }
    if(epSt & USB_ENDP02) {
        __usbDevEp1RxDone();                                            /*  �߼��˵�1�������ݴ���       */
    }
    if(epSt & USB_ENDP03) {
        __usbDevEp1TxDone();                                            /*  �߼��˵�1�������ݴ���       */
    }
    if(epSt & USB_ENDP04) {
        __usbDevEp2RxDone();                                            /*  �߼��˵�2�������ݴ���       */
    }
    if(epSt & USB_ENDP05) {
        __usbDevEp2TxDone();                                            /*  �߼��˵�2�������ݴ���       */
    }
}
/*********************************************************************************************************
** Function name:       __usbDevBusReset
** Descriptions:        USB ���߸�λ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevBusReset (void)
{
    __usbDevIntConfig();
    __usbDevConfigEndpoint();                                           /*  �����������ж˵�������С  */
}

/*********************************************************************************************************
** Function name:       __usbDevSuspendChange
** Descriptions:        USB ���߹���ı�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSuspendChange (void)
{

}

/*********************************************************************************************************
** Function name:       _usbConnectChange
** Descriptions:        USB �������Ӹı�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevConnectChange (void)
{

}

/*********************************************************************************************************
** Function name:       usbDevInit
** Descriptions:        ��ʼ�� USB �豸������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��ʼ���ɹ�  0
**                      ��ʼ��ʧ��  ������
*********************************************************************************************************/
UINT8 usbDevInit (void)
{
    __usbDevHardwareInit();                                             /*  ��ʼ��Ӳ��                  */

    bEPPflags.value = 0;                                                /*  ��USB�¼���־Ϊ0            */

    if (__usbDevReadTestRegister() != 0xA50F) {                         /*  �����ԼĴ���                */
        return 1;                                                       /*  ��ʼ��ʧ��                  */
    }

    __usbDevIntConfig();                                                /*  �����ж�                    */
    __usbDevConfigEndpoint();                                           /*  ���ö˵�                    */

#if DMA_ENGINE_EN > 0
    usbDevDMAInit();                                                    /*  ��ʼ��USB DMA               */
    __usbDevSetMode(0x28);
#else
    __usbDevSetMode(0x00);                                              /*  �ɹ�����Ų����ж�          */
#endif

    //__usbDevSetAddressEnable(0, 1);

    __usbDevHadwareReconnect();
    __usbDevSoftReconnect();                                            /*  ��������USB                 */

    return (0);                                                         /*  ��ʼ��USB�ɹ�               */
}

/*********************************************************************************************************
** Function name:       usbDevServe
** Descriptions:        USB �豸�������¼�������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usbDevServe (void)
{
    if (bEPPflags.bits.setup_packet) {
                                                                        /*  �յ� SETUP ��               */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.setup_packet = 0;
        __usbDevControlHandler();                                       /*  ���ƴ��䴦��                */
        USB_EXIT_CRITICAL_SECTION();
    }

    if (bEPPflags.bits.bus_reset) {
                                                                        /*  USB���߸�λ����             */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.bus_reset = 0;
        USB_EXIT_CRITICAL_SECTION();
    }

    if (bEPPflags.bits.suspend) {
                                                                        /*  USB���߹���ı䴦��         */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.suspend= 0;
        USB_EXIT_CRITICAL_SECTION();
        __usbDevSuspendChange();
    }

    if (bEPPflags.bits.con_change) {
                                                                        /*  USB�������Ӹı䴦��         */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.con_change = 0;
        USB_EXIT_CRITICAL_SECTION();
        __usbDevConnectChange();
    }
}

/*********************************************************************************************************
** Function name:       usbDevException
** Descriptions:        USB �жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void  usbDevException (void)
{
    UINT32 ulDevIntState = 0;
    UINT32 ulDevState    = 0;

    bEPPflags.bits.in_isr = 1;                                          /*  ��ʶ��������ж�            */

    ulDevIntState = (UINT32)__DevIntSt;                                 /*  ���豸�ж�״̬�Ĵ���        */

    if (ulDevIntState & DEVINT_STATUS_DEVSTAT) {

        ulDevState = __usbDevGetDevStatus();

        if ((ulDevState & 0x10) != 0) {
            __usbDevBusReset();                                         /*  ���߸�λ����                */
            bEPPflags.bits.bus_reset = 1;                               /*  ��ʶ���߸�λ                */
        }

        if ((ulDevState & 0x08) != 0) {
            bEPPflags.bits.suspend = 1;                                 /*  ��ʶ���߹���ı�            */
        }

        if ((ulDevState & 0x02) != 0) {
            bEPPflags.bits.con_change = 1;                              /*  ��ʶ�������Ӹı�            */
        }

        __DevIntClr = DevStatusInterrupt;
        goto USB_ISR_EXIT;                                              /*  �˳��жϷ������           */
    }

    if (ulDevIntState & DEVINT_STATUS_EP_FAST) {
        __usbDevEpIntHandler();
        __DevIntClr = FASTINTERRUPT;
    }

    if (ulDevIntState & DEVINT_STATUS_EP_SLOW) {
        __usbDevEpIntHandler();
        __DevIntClr = SLOWINTERRUPT;
    }

#if DMA_ENGINE_EN
    if (__USBIntSt & USBINT_STATUS_DMA) {
        usbDevDMAException();
    }
#endif

USB_ISR_EXIT:
    bEPPflags.bits.in_isr = 0;                                          /*  ��ʶ�����˳��ж�            */
}

/*********************************************************************************************************
** Function name:       usbDevGetVer
** Descriptions:        �õ� USB Device ������汾��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ������汾��
*********************************************************************************************************/
UINT16 usbDevGetVer(void)
{
    return 0x0113;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
