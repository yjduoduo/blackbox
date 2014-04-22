/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevCI.c
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 �̼���������ӿڲ�Դ�ļ�
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            �޸Ĳ��ִ����Է��ϴ���淶v1.2
**--------------------------------------------------------------------------------------------------------
** Modified by:             LiuWeiyun
** Modified date:           2009-01-05
** Version:                 1.2
** Descriptions:            LPC3200��ֲ
*********************************************************************************************************/

#include    "usbDevConfig.h"

/*********************************************************************************************************
  ����USB�豸�Ĵ���
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevConfigMaxPaketSize
** Descriptions:        ���� USB Device �˵��������С
** input parameters:    ucEpIndex       ����˵��
**                      ulPacketSize    �ö˵��������С
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevConfigMaxPaketSize (UINT8 ucEpIndex, UINT32 ulPacketSize)
{
    __EPRealized |= (UINT32)(0x01 << ucEpIndex);                        /*  ��ԭ�����ڵļĴ���ֵ�������*/

    __EPIndex = (UINT32)ucEpIndex;                                      /*  ѡ��˵�                    */

    __EPMaxPacketSize = (UINT32)ulPacketSize;                           /*  д��˵���������ֵ        */

    while((__DevIntSt & EP_RLZEDINT) == 0);                             /*  �ȴ��������                */

    __DevIntClr = EP_RLZEDINT;                                          /*  ����жϱ�־λ              */
}

/*********************************************************************************************************
  Э����������
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevGetData
** Descriptions:        ��USB�������ݼĴ����ж�ȡ����
** input parameters:    ulCmd       ����
** output parameters:   ��
** Returned value:      ������һ���ֽ�
*********************************************************************************************************/
UINT8 __usbDevGetData (UINT32 ulCmd)
{
    __CmdCode = ulCmd;                                                  /*  д��������                  */

    while((__DevIntSt & __CDFULL) == 0);                                  /*  �ȴ�__DevIntSt�Ĵ���CDFULLλ��1*/

    __DevIntClr = __CDFULL;                                               /*  ���CDFULLλ                */

    return (UINT8)__CmdData;                                            /*  ��ȡ����                    */
}

/*********************************************************************************************************
** Function name:       __usbDevSendCmd
** Descriptions:        ��USB�������Ĵ���д������
** input parameters:    ulCmd   ����
**                      ulData  ����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSendCmd (UINT32 ulCmd, UINT32 ulData)
{
    __CmdCode = ulCmd;                                                  /*  д��������                  */

    while((__DevIntSt & __CCEMPTY) == 0);                                 /*  �ȴ�CCEMPTY λ��1           */

    __DevIntClr = __CCEMPTY;                                              /*  ��� CCEMPTY λ             */

    if (ulData != 0) {                                                  /*  ����������ݽ׶�            */

        __CmdCode = ulData;                                             /*  �����ݱ���д���������Ĵ���*/

        while((__DevIntSt & __CCEMPTY) == 0);                             /*  �ȴ�CCEMPTY λ��1           */

        __DevIntClr = __CCEMPTY;                                          /*  ��� CCEMPTY λ             */
    }
}

/*********************************************************************************************************
  LPC3250 USB�豸�������������
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevReadTestRegister
** Descriptions:        �����ԼĴ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ���ԼĴ�����ֵ
*********************************************************************************************************/
UINT16 __usbDevReadTestRegister (void)
{
    UINT16 usTmp = 0;

    __usbDevSendCmd(USBCMD_RDTEST_REG,0);

    usTmp = __usbDevGetData(USBDAT_RDTEST_REG);                         /*  ��ȡ���ֽ�                  */
    usTmp =(UINT16)(usTmp+(UINT16)(__usbDevGetData(USBDAT_RDTEST_REG) << 8));

    return usTmp;
}

/*********************************************************************************************************
** Function name:       __usbDevSetAddressEnable
** Descriptions:        ���� USB �豸��ַ
** input parameters:    ucAddress   ��������ĵ�ֵַ
**                      ucEnable    USB�豸ʹ��λ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetAddressEnable (UINT8 ucAddress, UINT8 ucEnable)
{
    if (ucEnable) {
        ucAddress |= 0x80;
    }

    __usbDevSendCmd(USBCMD_SET_ADDRESS,((UINT32)(ucAddress << 16)) | USBDAT_SET_ADDRESS);
}

/*********************************************************************************************************
** Function name:       __usbDevSetEndpointEnable
** Descriptions:        ���� USB �豸
** input parameters:    ucEnable        USB�˵�ʹ��λ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetEndpointEnable (UINT8 ucEnable)
{
    __usbDevSendCmd(USBCMD_CONFIG_DEV, ((UINT32)(ucEnable << 16)) | USBDAT_CONFIG_DEV);
}

/*********************************************************************************************************
** Function name:       __usbDevSetMode
** Descriptions:        ����ģʽ
** input parameters:    ucSetVal    ģʽֵ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetMode (UINT8 ucSetVal)
{
    __usbDevSendCmd(USBCMD_SET_MODE, ((UINT32)(ucSetVal << 16)) | USBDAT_SET_MODE);
}

/*********************************************************************************************************
** Function name:       __usbDevSetDevStatus
** Descriptions:        �����豸״̬
** input parameters:    ucSetvalue  ģʽֵ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetDevStatus (UINT8 ucSetVal)
{
    __usbDevSendCmd(USBCMD_SET_DEVSTATUS, ((UINT32)(ucSetVal << 16)) | USBDAT_SET_DEVSTATUS);
}

/*********************************************************************************************************
** Function name:       __usbDevGetDevStatus
** Descriptions:        ��ȡ�豸״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
UINT8 __usbDevGetDevStatus (void)
{
    __usbDevSendCmd(USBCMD_GET_DEVSTATUS, 0);

    return __usbDevGetData(USBDAT_GET_DEVSTATUS);
}

/*********************************************************************************************************
** Function name:       __usbDevSelectEndpoint
** Descriptions:        ѡ��˵�,����ȡ�˵���Ϣ
** input parameters:    ucEpIndex   ����˵�����
** output parameters:   ��
** Returned value:      ��Ӧ�˵����Ϣ
*********************************************************************************************************/
UINT8 __usbDevSelectEndpoint (UINT8 ucEpIndex)
{
    UINT32 ulTmp = (UINT32)(ucEpIndex << 16);

    __usbDevSendCmd(ulTmp | USBCMD_SEL_ENDP, 0);

    return __usbDevGetData(ulTmp | USBDAT_SEL_ENDP);
}

/*********************************************************************************************************
** Function name:       __usbDevSelectClrIntEndpoint
** Descriptions:        ѡ��˵�,����ȡ�˵���Ϣ�������Ӧ�˵���ж�
** input parameters:    ucEpIndex   ����˵��
** output parameters:   ��
** Returned value:      ��Ӧ�˵����Ϣ
*********************************************************************************************************/
UINT8 __usbDevSelectClrIntEndpoint (UINT8 ucEpIndex)
{
    UINT8  i = 0;
    UINT32 ulTmp = (UINT32)((ucEpIndex + 0x40) << 16);

    __usbDevSendCmd(ulTmp | USBCMD_SEL_CLRINT_ENDP, 0);
    i = __usbDevGetData(ulTmp | USBDAT_SEL_CLRINT_ENDP);

    __EPIntClr = (unsigned long)(0x01 << ucEpIndex);

    return i;
}

/*********************************************************************************************************
** Function name:       __usbDevSetEndpointStatus
** Descriptions:        ���ö˵�״̬
** input parameters:    ucEpIndex       ����˵��
**                      ucStalled   �˵�ֹͣλ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetEndpointStatus (UINT8 ucEpIndex, UINT8 ucStalled)
{
    UINT32 ulTmp1 = (UINT32)((ucEpIndex + 0x40) << 16);
    UINT32 ulTmp2 = (UINT32)(ucStalled << 16);

    __usbDevSendCmd(ulTmp1 | USBCMD_SET_ENDP_STATUS, ulTmp2 | USBDAT_SET_ENDP_STATUS);
}

/*********************************************************************************************************
** Function name:       __usbDevClearBuffer
** Descriptions:        ���OUT�˵㻺����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
UINT8 __usbDevClearBuffer (void)
{
    __usbDevSendCmd(USBCMD_CLEAR_BUFFER, 0);                            /*  ������û�����ݽ׶�          */

    return __usbDevGetData(USBDAT_CLEAR_BUFFER);
}

/*********************************************************************************************************
** Function name:       __usbDevValidBuffer
** Descriptions:        ʹ�� IN �˵㻺����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevValidBuffer (void)
{
    __usbDevSendCmd(USBCMD_VALID_BUFFER, 0);
}

/*********************************************************************************************************
** Function name:       __usbDevSoftDisconnect
** Descriptions:        �Ͽ���USB���ߵ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSoftDisconnect (void)
{
    __usbDevSetDevStatus(0x00);
}

/*********************************************************************************************************
** Function name:       __usbDevSoftConnect
** Descriptions:        ���ӵ�USB����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSoftConnect (void)
{
    __usbDevSetDevStatus(0x01);
}

/*********************************************************************************************************
** Function name:       __usbDevSoftReconnect
** Descriptions:        ��������USB����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSoftReconnect (void)
{
    UINT32 clk_cnt;

    __usbDevSoftDisconnect();                                           /*  �Ͽ���USB���ߵ�����         */

    for (clk_cnt = 0;clk_cnt<= 0x1FFFF;clk_cnt++);                      /*  ��ʱ                        */

    __usbDevSoftConnect();                                              /*  ���ӵ�USB����               */
}

/*********************************************************************************************************
  �˵����ݴ���
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       usbDevReadEndpoint
** Descriptions:        ͨ��ָ������˵��ȡ����
** input parameters:    ucEpIndex       ����˵��
**                      pucRcvBuf       ���ջ�����ָ��
**                      ucReadLen       Ҫ�����ֽڳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
UINT8 usbDevReadEndpoint (UINT8 ucEpIndex,
                          UINT8 *pucRcvBuf,
                          UINT8 ucReadLen)
{
    UINT32 ulTmp    = 0;
    UINT32 i        = 0;
    UINT32 j        = 0;
    UINT32 ulRcvLen = 0;                                                /*  FIFO�����յ����ֽ���        */

    __USBCtrl = (UINT32)((ucEpIndex >> 1) << 2 | 0x01);                 /*  ѡ���߼��˵㲢ʹ�ܶ�����    */

    if((ucEpIndex % 2 == 0)) {
        while((__RxPacketLen & (0x3 << 10)) == 0);                      /*  �ȴ����ݾ���                */
    }

    ulRcvLen =(UINT32)(__RxPacketLen & 0x3FF);                          /*  ��ȡ�յ������ݵ��ֽڳ���    */

    if (ulRcvLen > ucReadLen) {
        ulRcvLen = ucReadLen;
    }
                                                                        /*  �� EP RAM �ж�ȡ����        */
    for (i = 0; i < (ulRcvLen / 4); i++) {

        ulTmp =(UINT32)__RxData;
        *pucRcvBuf++ = (UINT8)ulTmp;
        *pucRcvBuf++ = (UINT8)(ulTmp >> 8);
        *pucRcvBuf++ = (UINT8)(ulTmp >> 16);
        *pucRcvBuf++ = (UINT8)(ulTmp >> 24);
    }

    j = ulRcvLen % 4;
    if (j != 0) {

        ulTmp = (UINT32)__RxData;

        for(i = 0; i < j; i++) {
           *pucRcvBuf++ = (UINT8)(ulTmp >> (i << 3));
        }
    }

    __DevIntClr = __RxENDPKT;                                             /*  ��� RxENDPKT λ            */

    __usbDevSelectEndpoint(ucEpIndex);
    __usbDevClearBuffer();                                              /*  ���OUT������               */

    __USBCtrl = 0;
    return ((UINT8)ulRcvLen);                                           /*  ����ʵ���յ����ֽ���        */
}

/*********************************************************************************************************
** Function name:       usbDevWriteEndpoint
** Descriptions:        ��ָ������˵�д������
** input parameters:    ucEpIndex       ����˵��
**                      pucWriteBuf     ���ͻ�����
**                      ucWriteLen      Ҫд���ֽڳ���
** output parameters:   ��
** Returned value:      ʵ��д����ֽ���
*********************************************************************************************************/
UINT8 usbDevWriteEndpoint (UINT8 ucEpIndex,
                           UINT8 *pucWriteBuf,
                           UINT8 ucWriteLen)
{
    UINT32 ulTmp = 0;
    UINT32 i     = 0;
    UINT32 j     = 0;

    ulTmp = ((ucEpIndex >> 1) << 2) | 0x02;                             /*  ѡ���߼��˵㲢ʹ��д����    */
    __USBCtrl = (UINT32)ulTmp;


    __TxPacketLen = (UINT32)ucWriteLen;                                     /*  д��Ҫ���͵����ݵ��ֽڳ���  */

    j = ucWriteLen >> 2;
    for(i = 1; i <= j; i++) {

        ulTmp =  (UINT32)*pucWriteBuf++;
        ulTmp += (UINT32)(*pucWriteBuf++ << 8);
        ulTmp += (UINT32)(*pucWriteBuf++ << 16);
        ulTmp += (UINT32)(*pucWriteBuf++ << 24);
        __TxData = ulTmp;

        while((__TxPacketLen & 0x3FF) != (UINT8)(ucWriteLen - (i << 2)));
    }

    j = ucWriteLen % 4;
    if (j != 0) {

        ulTmp = 0;
        for(i = 0; i < j; i++) {

           ulTmp |= ((UINT32)*pucWriteBuf) << (i << 3);
           pucWriteBuf++;
        }

        __TxData = ulTmp;

        while((__TxPacketLen & 0x3FF) != 0);                                /*  �ȴ� __TxPacketLen Ϊ 0         */
    }

    if (ucWriteLen != 0) {

        while((__DevIntSt & __TxENDPKT) ==0);                         /*  �ȴ�д�����                */
        __DevIntClr = __TxENDPKT;
    }

    __usbDevSelectEndpoint(ucEpIndex);                                  /*  ʹ�ܷ��ͻ�����              */
    __usbDevValidBuffer();

    __USBCtrl = 0;
    return ucWriteLen;                                                  /*  ����ʵ��д�볤��            */
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
