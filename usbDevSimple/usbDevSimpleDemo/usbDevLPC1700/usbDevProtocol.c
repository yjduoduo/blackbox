/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevProtocol.c
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 �̼�����Э���Դ�ļ�
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
**
*********************************************************************************************************/

#include "usbDevConfig.h"

CONTROL_XFER ControlData;                                               /*  ���崫����ƽṹ����        */

/*********************************************************************************************************
  USB Э��㺯��
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbStallEp0
** Descriptions:        ʹ���ƶ˵㴦��ֹͣ״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevStallEp0 (void)
{
    __usbDevSetEndpointStatus(0, 1);                                    /*  ֹͣ����OUT�˵�             */
    __usbDevSetEndpointStatus(1, 1);                                    /*  ֹͣ����IN�˵�              */
}

/*********************************************************************************************************
** Function name:       __usbDevReserved
** Descriptions:        �����ӳ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevReserved (void)
{
    __usbDevStallEp0();
}

/*********************************************************************************************************
** Function name:       __usbDevInitUnconfig
** Descriptions:        �����ַ״̬,��ֹ 0 ��������ж˵�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevInitUnconfig (void)
{
    __usbDevSetEndpointEnable(0);
}

/*********************************************************************************************************
** Function name:       __usbInitConfig
** Descriptions:        ���ô���,����˵��շ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevInitConfig (void)
{
    __usbDevSetEndpointEnable(1);
}

/*********************************************************************************************************
** Function name:       __usbDevSingleTransmit
** Descriptions:        ͨ������˵� 1 ��������
** input parameters:    pcSendbuf   ��������ָ��
**                      ucSendlen   �������ݳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSingleTransmit (UINT8 *pucSendbuf,UINT8 ucSendLen)
{
    if( ucSendLen <= EP0_PACKET_SIZE) {                                 /*  �������˵�������С        */

        usbDevWriteEndpoint(1,
                            pucSendbuf,
                            ucSendLen);                                 /*  �����IN�˵�д��1�ֽ�����   */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevCodeTransmit
** Descriptions:        ͨ������˵� 1 ��������
** input parameters:    pucRomData  ��������ָ��
**                      usSendLen   �������ݳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevCodeTransmit(UINT8 * pucRomData, UINT16 usSendLen)
{
    ControlData.wCount = 0;

    if (ControlData.wLength > usSendLen) {

        ControlData.wLength = usSendLen;                                /*  ����ҪС��usSendLen         */
    }

    ControlData.pData = pucRomData;

    if (ControlData.wLength >= EP0_PACKET_SIZE) {

        usbDevWriteEndpoint(1,
                            ControlData.pData,
                            EP0_PACKET_SIZE );

        ControlData.wCount += EP0_PACKET_SIZE;                          /*  �������ۼ�                  */

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.control_state = USB_TRANSMIT;                    /*  ��ʶ����״̬                */
        USB_EXIT_CRITICAL_SECTION();

    } else {

        usbDevWriteEndpoint(1,
                            pucRomData,
                            (UINT8)ControlData.wLength);                /*  ��ȫ������д��˵�          */

        ControlData.wCount =(UINT16)(ControlData.wCount
                                     + ControlData.wLength);            /*  �������ۼ�                  */

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.control_state = USB_IDLE;                        /*  ��ʶ����״̬                */
        USB_EXIT_CRITICAL_SECTION();
    }
}

/*********************************************************************************************************
  USB ��׼�豸����������
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevGetStatus
** Descriptions:        ����Ҫ���ȡ״̬,�豸����16λ��״̬������������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevGetStatus (void)
{
    UINT8 ucEpindex   = 0;
    UINT8 ucTxdata[2] = {0};
    UINT8 ucEpstat    = 0;

    UINT8 ucRecipient =(UINT8)(ControlData.DeviceRequest.bmRequestType & USB_RECIPIENT);
                                                                        /*  ��ȡ��������                */
    if (ucRecipient == USB_RECIPIENT_DEVICE) {

        if (bEPPflags.bits.remote_wakeup == 1) {
            ucTxdata[0] = 3;                                            /*  ֧��Զ�̻��Ѻ��Թ���        */
        } else {

            ucTxdata[0] = 1;                                            /*  ��֧��������������          */
        }

        ucTxdata[1]=0;                                                  /*  �� 8 λ�� 0                 */
        __usbDevSingleTransmit(ucTxdata, 2);                            /*  ����16ibt��USB����          */

    } else if (ucRecipient == USB_RECIPIENT_INTERFACE) {

        ucTxdata[0]=0;
        ucTxdata[1]=0;
        __usbDevSingleTransmit(ucTxdata, 2);                            /*  ����16ibt��USB����          */

    } else if (ucRecipient == USB_RECIPIENT_ENDPOINT) {

        ucEpindex = (UINT8)(ControlData.DeviceRequest.wIndex & MAX_ENDPOINTS);
        if (ControlData.DeviceRequest.wIndex & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {

            ucEpstat = __usbDevSelectEndpoint((UINT8)(ucEpindex * 2 + 1));
                                                                        /*  �� IN �˵�״̬              */
        } else {

            ucEpstat = __usbDevSelectEndpoint((UINT8)(ucEpindex * 2));  /*  �� OUT �˵�״̬             */
        }

        if (ucEpstat & USB_STALL) {
            ucTxdata[0] = 1;                                            /*  �˵��ѱ���ֹ                */

        } else {

            ucTxdata[0] = 0;                                            /*  �˵��ѽ��                  */
        }

        ucTxdata[1] = 0;
        __usbDevSingleTransmit(ucTxdata, 2);                            /*  ����16ibt��USB����          */

    } else {

        __usbDevStallEp0();                                             /*  �Ǳ�׼����,��ֹ�߼��˵�0    */
    }

}

/*********************************************************************************************************
** Function name:       __usbDevClearFeature
** Descriptions:        �������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevClearFeature (void)
{
    UINT8 ucEpindex   = 0;
    UINT8 ucRecipient = (UINT8)(ControlData.DeviceRequest.bmRequestType & USB_RECIPIENT);
                                                                        /*  ��ȡ��������                */
    if (ucRecipient == USB_RECIPIENT_DEVICE
        && ControlData.DeviceRequest.wValue == USB_FEATURE_REMOTE_WAKEUP) {

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.remote_wakeup = 0;                               /*  ���Զ�̻�������            */
        USB_EXIT_CRITICAL_SECTION();
        __usbDevSingleTransmit(0, 0);                                   /*  ����һ���հ�                */

    } else if ((ucRecipient == USB_RECIPIENT_ENDPOINT)
               && (ControlData.DeviceRequest.wValue == USB_FEATURE_ENDPOINT_STALL)) {

        ucEpindex = (UINT8)(ControlData.DeviceRequest.wIndex & MAX_ENDPOINTS);
        if (ControlData.DeviceRequest.wIndex & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2 + 1), 0);   /*  ��� IN �˵�                */

        } else {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2), 0);       /*  ��� OUT �˵�               */
        }

        __usbDevSingleTransmit(0, 0);                                   /*  ���Ϳհ���ʾִ�гɹ�        */

    } else {

        __usbDevStallEp0();                                             /*  ���ɹ�,��ֹ�߼��˵�0        */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevSetFeature
** Descriptions:        ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetFeature (void)
{
    UINT8 ucEpindex = 0;
    UINT8 ucRecipient =(UINT8)(ControlData.DeviceRequest.bmRequestType & USB_RECIPIENT);
                                                                        /*  ��ȡ��������                */
    if ((ucRecipient == USB_RECIPIENT_DEVICE)                           /*  �����豸                    */
        && (ControlData.DeviceRequest.wValue == USB_FEATURE_REMOTE_WAKEUP)) {

        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.remote_wakeup = 1;                               /*  ����Զ�̻��ѱ�־            */
        USB_EXIT_CRITICAL_SECTION();
        __usbDevSingleTransmit(0, 0);                                   /*  ����һ���հ�                */

    } else if ((ucRecipient == USB_RECIPIENT_ENDPOINT)                  /*  ����˵�                    */
        && (ControlData.DeviceRequest.wValue == USB_FEATURE_ENDPOINT_STALL)) {

        ucEpindex = (UINT8)(ControlData.DeviceRequest.wIndex & MAX_ENDPOINTS);

        if (ControlData.DeviceRequest.wIndex & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2 + 1), 1);   /*  IN �˵㱻��ֹ               */

        } else {

            __usbDevSetEndpointStatus((UINT8)(ucEpindex * 2), 1);       /*  OUT�˵㱻��ֹ               */
        }

        __usbDevSingleTransmit(0, 0);                                   /*  ����һ���հ�                */

    } else {

        __usbDevStallEp0();                                             /*  �Ǳ�׼����,��ֹ�߼��˵�0    */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevSetAddress
** Descriptions:        ���õ�ַ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetAddress (void)
{
    __usbDevSetAddressEnable((UINT8)(ControlData.DeviceRequest.wValue & DEVICE_ADDRESS_MASK),1);

    __usbDevSingleTransmit(0, 0);                                       /*  ����һ���հ�                */
}

/*********************************************************************************************************
** Function name:       __usbDevGetDescriptor
** Descriptions:        ��ȡ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevGetDescriptor (void)
{
    UINT8 ucDescriptor =(UINT8)(MSB(ControlData.DeviceRequest.wValue)); /*  ȡ������������              */

    if (ucDescriptor == USB_DEVICE_DESCRIPTOR_TYPE) {
                                                                        /*  ��ȡ�豸������              */
        __usbDevCodeTransmit((UINT8 *)&DeviceDescr, sizeof(USB_DEVICE_DESCRIPTOR));

    } else if (ucDescriptor == USB_CONFIGURATION_DESCRIPTOR_TYPE) {

         if (ControlData.DeviceRequest.wLength > CONFIG_DESCRIPTOR_LENGTH) {

             ControlData.DeviceRequest.wLength = CONFIG_DESCRIPTOR_LENGTH;
         }
                                                                        /*  ��������������              */
         __usbDevCodeTransmit((UINT8 *)&(usb_descr.ConfigDescr),ControlData.DeviceRequest.wLength);

    } else {

         __usbDevStallEp0();                                            /*  ��ֹ�߼��˵�0               */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevGetConfiguration
** Descriptions:        ��ȡ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevGetConfiguration (void)
{
    UINT8 ucTmp = (UINT8)bEPPflags.bits.configuration;                  /*  ȡ������ֵ                  */

    __usbDevSingleTransmit(&ucTmp, 1);                                  /*  ��������ֵ                  */
}

/*********************************************************************************************************
** Function name:       __usbDevSetConfiguration
** Descriptions:        ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetConfiguration (void)
{
    if (ControlData.DeviceRequest.wValue == 0) {

        __usbDevSingleTransmit(0, 0);                                   /*  ����һ���հ�                */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.configuration = 0;                               /*  ��ʶ�豸δ����              */
        USB_EXIT_CRITICAL_SECTION();
        __usbDevInitConfig();                                           /*  ��ֹ��0��������߼��˵�     */

    } else if (ControlData.DeviceRequest.wValue == 1) {

        __usbDevSingleTransmit(0, 0);                                   /*  ����һ���հ�                */
        __usbDevInitUnconfig();                                         /*  ��ֹ��0��������߼��˵�     */
        __usbDevInitConfig();                                           /*  ʹ��ȫ���˵�                */
        USB_ENTER_CRITICAL_SECTION();
        bEPPflags.bits.configuration = 1;                               /*  ��ʶ�豸�ѱ�����            */
        USB_EXIT_CRITICAL_SECTION();

    } else {
         __usbDevStallEp0();                                            /*  ��ֹ�߼��˵�0               */
    }
}

/*********************************************************************************************************
** Function name:       __usbDevGetInterface
** Descriptions:        ��ȡ�ӿ���Ϣ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevGetInterface (void)
{
    UINT8 ucTxData = 0;                                                 /*  ֻ��һ���ӿ�                */

    __usbDevSingleTransmit(&ucTxData, 1);                               /*  ����һ���ֽ�                */
}

/*********************************************************************************************************
** Function name:       __usbDevSetInterface
** Descriptions:        ���ýӿ���Ϣ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevSetInterface (void)
{
    if (ControlData.DeviceRequest.wValue == 0 && ControlData.DeviceRequest.wIndex == 0) {

        __usbDevSingleTransmit(0, 0);                                   /*  ����һ���հ�,��ʾִ�гɹ�   */
    } else {

        __usbDevStallEp0();                                             /*  ��ֹ�߼��˵�0               */
    }
}

/*********************************************************************************************************
** Function name:       StandardDeviceRequest
** Descriptions:        USB ��׼�豸������ڵ�ַָ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void (*StandardDeviceRequest[]) (void) =
{
    __usbDevGetStatus,
    __usbDevClearFeature,
    __usbDevReserved,
    __usbDevSetFeature,
    __usbDevReserved,
    __usbDevSetAddress,
    __usbDevGetDescriptor,
    __usbDevReserved,
    __usbDevGetConfiguration,
    __usbDevSetConfiguration,
    __usbDevGetInterface,
    __usbDevSetInterface,
    __usbDevReserved,
    __usbDevReserved,
    __usbDevReserved,
    __usbDevReserved
};

/*********************************************************************************************************
** Function name:       __usbDevEp0RxDone
** Descriptions:        ͨ���˵����� 0 ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevEp0RxDone (void)
{
    UINT8 ucEpState  = 0;
    UINT8 ucReadLen = 0;
    UINT8 req[sizeof(DEVICE_REQUEST)];

    ucEpState = __usbDevSelectClrIntEndpoint(0);                        /*  ����ö˵���ж�            */

    if (ucEpState & USB_SETUPPACKET) {                                  /*  ���յ�SETUP��               */

        ControlData.wLength = 0;
        ControlData.wCount = 0;

        if(usbDevReadEndpoint(0,req,sizeof(ControlData.DeviceRequest))
           != sizeof(DEVICE_REQUEST)) {                                 /*  �Ӷ˵� 0 ��ȡ����           */

            __usbDevSetEndpointStatus(0, 1);                            /*  ��ֹ���ƶ˵�0               */
            __usbDevSetEndpointStatus(1, 1);                            /*  ��ֹ���ƶ˵�1               */
            bEPPflags.bits.control_state = USB_IDLE;                    /*  ��ʶ����״̬                */
            return;
        }

        /*
         *  ���� SETUP ���ɹ�
         */

        ControlData.DeviceRequest.bmRequestType = req[0];
        ControlData.DeviceRequest.bRequest      = req[1];
        ControlData.DeviceRequest.wValue        = (UINT16)(req[3] * 256 + req[2]);
        ControlData.DeviceRequest.wIndex        = (UINT16)(req[5] * 256 + req[4]);
        ControlData.DeviceRequest.wLength       = (UINT16)(req[7] * 256 + req[6]);
        ControlData.wLength = ControlData.DeviceRequest.wLength;

        if (ControlData.DeviceRequest.bmRequestType & (UINT8)USB_ENDPOINT_DIRECTION_MASK) {
                                                                        /*  ���Ϊ���ƶ�ȡ              */
             bEPPflags.bits.setup_packet = 1;
             bEPPflags.bits.control_state = USB_TRANSMIT;

        } else {                                                        /*  ���Ϊ����д                */

             if (ControlData.DeviceRequest.wLength == 0) {

                 bEPPflags.bits.setup_packet = 1;
                 bEPPflags.bits.control_state = USB_IDLE;

             } else {

                 if (ControlData.DeviceRequest.wLength > MAX_CONTROLDATA_SIZE) {

                    bEPPflags.bits.control_state = USB_IDLE;
                    __usbDevSetEndpointStatus(0, 1);                    /*  ��ֹ���ƶ˵�0               */
                    __usbDevSetEndpointStatus(1, 1);                    /*  ��ֹ���ƶ˵�1               */

                 } else {

                    bEPPflags.bits.control_state = USB_RECEIVE;         /*  ��ʶ����״̬                */
                 }

             }
        }
    }

    /*
     *  ����Ϊ���ƽ������ݽ׶�
     */

    else if (bEPPflags.bits.control_state == USB_RECEIVE) {
                                                                        /*  �����ǰΪ����״̬          */
        ucReadLen = usbDevReadEndpoint(0,
                                       ControlData.dataBuffer + ControlData.wCount,
                                       EP0_PACKET_SIZE);                /*  �Ӷ˵� 0 ��ȡ����           */

        ControlData.wCount = (UINT16)(ControlData.wCount+ucReadLen);

        if( ucReadLen != EP0_PACKET_SIZE || ControlData.wCount >= ControlData.wLength) {
                                                                        /*  ��ɽ�������                */
            bEPPflags.bits.setup_packet = 1;
            bEPPflags.bits.control_state = USB_IDLE;
        }

    } else {

        bEPPflags.bits.control_state = USB_IDLE;
    }
}

/*********************************************************************************************************
** Function name:       __usbDevEp0TxDone
** Descriptions:        ͨ������˵� 0 ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevEp0TxDone(void)
{
    SINT16 i = (SINT16)(ControlData.wLength - ControlData.wCount);

    __usbDevSelectClrIntEndpoint(1);                                    /*  ����˵��жϱ�־            */

    if (bEPPflags.bits.control_state != USB_TRANSMIT) {
                                                                        /*  �Ƿ���״̬                  */
        __usbDevSingleTransmit(0, 0);
        return;                                                         /*  ֱ�ӷ���                    */
    }

    if( i >= EP0_PACKET_SIZE) {
                                                                        /*  δ�����ֽ�������64          */
        usbDevWriteEndpoint(1,
                            ControlData.pData + ControlData.wCount,
                            EP0_PACKET_SIZE);

        ControlData.wCount += EP0_PACKET_SIZE;
        bEPPflags.bits.control_state = USB_TRANSMIT;

    } else if( i != 0) {
                                                                        /*  ��������δ���͵��ֽ�        */
        usbDevWriteEndpoint(1, ControlData.pData + ControlData.wCount,(UINT8)i);
        ControlData.wCount =(UINT16)(ControlData.wCount+i);
        bEPPflags.bits.control_state = USB_IDLE;                        /*  ��״̬Ϊ�ȴ�״̬            */

    } else if (i == 0) {

        usbDevWriteEndpoint(1, 0, 0);                                   /*  ��ɷ���,�ٷ���һ���հ�     */
        bEPPflags.bits.control_state = USB_IDLE;
    }
}

/*********************************************************************************************************
** Function name:       __usbDevControlHandler
** Descriptions:        ���ƴ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void __usbDevControlHandler (void)
{
    UINT8 ucRequesttype  = 0;
    UINT8 ucRequestindex = 0;

    ucRequesttype  = (UINT8)(ControlData.DeviceRequest.bmRequestType & USB_REQUEST_TYPE_MASK);
                                                                        /*  ������������                */

    ucRequestindex = (UINT8)(ControlData.DeviceRequest.bRequest & USB_REQUEST_MASK);

    if (ucRequesttype == USB_STANDARD_REQUEST)
        (*StandardDeviceRequest[ucRequestindex])();                     /*  ��׼����                    */
    //else if (ucRequesttype == USB_VENDOR_REQUEST)                     /*  ��������                    */
    //  (*VendorDeviceRequest[ucRequestindex])();
    //else if(ucRequesttype == USB_CLASS_REQUEST)
    //  (*ClassDeviceRequest[ucRequestindex])();                        /*  ������                      */
    else {

        __usbDevStallEp0();                                             /*  ��ֹ�߼��˵�0               */
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
