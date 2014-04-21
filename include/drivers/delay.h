#ifndef DELAY_H
#define DELAY_H

#include "includeall.h"

#ifdef EXTERN_DELAY
#define EXT_DELAY
#else
#define EXT_DELAY extern
#endif






EXT_DELAY void SysTick_Handler(void);
EXT_DELAY void Delay1Ms(uint32 t);

EXT_DELAY void delay10cyc(void);


#ifdef DELAY_VAR
volatile uint32 msTicks;                            /* counts 1ms timeTicks */


#endif




#endif // DELAY_H
