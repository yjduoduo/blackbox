/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2009-05-12
** Last Version:        V1.01
** Descriptions:        The main() function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Chengmingji
** Created date:        2009-07-24
** Version:             V1.00
** Descriptions:        ����û�Ӧ�ó���
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         LiuWeiyun
** Modified date:       2009-07-27
** Version:             V1.01
** Descriptions:        ��дusbDevSimple����
**
** Rechecked by:        Zhang Ningbo
*********************************************************************************************************/
#include <string.h>
#include "..\usbDevLPC1700\usbDevConfig.h"
#include "..\config.h"

/*********************************************************************************************************
  ������궨��
*********************************************************************************************************/
#define     BEEP           (1ul << 7)                                   /*  ������P0.7                  */
#define     KEY1           (1ul << 10)                                  /*  P2.10����KEY1               */
#define     BEEP_INIT()     FIO0DIR |= BEEP                             /*  �������������              */
#define     BEEP_OFF()      FIO0SET = BEEP                              /*  ��������                    */
#define     BEEP_ON()       FIO0CLR = BEEP                              /*  ��������                    */
/*********************************************************************************************************
** Function name:       usbDeviceDemo
** Descriptions:        �򵥵����ݴ���
** Input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void  usbDeviceDemo (void)
{

    zyIsrSet(NVIC_USB, (unsigned long)usbDevException, PRIO_EIGHT);     /* �����ⲿ�ж����ȼ���ʹ��     */

    if (0 != usbDevInit()) {                                            /* ��ʼ��USB������              */
        while(1);                                                       /* ��ʼ��ʧ�ܣ�����ִֹͣ��     */
    }

    while(1) {

        usbDevServe();                                                  /* ����USB�¼�                  */

        if (bEPPflags.bits.configuration == 1) {                        /* USB Device�������           */

            if (bEPPflags.bits.ep1_rxdone == 1) {                       /* �߼��˵�1�յ�����            */
                zyIrqDisable();
                bEPPflags.bits.ep1_rxdone = 0;                          /* ����˵�1�յ����ݱ�־        */
                usbDevWriteEndpoint(3, GenEpBuf, 10);                   /* ����10���ֽڵ�PC             */
                zyIrqEnable();
            }
        }
    }
}

/*********************************************************************************************************
** Function name:       main
** Descriptions:        PC��ͨ���߼��˵�1����10���ֽڵ���λ������λ���ӵ����Ǹ��ֽں���ͨ���˵�1���ص�PC
**                      ʵ��ǰ��������ñ�̽����¶˿�.
**                      JP2(VBUS��P1.30,U1CON��P2.9,DEV_D-��P0.30,DEV_D+��P0.29,USB DEV��P1.18)
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int  main (void)
{
    targetInit();                                                       /* ��ʼ��Ŀ��壬����ɾ��       */
    usbDeviceDemo();
    while (1) {

    }
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
