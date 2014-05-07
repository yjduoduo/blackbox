/****************************************************************************
 *   $Id:: uart.h 5751 2010-11-30 23:56:11Z usb00423                        $
 *   Project: NXP LPC17xx UART example
 *
 *   Description:
 *     This file contains UART code header definition.
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
#ifndef __UART1_H
#define __UART1_H

#include "includeall.h"


#ifdef EXT_FLAG
#define EXT_UART1
#else
#define EXT_UART1 extern
#endif

#define IER_RBR		0x01//�����ж�
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE		0x40


/***************************************************/
//public function declare
//uart init,0,1

EXT_UART1 void uart1_init(uint32 baudrate );
EXT_UART1 void uart1_enable(void);
EXT_UART1 void uart1_disable(void);
EXT_UART1 void uart1_getstring (uint32 *pucStr, uint32 ulNum);
EXT_UART1 void uart1_sendstring (char *s);


#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
