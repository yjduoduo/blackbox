/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               usbDevCI.c
** Latest modified Date:    2008-05-10
** Latest Version:          1.1
** Descriptions:            usbDevLPC3250 固件程序命令接口层源文件
**--------------------------------------------------------------------------------------------------------
** Created by:              ZhengMingyuan
** Created date:            2005-08-08
** Version:                 1.0
** Descriptions:            The original version
**--------------------------------------------------------------------------------------------------------
** Modified by:             LingGuilin
** Modified date:           2008-05-10
** Version:                 1.1
** Descriptions:            修改部分代码以符合代码规范v1.2
**--------------------------------------------------------------------------------------------------------
** Modified by:             LiuWeiyun
** Modified date:           2009-01-05
** Version:                 1.2
** Descriptions:            LPC3200移植
*********************************************************************************************************/

#include    "usbDevConfig.h"

/*********************************************************************************************************
  操作USB设备寄存器
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevConfigMaxPaketSize
** Descriptions:        配置 USB Device 端点的最大包大小
** input parameters:    ucEpIndex       物理端点号
**                      ulPacketSize    该端点的最大包大小
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevConfigMaxPaketSize (UINT8 ucEpIndex, UINT32 ulPacketSize)
{
    __EPRealized |= (UINT32)(0x01 << ucEpIndex);                        /*  与原来存在的寄存器值做或操作*/

    __EPIndex = (UINT32)ucEpIndex;                                      /*  选择端点                    */

    __EPMaxPacketSize = (UINT32)ulPacketSize;                           /*  写入端点最大包长度值        */

    while((__DevIntSt & EP_RLZEDINT) == 0);                             /*  等待处理完成                */

    __DevIntClr = EP_RLZEDINT;                                          /*  清除中断标志位              */
}

/*********************************************************************************************************
  协议引擎命令
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevGetData
** Descriptions:        从USB命令数据寄存器中读取数据
** input parameters:    ulCmd       命令
** output parameters:   无
** Returned value:      读到的一个字节
*********************************************************************************************************/
UINT8 __usbDevGetData (UINT32 ulCmd)
{
    __CmdCode = ulCmd;                                                  /*  写入命令字                  */

    while((__DevIntSt & __CDFULL) == 0);                                  /*  等待__DevIntSt寄存器CDFULL位置1*/

    __DevIntClr = __CDFULL;                                               /*  清除CDFULL位                */

    return (UINT8)__CmdData;                                            /*  读取数据                    */
}

/*********************************************************************************************************
** Function name:       __usbDevSendCmd
** Descriptions:        向USB命令代码寄存器写入命令
** input parameters:    ulCmd   命令
**                      ulData  数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSendCmd (UINT32 ulCmd, UINT32 ulData)
{
    __CmdCode = ulCmd;                                                  /*  写入命令字                  */

    while((__DevIntSt & __CCEMPTY) == 0);                                 /*  等待CCEMPTY 位置1           */

    __DevIntClr = __CCEMPTY;                                              /*  清除 CCEMPTY 位             */

    if (ulData != 0) {                                                  /*  如果还有数据阶段            */

        __CmdCode = ulData;                                             /*  将数据编码写入命令代码寄存器*/

        while((__DevIntSt & __CCEMPTY) == 0);                             /*  等待CCEMPTY 位置1           */

        __DevIntClr = __CCEMPTY;                                          /*  清除 CCEMPTY 位             */
    }
}

/*********************************************************************************************************
  LPC3250 USB设备控制器相关命令
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       __usbDevReadTestRegister
** Descriptions:        读测试寄存器
** input parameters:    无
** output parameters:   无
** Returned value:      测试寄存器的值
*********************************************************************************************************/
UINT16 __usbDevReadTestRegister (void)
{
    UINT16 usTmp = 0;

    __usbDevSendCmd(USBCMD_RDTEST_REG,0);

    usTmp = __usbDevGetData(USBDAT_RDTEST_REG);                         /*  读取低字节                  */
    usTmp =(UINT16)(usTmp+(UINT16)(__usbDevGetData(USBDAT_RDTEST_REG) << 8));

    return usTmp;
}

/*********************************************************************************************************
** Function name:       __usbDevSetAddressEnable
** Descriptions:        设置 USB 设备地址
** input parameters:    ucAddress   主机分配的地址值
**                      ucEnable    USB设备使能位
** output parameters:   无
** Returned value:      无
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
** Descriptions:        设置 USB 设备
** input parameters:    ucEnable        USB端点使能位
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetEndpointEnable (UINT8 ucEnable)
{
    __usbDevSendCmd(USBCMD_CONFIG_DEV, ((UINT32)(ucEnable << 16)) | USBDAT_CONFIG_DEV);
}

/*********************************************************************************************************
** Function name:       __usbDevSetMode
** Descriptions:        设置模式
** input parameters:    ucSetVal    模式值
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetMode (UINT8 ucSetVal)
{
    __usbDevSendCmd(USBCMD_SET_MODE, ((UINT32)(ucSetVal << 16)) | USBDAT_SET_MODE);
}

/*********************************************************************************************************
** Function name:       __usbDevSetDevStatus
** Descriptions:        设置设备状态
** input parameters:    ucSetvalue  模式值
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetDevStatus (UINT8 ucSetVal)
{
    __usbDevSendCmd(USBCMD_SET_DEVSTATUS, ((UINT32)(ucSetVal << 16)) | USBDAT_SET_DEVSTATUS);
}

/*********************************************************************************************************
** Function name:       __usbDevGetDevStatus
** Descriptions:        获取设备状态
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
UINT8 __usbDevGetDevStatus (void)
{
    __usbDevSendCmd(USBCMD_GET_DEVSTATUS, 0);

    return __usbDevGetData(USBDAT_GET_DEVSTATUS);
}

/*********************************************************************************************************
** Function name:       __usbDevSelectEndpoint
** Descriptions:        选择端点,并获取端点信息
** input parameters:    ucEpIndex   物理端点索引
** output parameters:   无
** Returned value:      对应端点的信息
*********************************************************************************************************/
UINT8 __usbDevSelectEndpoint (UINT8 ucEpIndex)
{
    UINT32 ulTmp = (UINT32)(ucEpIndex << 16);

    __usbDevSendCmd(ulTmp | USBCMD_SEL_ENDP, 0);

    return __usbDevGetData(ulTmp | USBDAT_SEL_ENDP);
}

/*********************************************************************************************************
** Function name:       __usbDevSelectClrIntEndpoint
** Descriptions:        选择端点,并获取端点信息和清除对应端点的中断
** input parameters:    ucEpIndex   物理端点号
** output parameters:   无
** Returned value:      对应端点的信息
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
** Descriptions:        设置端点状态
** input parameters:    ucEpIndex       物理端点号
**                      ucStalled   端点停止位
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSetEndpointStatus (UINT8 ucEpIndex, UINT8 ucStalled)
{
    UINT32 ulTmp1 = (UINT32)((ucEpIndex + 0x40) << 16);
    UINT32 ulTmp2 = (UINT32)(ucStalled << 16);

    __usbDevSendCmd(ulTmp1 | USBCMD_SET_ENDP_STATUS, ulTmp2 | USBDAT_SET_ENDP_STATUS);
}

/*********************************************************************************************************
** Function name:       __usbDevClearBuffer
** Descriptions:        清除OUT端点缓冲区
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
UINT8 __usbDevClearBuffer (void)
{
    __usbDevSendCmd(USBCMD_CLEAR_BUFFER, 0);                            /*  该命令没有数据阶段          */

    return __usbDevGetData(USBDAT_CLEAR_BUFFER);
}

/*********************************************************************************************************
** Function name:       __usbDevValidBuffer
** Descriptions:        使能 IN 端点缓冲区
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevValidBuffer (void)
{
    __usbDevSendCmd(USBCMD_VALID_BUFFER, 0);
}

/*********************************************************************************************************
** Function name:       __usbDevSoftDisconnect
** Descriptions:        断开与USB总线的连接
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSoftDisconnect (void)
{
    __usbDevSetDevStatus(0x00);
}

/*********************************************************************************************************
** Function name:       __usbDevSoftConnect
** Descriptions:        连接到USB总线
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSoftConnect (void)
{
    __usbDevSetDevStatus(0x01);
}

/*********************************************************************************************************
** Function name:       __usbDevSoftReconnect
** Descriptions:        重新连接USB总线
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void __usbDevSoftReconnect (void)
{
    UINT32 clk_cnt;

    __usbDevSoftDisconnect();                                           /*  断开与USB总线的连接         */

    for (clk_cnt = 0;clk_cnt<= 0x1FFFF;clk_cnt++);                      /*  延时                        */

    __usbDevSoftConnect();                                              /*  连接到USB总线               */
}

/*********************************************************************************************************
  端点数据传输
*********************************************************************************************************/

/*********************************************************************************************************
** Function name:       usbDevReadEndpoint
** Descriptions:        通过指定物理端点读取数据
** input parameters:    ucEpIndex       物理端点号
**                      pucRcvBuf       接收缓冲区指针
**                      ucReadLen       要读的字节长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
UINT8 usbDevReadEndpoint (UINT8 ucEpIndex,
                          UINT8 *pucRcvBuf,
                          UINT8 ucReadLen)
{
    UINT32 ulTmp    = 0;
    UINT32 i        = 0;
    UINT32 j        = 0;
    UINT32 ulRcvLen = 0;                                                /*  FIFO中已收到的字节数        */

    __USBCtrl = (UINT32)((ucEpIndex >> 1) << 2 | 0x01);                 /*  选择逻辑端点并使能读操作    */

    if((ucEpIndex % 2 == 0)) {
        while((__RxPacketLen & (0x3 << 10)) == 0);                      /*  等待数据就绪                */
    }

    ulRcvLen =(UINT32)(__RxPacketLen & 0x3FF);                          /*  获取收到的数据的字节长度    */

    if (ulRcvLen > ucReadLen) {
        ulRcvLen = ucReadLen;
    }
                                                                        /*  从 EP RAM 中读取数据        */
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

    __DevIntClr = __RxENDPKT;                                             /*  清除 RxENDPKT 位            */

    __usbDevSelectEndpoint(ucEpIndex);
    __usbDevClearBuffer();                                              /*  清除OUT缓冲区               */

    __USBCtrl = 0;
    return ((UINT8)ulRcvLen);                                           /*  返回实际收到的字节数        */
}

/*********************************************************************************************************
** Function name:       usbDevWriteEndpoint
** Descriptions:        向指定物理端点写入数据
** input parameters:    ucEpIndex       物理端点号
**                      pucWriteBuf     发送缓冲区
**                      ucWriteLen      要写的字节长度
** output parameters:   无
** Returned value:      实际写入的字节数
*********************************************************************************************************/
UINT8 usbDevWriteEndpoint (UINT8 ucEpIndex,
                           UINT8 *pucWriteBuf,
                           UINT8 ucWriteLen)
{
    UINT32 ulTmp = 0;
    UINT32 i     = 0;
    UINT32 j     = 0;

    ulTmp = ((ucEpIndex >> 1) << 2) | 0x02;                             /*  选择逻辑端点并使能写操作    */
    __USBCtrl = (UINT32)ulTmp;


    __TxPacketLen = (UINT32)ucWriteLen;                                     /*  写入要发送的数据的字节长度  */

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

        while((__TxPacketLen & 0x3FF) != 0);                                /*  等待 __TxPacketLen 为 0         */
    }

    if (ucWriteLen != 0) {

        while((__DevIntSt & __TxENDPKT) ==0);                         /*  等待写入完成                */
        __DevIntClr = __TxENDPKT;
    }

    __usbDevSelectEndpoint(ucEpIndex);                                  /*  使能发送缓冲区              */
    __usbDevValidBuffer();

    __USBCtrl = 0;
    return ucWriteLen;                                                  /*  返回实际写入长度            */
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
