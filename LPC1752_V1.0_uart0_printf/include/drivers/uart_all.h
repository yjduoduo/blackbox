#ifndef UART_ALL_H
#define UART_ALL_H

#include "uart0.h"
#include "uart1.h"

#ifdef EXTERN_UART_ALL
#define EXT_UART_ALL
#else
#define EXT_UART_ALL extern
#endif


EXT_UART_ALL void uart_all_enable(void);
EXT_UART_ALL void uart_all_disable(void);



#endif // UART_ALL_H
