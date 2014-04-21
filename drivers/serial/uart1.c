/****************************************************************************
 *   $Id:: uart.c 5751 2010-11-30 23:56:11Z usb00423                        $
 *   Project: NXP LPC17xx UART example
 *
 *   Description:
 *     This file contains UART code example which include UART initialization,
 *     UART interrupt handler, and APIs for UART access.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "lpc17xx.h"
#include "type.h"
#include "uart1.h"


#define UART1_PCONP_BIT 4



void uart1_pconp_conf(void)
{
    PCONP |= (1<<UART1_PCONP_BIT);
}
void uart1_pin_conf(void)
{
    PINSEL4 &= ~0x0000000F;  //p2.0,p2.1 as 00
    PINSEL4 |= 0x0000000A;	/* Enable RxD1 P2.1, TxD1 P2.0 as(10 10)*/
}


/*****************************************************************************
** Function name:		uartn_init
**
** Descriptions:		Initialize UART port, setup pin select,
**						clock, parity, stop bits, FIFO, etc.
**
** parameters:			portNum(0 or 1) and UART baudrate
** Returned value:		true or false, return false only if the 
**						interrupt handler can't be installed to the 
**						VIC table
** 
*****************************************************************************/
void uart1_init(uint32 baudrate )
{
    uint32 Fdiv;
    uint32 pclkdiv, pclk;
    uart1_pconp_conf();
    uart1_pin_conf();

    /* By default, the PCLKSELx value is zero, thus, the PCLK for
    all the peripherals is 1/4 of the SystemFrequency. */
    /* Bit 8,9 are for UART1 */
    pclkdiv = (PCLKSEL0 >> 8) & 0x03;
    switch ( pclkdiv )
    {
    case 0x00:
    default:
        pclk = SystemFrequency/4;
        break;
    case 0x01:
        pclk = SystemFrequency;
        break;
    case 0x02:
        pclk = SystemFrequency/2;
        break;
    case 0x03:
        pclk = SystemFrequency/8;
        break;
    }

    U1LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
    Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
    U1DLM = Fdiv / 256;
    U1DLL = Fdiv % 256;
    U1LCR = 0x03;		/* DLAB = 0 */
    U1FCR = 0x07;		/* Enable and reset TX and RX FIFO 1个字节触发. */
    //     U1FCR = 0xe7;		/* Enable and reset TX and RX FIFO e7,14个字节触发. 97,9个字节触发.*/



    U1IER = IER_RBR;//（RBR 使能接收中断） | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */

    //    U1IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */

    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, PRIO_ONE); //中断优先级


}

void uart1_enable(void)
{
    U1IER = IER_RBR;
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, PRIO_ONE); //中断优先级
}

void uart1_disable(void)
{
    NVIC_DisableIRQ(UART1_IRQn);
}


uint32 uart1_sendbyte(uint32 buf)
{  
    //当检测到UARTn THR已空时，THRE就会立即被设置。写UnTHR会清零THRE
    //0  -  UnTHR包含有效字符
    //1  -  UnTHR为空
    while(!((U1LSR) & 0x20));       //等待判断LSR[5](即THRE)是否是1,1时表示THR中为空
    return (U1THR = buf);  //发送数据
}  

void uart1_sendstring (char *s)
{
    while ( *s != 0 )
    {
        uart1_sendbyte(*s++);
    }
}

uint32 uart1_getbyte(void)
{
    uint32 ucRcvData;

    while ((U1LSR & 0x01) == 0);                                        /* 等待接收标志置位             */
    ucRcvData = U1RBR;                                                  /* 读取数据                     */
    return (ucRcvData);
}

void uart1_getstring (uint32 *pucStr, uint32 ulNum)
{
    for (; ulNum > 0; ulNum--){
        *pucStr++ =  uart1_getbyte();
    }
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART 0 port based
**						on the data length
**
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
**
*****************************************************************************/
void UARTSend( uint32 portNum, uint8 *Ptr, uint32 Length  )
{
    if ( portNum == 0 )
    {
        while ( Length != 0 )
        {
            /* THRE status, contain valid data */
            //while ( !(UART0TxEmpty & 0x01) );
            U0THR = *Ptr++;
            //UART0TxEmpty = 0;	/* not empty in the THR until it shifts out */
            //BufferPtr++;
            Length--;
        }
    }
    else
    {
        while ( Length != 0 )
        {
            /* THRE status, contain valid data */
            // while ( !(UART1TxEmpty & 0x01) );
            U1THR =  *Ptr++;
            //UART1TxEmpty = 0;	/* not empty in the THR until it shifts out */
            //BufferPtr++;
            Length--;
        }
    }
    return;
}






/******************************************************************************
**                            End Of File
******************************************************************************/

