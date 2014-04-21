#include "lcd_printf.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart0.h"//uart0
#include "uart1.h"//uart1
#include "includeall.h"

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
static char  buf[128];
void lcd_printf(char *str,...)
{
#ifdef DEBUG_SOFT
    va_list ptr;
    va_start(ptr,str);
    vsprintf(buf,str,ptr);
    puts__(buf);
    va_end(ptr);
#endif

}
