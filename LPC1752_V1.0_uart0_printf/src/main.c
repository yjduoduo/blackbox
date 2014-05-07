#include "includeall.h"
// #include "inc_all_hardware.h"
// #include "inc_for_src.h"


#define UART0_BPS 9600

void hello_blackbox(void)
{
    lcd_printf("===========hello,black box!!!===========\n");
    lcd_printf("SystemFrequency:%d\n",SystemFrequency);
}


int main(void)
{
    SystemClockUpdate();
    uart0_init(UART0_BPS);

    hello_blackbox();//welcome

    while(1)
    {
        lcd_printf("hello,black box!!!\n");
    }

}
