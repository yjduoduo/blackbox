#define DELAY_VAR
#include "delay.h"
#include<LPC17XX.h>

void SysTickInit(void)
{
    // SysTick��ʱ����ʼ��
    if (SysTick_Config(0x05F5E100 / 1000)) /* Setup SysTick Timer for 1 msec interrupts */
    {
        while (1); /* Capture error */
    }

}



void SysTick_Handler(void)
{
    msTicks++;                        /* increment counter necessary in Delay() */
}
/*------------------------------------------------------------------------------
 delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
void Delay (uint32 dlyTicks)
{
    uint32_t curTicks;

    curTicks = msTicks; // ��ȡ��ǰֵ

    while ((msTicks - curTicks) < dlyTicks); //�ȴ�������ʱ
}


void DelayUs2x(unsigned char t)
{
    while(--t);
}
void Delay1Ms(uint32 t)
{
    while(t--)
    {
        //������ʱ1mS
        DelayUs2x(245);
        DelayUs2x(245);
    }

}
