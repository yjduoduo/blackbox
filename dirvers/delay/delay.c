#define DELAY_VAR
#include "delay.h"





void SysTick_Handler(void)
{
    msTicks++;                        /* increment counter necessary in Delay() */
}
void DelayUs2x(unsigned char t)
{
    while(--t);
}
void delay10cyc(void)
{
    uint8 i;
    for(i=0;i<10;i++);
}
void Delay1Ms(uint32 t)
{
   while(t--)
   {
       //´óÖÂÑÓÊ±1mS
       DelayUs2x(245);
       DelayUs2x(245);
   }

}
