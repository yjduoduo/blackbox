/*****************************Copyright(c)****************************************************************
**                    Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                           http://www.embedtools.com
**
**------File Info-----------------------------------------------------------------------------------------
** File Name:            isp1301.c
** Latest modified Date: 2009-01-05
** Latest Version:       V1.2
** Description:          ISP1301.c, ISP1301����ز�������
**--------------------------------------------------------------------------------------------------------
** Created By:           Longsui Wu
** Created date:         2007-10-19
** Version:              V1.0
** Descriptions:         ��ʼ�汾
**--------------------------------------------------------------------------------------------------------
** Modified by:          LiuWeiyun
** Modified date:        2009-06-18
** Version:              1.2
** Descriptions:         LPC1700��ֲ
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
*********************************************************************************************************/

#include "usbDevConfig.h"

UINT16 __isp1301VendorIdRead (void);
UINT16 __isp1301ProductIdRead (void);

/*********************************************************************************************************
** Function name:       __isp1301Init
** Descriptions:        ISP1301��ʼ��, �˳�ʼ����Ͽ���������
** input parameters:    None
** output parameters:   None
** Returned value:      TRUE : �����ɹ�, FALSE : ����ʧ��
*********************************************************************************************************/
BOOL __isp1301Init (void)
{
    if (__isp1301VendorIdRead() != 0x04CC) {
        return FALSE;
    }

    if (__isp1301ProductIdRead() != 0x1301) {
        return FALSE;
    }
    __isp1301DPPURRemove();
    __isp1301SetModeDAT_SE0();
    __isp1301SetSpdSuspCtrl();
    __isp1301SetFullSpeed();

    return TRUE;
}

/*********************************************************************************************************
** Function name:       __isp1301Init
** Descriptions:        ISP1301��ʼ��, �˳�ʼ����Ͽ���������
** input parameters:    None
** output parameters:   None
** Returned value:      TRUE : �����ɹ�, FALSE : ����ʧ��
*********************************************************************************************************/
UINT16 __isp1301VendorIdRead (void)
{
    UINT16 usId = 0;
    UINT8  ucTmp;

    ucTmp = __isp1301Read(__ISP1301_VENDOR_ID + 1);
    usId  = (UINT16)(ucTmp << 8);
    ucTmp = __isp1301Read(__ISP1301_VENDOR_ID);
    usId  = (UINT16)(usId + ucTmp);

    return usId;
}

/*********************************************************************************************************
** Function name:       __isp1301Init
** Descriptions:        ISP1301��ʼ��, �˳�ʼ����Ͽ���������
** input parameters:    None
** output parameters:   None
** Returned value:      TRUE : �����ɹ�, FALSE : ����ʧ��
*********************************************************************************************************/
UINT16 __isp1301ProductIdRead (void)
{
    UINT16 usId = 0;
    UINT16 ucTmp;

    ucTmp = __isp1301Read(__ISP1301_PRODUCT_ID + 1);
    usId  = (UINT16)(ucTmp << 8);
    ucTmp = __isp1301Read(__ISP1301_PRODUCT_ID);
    usId  = (UINT16)(usId + ucTmp);

    return usId;
}

/*********************************************************************************************************
** Function name:       __usbI2cWrite
** Descriptions:        ͨ��I2C��ISP1301��������
** input parameters:    pusData  ���������ݻ�����
**                      ucLength �ȷ������ݳ���,ͨ��Ϊ3
** output parameters:   None
** Returned value:      TRUE : �����ɹ�, FALSE : ����ʧ��
*********************************************************************************************************/
BOOL __usbI2cWrite (const UINT16 *pusData, UINT8 ucLength)
{
    UINT8 i;

    for (i = 0; i< ucLength; i++) {
        __I2CTx = *(pusData + i);
        while (__I2CSt & __ISP1301_TFF);                            /*  ���I2C���� FIFO ��,��ȴ�  */
    }

    while (!(__I2CSt & __USB_TDI));                                 /*  �ȴ��������                */

    __I2CSt = __USB_TDI;                                            /*  �����־λ                  */

    return TRUE;
}

/*********************************************************************************************************
** Function name:       __isp1301Write
** Descriptions:        ͨ��I2C��ISP1301д����
** input parameters:    ucReg    �Ĵ�����ַ
**                      ucRegBit �Ĵ����е�λ
** output parameters:   None
** Returned value:      TRUE : �����ɹ�, FALSE : ����ʧ��
*********************************************************************************************************/
BOOL __isp1301Write (UINT8 ucReg, UINT8 ucRegBit)
{
    UINT16 usData[3];

    usData[0] = __ISP1301_ADDR_WRITE | __USB_I2C_START;
    usData[1] = ucReg;
    usData[2] = (UINT16)(ucRegBit | __USB_I2C_STOP);

    return __usbI2cWrite(usData, 3);
}

/*********************************************************************************************************
** Function name:       __isp1301Read
** Descriptions:        ͨ��I2C��ȡISP1301�����Ĵ�������
** input parameters:    ucReg    �Ĵ�����ַ
** output parameters:   None
** Returned value:      ��ȡ���ļĴ���ֵ
*********************************************************************************************************/
UINT8 __isp1301Read (UINT8 ucReg)
{
    __I2CTx = __ISP1301_ADDR_WRITE | __USB_I2C_START;
    __I2CTx = ucReg;
    __I2CTx = __ISP1301_ADDR_READ | __USB_I2C_START;
    __I2CTx = __USB_I2C_STOP;

    while (!(__I2CSt & __USB_TDI));                                 /*  �ȴ��������                */
    __I2CSt = __USB_TDI;                                            /*  �����־λ                  */

    while (__I2CSt & __ISP1301_RFE);                                /*  �ȴ�FIFO����                */

    return (UINT8)(__I2CRx);
}

/*********************************************************************************************************
** Function name:       __isp1301MultiRead
** Descriptions:        ͨ��I2C��ȡISP1301����Ĵ�������
** input parameters:    ucReg     �Ĵ�����ַ
**                      ucLen     Ҫ��ȡ�ļĴ�������
** output parameters:   pucRegDat ��ȡ���ļĴ���ֵ
** Returned value:      TRUE : �����ɹ�, FALSE : ����ʧ��
*********************************************************************************************************/
BOOL __isp1301MultiRead (UINT8 ucReg, UINT8 *pucRegDat, UINT8 ucLen)
{
    SINT8 i = (SINT8)ucLen;

    __I2CTx = __ISP1301_ADDR_WRITE | __USB_I2C_START;
    __I2CTx = ucReg;
    __I2CTx = __ISP1301_ADDR_READ | __USB_I2C_START;

    while (--i) {

        __I2CTx = 0x00;
        while (!(__I2CSt & __USB_TDI));                             /*  �ȴ��������                */
        __I2CSt = __USB_TDI;                                        /*  �����־λ                  */

        while (__I2CSt & __ISP1301_RFE);                            /*  �ȴ�FIFO����                */

        *pucRegDat++ = (UINT8)__I2CRx;
    }

    __I2CTx = __USB_I2C_STOP;
    while (!(__I2CSt & __USB_TDI));                                 /*  �ȴ��������                */
    __I2CSt = __USB_TDI;                                            /*  �����־λ                  */

    while (__I2CSt & __ISP1301_RFE);                                /*  �ȴ�FIFO����                */

    *pucRegDat = (UINT8)__I2CRx;

    return TRUE;
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
