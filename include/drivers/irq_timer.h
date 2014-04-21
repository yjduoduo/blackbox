#ifndef IRQ_TIMER_H
#define IRQ_TIMER_H
#include  "type.h"




extern void delayMs(BYTE timer_num, DWORD delayInMs);
extern void TIMER0_IRQHandler (void);
extern void TIMER1_IRQHandler (void);
extern void enable_timer( BYTE timer_num );
extern void disable_timer( BYTE timer_num );
extern void reset_timer( BYTE timer_num );

#endif // IRQ_TIMER_H
