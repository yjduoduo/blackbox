#include "irq_timer.h"
#include "LPC17xx.h"		/* LPC23xx/24xx Peripheral Registers	*/
#include "type.h"
#include "led.h"


volatile DWORD timer0_counter = 0;
volatile DWORD timer1_counter = 0;


/******************************************************************************
** Function name:		Timer0Handler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**						executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler (void)
{
    T0IR = 1;			/* clear interrupt flag */
    // IENABLE;			/* handles nested interrupt */

    led_test_1sonoff();

    timer0_counter++;

    // IDISABLE;
    // VICVectAddr = 0;	/* Acknowledge Interrupt */
}

/******************************************************************************
** Function name:		Timer1Handler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**						executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
    T1IR = 1;			/* clear interrupt flag */
    // IENABLE;			/* handles nested interrupt */

    timer1_counter++;

    // IDISABLE;
    // VICVectAddr = 0;	/* Acknowledge Interrupt */
}
