#include "hardware_sets.h"
#include "LPC17xx.h"
#include "uart0.h"
#include "led.h"
#include "w25q256.h"
#include "stdlib.h"
#include "string.h"
#include "CTimeDef.h"
#include "timer_all.h"
#include "ops_w25qxx.h"
#include "usbDevConfig.h"
#include "config.h"



void USB_Init(void)
{

}
/*********************************************************************************************************
** Function name:       usbDeviceDemo
** Descriptions:        �򵥵����ݴ���
** Input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void  usbDeviceDemo (void)
{
//     Debug("usb init!!!\n");
//     zyIsrSet(NVIC_USB, (unsigned long)usbDevException, PRIO_EIGHT);     /* �����ⲿ�ж����ȼ���ʹ��     */

//     if (0 != usbDevInit()) {                                            /* ��ʼ��USB������              */
//         while(1);                                                       /* ��ʼ��ʧ�ܣ�����ִֹͣ��     */
//     }

//     while(1) {

//         usbDevServe();                                                  /* ����USB�¼�                  */

//         if (bEPPflags.bits.configuration == 1) {                        /* USB Device�������           */

//             if (bEPPflags.bits.ep1_rxdone == 1) {                       /* �߼��˵�1�յ�����            */
//                 zyIrqDisable();
//                 bEPPflags.bits.ep1_rxdone = 0;                          /* ����˵�1�յ����ݱ�־        */
//                 usbDevWriteEndpoint(3, GenEpBuf, 10);                   /* ����10���ֽڵ�PC             */
//                 zyIrqEnable();
//             }
//         }
//     }
}



void ops_init(void)
{
    Init_OpsFlash();


}

void hardware_init(void)
{
    SystemClockUpdate();
    uart0_init(UART0_BPS);
    spi_init(0);
    led_init();
//    USB_Init();
    timer_all_init(0,Get_Timer_1s());

    Debug("\n~~~~~~~~~~~~~~~~~~~~~~\n");
    Debug("~~~~~~~~black box~~~~~\n");
    Debug("~~~~~~~~~~~~~~~~~~~~~~\n");
    Debug("init done!\n");
    //�����������ʼ��
    ops_init();

}



