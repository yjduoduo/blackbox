#ifndef INCLUDEALL_H
#define INCLUDEALL_H

//standard
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

//type
#include "type.h"

//system
#include "LPC17xx.h"
#include "lcd_printf.h"
#include "uart0.h"
#include "delay.h"
#include "timer_all.h" //定时器
#include "irq_manage.h"
#include "utils.h"


















//调试接口
#define DEBUG_SOFT
#define Debug lcd_printf




#endif // INCLUDEALL_H
