/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevHAL.c
** Latest modified Date:    2009-05-26
** Latest Version:          1.2
** Descriptions:            usbDevLPC3250 �̼�����Ӳ�������Դ�ļ�
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            �޸Ĳ��ִ������ʺ�SmartARM3250ϵ�п�����
**--------------------------------------------------------------------------------------------------------
** Modified by:             Liu Weiyun
** Modified date:           2009-05-26
** Version:                 1.2
** Descriptions:            modify for LPC1700
*********************************************************************************************************/

#include  "usbDevConfig.h"

/*********************************************************************************************************
    1.USB˫������D+��,P0.29(PINSEL1)
    2.��PINSSELn�е�ƫ��
*********************************************************************************************************/
#define __USB_DEV_DP                0x01UL
#define __REG_OFF_DEV_DP            26
/*********************************************************************************************************
    1.USB˫������D-��,P0.30(PINSEL1)
    2.��PINSSELn�е�ƫ��
*********************************************************************************************************/
#define __USB_DEV_DM                0x01UL
#define __REG_OFF_DEV_DM            28
/*********************************************************************************************************
    1.USB UP LED,P1.18(PINSEL3)
    2.��PINSSELn�е�ƫ��
*********************************************************************************************************/
#define __USB_DEV_UP_LED            0x01UL
#define __REG_OFF_DEV_UP_LED        4
/*********************************************************************************************************
    1.USB VBUS,P1.30(PINSEL3)
    2.��PINSSELn�е�ƫ��
*********************************************************************************************************/
#define __USB_DEV_VBUS              0x02UL
#define __REG_OFF_DEV_VBUS          28
/*********************************************************************************************************
    1.USB connect,P2.9(PINSEL4)
    2.��PINSSELn�е�ƫ��
*********************************************************************************************************/
#define __USB_DEV_CONNECT           0x01UL
#define __REG_OFF_DEV_CONNECT       18


#define __USB_DEV_CLK_EN           (0x01UL << 1)
#define __USB_PORTSEL_CLK_EN       (0x01UL << 3)
#define __USB_AHB_CLK_EN           (0x01UL << 4)

#define __MAX_I2C_RATE              100000


#define __HOST_POWER_DIS()
#define __HOST_POWER_EN()
/*********************************************************************************************************
    USB PLL

    (1) USBCLK = M * Fosc, or USBCLK = Fcco / (2 * p)
	(2) Fcco = USBCLK * 2 * P, or Fcco = Fosc * M * 2 * P
	(3) 10MHz <= Fosc <= 25MHz
	(4) USBCLK = 48MHz
	(5) 156MHz <= Fcco <= 320MHz
*********************************************************************************************************/
#define __USB_CLOCK                 48000000                            /*  USB ����Ƶ��                */
#define __USB_PLL_FOSC              12000000                            /*  USB PLL����Ƶ��             */
#define __USB_PLL_FCCO              (__USB_CLOCK * 2 * __USB_PLL_P)
#define __USB_PLL_M                 (__USB_CLOCK / __USB_PLL_FOSC)
#define __USB_PLL_P                 (2)

#define __USB_PLL_M_VALUE           (__USB_PLL_M - 1)
#define __USB_PLL_P_VALUE           (0x01)

#define __PLL_CON_EN                0
#define __PLL_CON_CONNECT           1

#define __PLL_CFG_MSEL              0
#define __PLL_CFG_PSEL              5

#define __PLL_STAT_MSEL             0
#define __PLL_STAT_PSEL             5
#define __PLL_STAT_EN_STAT          8
#define __PLL_STAT_CONNET_STAT      9
#define __PLL_STAT_PLOCK            10

#define __PLL_MODE_DIS_UNCON        0x00                                /* PLL disabled and unconnected */
#define __PLL_MODE_EN_UNCON         0x01                                /* PLL enabled but unconnected  */
#define __PLL_MODE_EN_CONN          0x03                                /* PLL enabled and connected    */

#define __PLL_OPT_FEED()            do {\
                                        USB_ENTER_CRITICAL_SECTION();\
                                        __USBPllFeed = 0xAA;\
                                        __USBPllFeed = 0x55;\
                                        USB_EXIT_CRITICAL_SECTION();\
                                    } while (0)
/*********************************************************************************************************
** Function name:       __usbPllConfig
** Descriptions:        USB PLL setting
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
** NOTE:                (1) USBCLK = M * Fosc, or USBCLK = Fcco / (2 * p)
**                      (2) Fcco = USBCLK * 2 * P, or Fcco = Fosc * M * 2 * P
**                      (3) 10MHz <= Fosc <= 25MHz
**                      (4) USBCLK = 48MHz
**                      (5) 156MHz <= Fcco <= 320MHz
*********************************************************************************************************/
static void  __usbPllConfig (void)
{
    if (0 != (__USBPllState & (0x01UL << __PLL_STAT_CONNET_STAT))) {
        __USBPllCon = __PLL_MODE_DIS_UNCON;
        __PLL_OPT_FEED();
    }

    __USBPllCon = __PLL_MODE_DIS_UNCON;
    __PLL_OPT_FEED();

    __USBPllCfg = (__USB_PLL_P_VALUE << __PLL_CFG_PSEL)
                | (__USB_PLL_M_VALUE << __PLL_CFG_MSEL);
    __PLL_OPT_FEED();

    __USBPllCon = __PLL_MODE_EN_UNCON;
    __PLL_OPT_FEED();

    while (0 == (__USBPllState & (0x01UL << __PLL_STAT_PLOCK)));

    __USBPllCon = __PLL_MODE_EN_CONN;
    __PLL_OPT_FEED();
}

/*********************************************************************************************************
** Function name:       __usbDevPinInit
** Descriptions:        USB�豸���ų�ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void __usbDevPinInit (void)
{
    USB_ENTER_CRITICAL_SECTION();

    __GpUrPin->sel1 &= ~((0x03UL << __REG_OFF_DEV_DP) |
                         (0x03UL << __REG_OFF_DEV_DM));
    __GpUrPin->sel1 |= (__USB_DEV_DP << __REG_OFF_DEV_DP) |
                       (__USB_DEV_DM << __REG_OFF_DEV_DM);

    __GpUrPin->sel3 &= ~((0x03UL << __REG_OFF_DEV_UP_LED) |
                         (0x03UL << __REG_OFF_DEV_VBUS));
    __GpUrPin->sel3 |= (__USB_DEV_UP_LED << __REG_OFF_DEV_UP_LED) |
                       (__USB_DEV_VBUS   << __REG_OFF_DEV_VBUS);

    __GpUrPin->sel4 &=  ~(0x03UL << __REG_OFF_DEV_CONNECT);
    __GpUrPin->sel4 |= (__USB_DEV_CONNECT << __REG_OFF_DEV_CONNECT);

    __GpUrPin->mod3 &= ~(0x03UL << 28);
    __GpUrPin->mod3 |=  (0x02UL << 28);

    USB_EXIT_CRITICAL_SECTION();
}
/*********************************************************************************************************
** Function name:       __usbDevHardwareInit
** Descriptions:        ��ʼ�� USB �豸������Ӳ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevHardwareInit (void)
{
#ifndef Fpclk
#define Fpclk   12000000
#endif

    UINT32  ulClkCtrl = (__USB_AHB_CLK_EN | /*__USB_PORTSEL_CLK_EN |*/ __USB_DEV_CLK_EN);

    __HOST_POWER_DIS();                                                 /*  �ر�LM3526����              */

    __PCONP |= 0x01UL << __PCONP_OFF_USB;

    __usbPllConfig();

    __USBClkCtrl = ulClkCtrl;                                           /*  ����USBʱ�ӿ��ƼĴ���       */
    while ((__USBClkSt & ulClkCtrl) != ulClkCtrl);

    __usbDevPinInit();


#if ATX_ISP1301_EN
    __I2CCtrl    |= 0x100;                                              /*  Reset I2C                   */
    /**
     * Set I2C rate
     */
#if ((Fpclk / __MAX_I2C_RATE) / 2) > 0xff
    __I2CClkHigh  = 0xff;
    __I2CClkLow   = 0xff;
#else
    __I2CClkHigh  = (Fpclk / __MAX_I2C_RATE) / 2;
    __I2CClkLow   = (Fpclk / __MAX_I2C_RATE) / 2;
#endif

    __isp1301Write(__MODE_CTL2_CLR, 0xff);
    __isp1301Write(__MODE_CTL1_CLR, 0xff);
    __isp1301Write(__MODE_CTL2_SET, __MODE_CTL2_BI_DI | __MODE_CTL2_PSW_OE);
    __isp1301Write(__MODE_CTL1_SET, __MODE_CTL1_DAT_SE0);

    __isp1301Write(__OTG_CTL1_CLR, __OTG_CTL1_VBUS_DRV | __OTG_CTL1_VBUS_CHRG);
    __isp1301Write(__OTG_CTL1_SET, __OTG_CTL1_VBUS_DISCHRG);

    __isp1301DisEnPswOE();
    __isp1301Write(__OTG_CTL1_CLR, __OTG_CTL1_DM_PULLDOWN |
                   __OTG_CTL1_DP_PULLDOWN | __OTG_CTL1_DM_PULLUP);
#endif
}

/*********************************************************************************************************
** Function name:       __usbDevHadwareDisconnect
** Descriptions:        �Ͽ���USB ���ߵ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevHadwareDisconnect (void)
{
#if ATX_ISP1301_EN
    /*
     *  ȡ�� ISP1301 �ϵ� D+ �ߵ�������
     */
    __I2CTx = 0x15A;                                                    /*  ���� ISP1301 ��ַ,          */
                                                                        /*  ...R/W=0, д����            */
    __I2CTx = 0x007;                                                    /*  send OTG Control (Set)      */
                                                                        /*  ...register address         */
    __I2CTx = 0x201;                                                    /*  Set DP_PULLUP bit,          */
                                                                        /*  ...send STOP condition      */
    while (!(__I2CSt & __USB_TDI));                                     /*  Wait for TDI to be set      */

    __I2CSt = __USB_TDI;                                                /*  Clear TDI                   */
#endif
}

/*********************************************************************************************************
** Function name:       __usbDevHadwareConnect
** Descriptions:        ���� USB ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevHadwareConnect (void)
{
#if ATX_ISP1301_EN
    /*
     *  ���� ISP1301 �ϵ� D+ �ߵ�������
     */
    __I2CTx = 0x15A;                                                    /*  ���� ISP1301 ��ַ,          */
                                                                        /*  ...R/W=0, д����            */
    __I2CTx = 0x006;                                                    /*  send OTG Control (Set)      */
                                                                        /*  ...register address         */
    __I2CTx = 0x201;                                                    /*  Set DP_PULLUP bit,          */
                                                                        /*  ...send STOP condition      */
    while (!(__I2CSt & __USB_TDI));                                     /*  Wait for TDI to be set      */

    __I2CSt = __USB_TDI;                                                /*  Clear TDI                   */
#endif
}

/*********************************************************************************************************
** Function name:       __usbDevHadwareReconnect
** Descriptions:        �������� USB ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevHadwareReconnect (void)
{
    UINT32 ulClkCnt = 0;

    __usbDevHadwareDisconnect();                                        /*  �Ͽ���USB���ߵ�����         */
    for (ulClkCnt = 0;ulClkCnt <= 0x1FFFF;ulClkCnt++);                  /*  ��ʱ                        */

    __usbDevHadwareConnect();                                           /*  ������USB����               */
}

/*********************************************************************************************************
** Function name:       __usbDevIntConfig
** Descriptions:        ����USB�豸���������ж�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevIntConfig (void)
{
    UINT32 tmp   = __EP_SLOW;

    __DevIntClr  = __FRAME;                                               /*  ��ֹͬ������֡�ж�          */

    __DevIntEn   = tmp;                                                 /*  ʹ�ܸ�λ�ж�, �����ж�      */

    __DevIntPrio = 0;                                                   /*  ʹ�ܶ˵��ж�Ϊ�����ȼ��ж�  */

#if DMA_ENGINE_EN
    tmp = (0x01 << 0) + (0x01 << 1);                                    /*  ʹ�ܿ��ƴ���˵�ӻ��ж�    */
#else
    tmp = (0x01 << 0) + (0x01 << 1) + (0x01 << 2) +
          (0x01 << 3) + (0x01 << 4) + (0x01 << 5);                      /*  ʹ�ܶ˵� 0 ~ 5 �ӻ��ж�     */
#endif


    __EPIntEn    =  tmp;                                                /*  ʹ������˵� 0 ~ 5�ж�      */

    __EPIntPrio &= ~tmp;                                                /*  ����Ϊ�����ȼ��ж�          */


    __USBIntSt   = USBINT_STATUS_BLOCKINTS;                             /*  ����USB�ж�                 */
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
