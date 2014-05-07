#include "lcd_printf.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart0.h"//uart0
#include "uart1.h"//uart1

//Ê¹ÓÃ´®¿Ú0
#define UART0_USED


void puts__(char *s)
{
#ifdef UART0_USED
    uart0_sendstring(s);
#else
    uart1_sendstring(s);
#endif
}
char  buf[256];
void lcd_printf(char *str,...)
{
#if 1
    va_list ptr;
    va_start(ptr,str);
    vsprintf(buf,str,ptr);
    puts__(buf);
    va_end(ptr);
#endif
}
