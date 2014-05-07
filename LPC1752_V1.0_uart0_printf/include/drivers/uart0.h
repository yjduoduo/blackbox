#ifndef UART0_H
#define UART0_H

#include "includeall.h"

#ifdef UART0_EXT
#define EXT_UART0
#else
#define EXT_UART0 extern
#endif


#define IER_RBR		0x01//Ω” ’÷–∂œ
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


//public declaretion
EXT_UART0 void uart0_init(uint32 baudrate);
EXT_UART0 void uart0_enable(void);
EXT_UART0 void uart0_disable(void);
EXT_UART0 void uart0_getstring (uint32 *pucStr, uint32 ulNum);
EXT_UART0 void uart0_sendstring (char *s);


#endif // UART0_H
