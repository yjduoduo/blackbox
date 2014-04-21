#include "timer_all.h"

void timer_all_init(uint8 timer_num, uint32 TimerInterval)
{
    if ( timer_num == 0 )
    {
        PCONP |= (0x01<<1);
        T0CTCR = 0x00;
        T0TCR  = 0x02;
        T0IR   = 1;
        T0TC   = 0;
        T0PR   = 0;
        T0MR0 = TimerInterval;//cTime0V_2ms;	//100us
        T0MCR = 3;				/* Interrupt and Reset on MR0 */
        T0TCR = 1;
        NVIC_EnableIRQ(TIMER0_IRQn);
        NVIC_SetPriority(TIMER0_IRQn, PRIO_TWO); //中断优先级

    }
    if ( timer_num == 1 )
    {
        PCONP |= (0x1<<2);
        T1CTCR = 0x00;
        T1TCR  = 0x02;
        T1IR   = 1;
        T1TC   = 0;
        T1PR   = 0;
        T1MR0 = TimerInterval;//SystemFrequency/40000;//100us
        T1MCR = 3;				/* Interrupt and Reset on MR0 */

        NVIC_EnableIRQ(TIMER1_IRQn);
        NVIC_SetPriority(TIMER1_IRQn, PRIO_SIX); //中断优先级
        T1TCR = 1;
    }
    if ( timer_num == 2 )
    {
        PCONP |= (0x1<<22);
        T2CTCR = 0x00;
        T2TCR  = 0x02;
        T2IR   = 1;
        T2TC   = 0;
        T2PR   = 0;
        T2MR0 = TimerInterval;//cTime0V_2ms;	//100us
        T2MCR = 3;				/* Interrupt and Reset on MR0 */
        NVIC_EnableIRQ(TIMER2_IRQn);
        NVIC_SetPriority(TIMER2_IRQn, PRIO_NINE); //中断优先级
        T2TCR = 1;
    }
    if ( timer_num == 3 )
    {
        PCONP |= (0x1<<23);
        T3CTCR = 0x00;
        T3TCR  = 0x02;
        T3IR   = 1;
        T3TC   = 0;
        T3PR   = 0;
        T3MR0 = TimerInterval;//cTime0V_2ms;	//100us
        T3MCR = 3;				/* Interrupt and Reset on MR0 */
        NVIC_EnableIRQ(TIMER3_IRQn);
        NVIC_SetPriority(TIMER3_IRQn, PRIO_TEN); //中断优先级
        T3TCR = 1;
    }
}


/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
**
******************************************************************************/
void enable_timer( uint8 timer_num )
{
    if ( timer_num == 0 )
    {
        T0TCR = 1;
    }
    else if ( timer_num == 1 )
    {
        T1TCR = 1;
    }
    else if ( timer_num == 2 )
    {
        T2TCR = 1;
    }
    else if ( timer_num == 3 )
    {
        T3TCR = 1;
    }
    return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1 oe 2 or 3
** Returned value:		None
**
******************************************************************************/
void disable_timer( uint8 timer_num )
{
    if ( timer_num == 0 )
    {
        T0TCR = 0;
    }
    else if ( timer_num == 1 )
    {
        T1TCR = 0;
    }
    else if ( timer_num == 2 )
    {
        T2TCR = 0;
    }
    else if ( timer_num == 3 )
    {
        T3TCR = 0;
    }
    return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
**
******************************************************************************/
void reset_timer( uint8 timer_num )
{
    uint32 regVal;

    if ( timer_num == 0 )
    {
        regVal = T0TCR;
        regVal |= 0x02;
        T0TCR = regVal;
    }
    else if ( timer_num == 1 )
    {
        regVal = T1TCR;
        regVal |= 0x02;
        T1TCR = regVal;
    }
    else if ( timer_num == 2 )
    {
        regVal = T2TCR;
        regVal |= 0x02;
        T2TCR = regVal;
    }
    else if ( timer_num == 3 )
    {
        regVal = T3TCR;
        regVal |= 0x02;
        T3TCR = regVal;
    }
    return;
}



/*****************************************************************************
** Function name:		delayMs
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second
**
** Returned value:		None
**
*****************************************************************************/
void delayMs(BYTE timer_num, DWORD delayInMs)
{
    if ( timer_num == 0 )
    {
        /*
    * setup timer #0 for delay
    */
        T0TCR = 0x02;		/* reset timer */
        T0PR  = 0x00;		/* set prescaler to zero */
        //	T0MR0 = delayInMs * (Fpclk / 1000-1);
        T0MR0 = delayInMs * (SystemFrequency/4 / 1000-1);
        T0IR  = 0xff;		/* reset all interrrupts */
        T0MCR = 0x04;		/* stop timer on match */
        T0TCR = 0x01;		/* start timer */

        /* wait until delay time has elapsed */
        while (T0TCR & 0x01);
    }
    else if ( timer_num == 1 )
    {
        /*
    * setup timer #1 for delay
    */
        T1TCR = 0x02;		/* reset timer */
        T1PR  = 0x00;		/* set prescaler to zero */
        //	T1MR0 = delayInMs * (Fpclk / 1000-1);
        T1MR0 = delayInMs * (SystemFrequency/4 / 1000-1);
        T1IR  = 0xff;		/* reset all interrrupts */
        T1MCR = 0x04;		/* stop timer on match */
        T1TCR = 0x01;		/* start timer */

        /* wait until delay time has elapsed */
        while (T1TCR & 0x01);
    }
    return;
}


