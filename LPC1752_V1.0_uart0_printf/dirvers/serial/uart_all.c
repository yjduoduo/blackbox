#include "uart_all.h"





void uart_all_enable(void)
{
    uart0_enable();
    uart1_enable();
}
void uart_all_disable(void)
{
    uart0_disable();
    uart1_disable();
}
