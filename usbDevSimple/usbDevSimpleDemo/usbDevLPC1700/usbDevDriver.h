/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevDriver.h
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC2400 �̼�����Ӧ�ò�ͷ�ļ�
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
*********************************************************************************************************/

#ifndef __USBDRIVER_H
#define __USBDRIVER_H

extern UINT8 GenEpBuf[];                                                /*  buffer of logic endpoint 1  */
extern UINT8 EpBuf[];                                                   /*  buffer of logic endpoint 2  */


/*********************************************************************************************************
  USB �¼���־�ṹ��
*********************************************************************************************************/

struct __stflags {

    unsigned bus_reset              : 1;    //���߸�λ
    unsigned suspend                : 1;    //���߹���ı�
    unsigned con_change             : 1;    //�������Ӹı�
    unsigned setup_packet           : 1;    //���յ�SETUP��
    unsigned remote_wakeup          : 1;    //Զ�̻��ѱ�־
    unsigned in_isr                 : 1;    //����USB�豸�жϷ�������־
    unsigned control_state          : 2;    //���ƶ˵㴦��״̬
                                                //0: ����״̬
                                                //1: ���ݷ���״̬
                                                //2: ���ݽ���״̬
    unsigned configuration          : 1;    //USB�豸������ɱ�־(0:δ����;1:������)
    unsigned command                : 1;    //δʹ��
    unsigned ep1_rxdone             : 1;    //�߼��˵�1���յ����ݱ�־
    unsigned ep2_rxdone             : 1;    //�߼��˵�2���յ����ݱ�־
    unsigned ep1_rxdma              : 1;    //�߼��˵�1(DMA)���յ����ݱ�־
    unsigned ep2_rxdma              : 1;    //�߼��˵�2(DMA)���յ����ݱ�־
};

typedef struct __stflags    __FLAGS;                                    /*  �ض����־λ�ṹ������      */

union __unflags {

    __FLAGS bits;
    UINT16 value;
};

typedef union __unflags EPPFLAGS;                                       /*  �ض����־λ����������      */

extern EPPFLAGS bEPPflags;                                              /*  �����������־λ����        */

/*********************************************************************************************************
  ����Ϊ���û����ŵ� API ����
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       usbDevInit
** Descriptions:        ��ʼ�� USB �豸������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��ʼ���ɹ�  0
**                      ��ʼ��ʧ��  ������
*********************************************************************************************************/
extern UINT8 usbDevInit (void);

/*********************************************************************************************************
** Function name:       usbDevServe
** Descriptions:        USB �豸�������¼�������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void usbDevServe (void);

/*********************************************************************************************************
** Function name:       usbDevException
** Descriptions:        USB �жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void usbDevException (void);

/*********************************************************************************************************
** Function name:       usbDevGetVer
** Descriptions:        �õ� USB Device ������汾��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ZLG/USB23xx ������汾��
*********************************************************************************************************/
extern UINT16 usbDevGetVer(void);

#endif                                                                  /*  __USBDRIVER_H               */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
