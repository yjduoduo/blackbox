#include "includeall.h"
#include "hardware_sets.h"




void hello_blackbox(void)
{
//     lcd_printf("===========hello,black box!!!===========\n");
    lcd_printf("SystemFrequency:%d\n",SystemFrequency);
}


int main(void)
{ 

    hardware_init();
    hello_blackbox();

    flash_total_test();

    while(1)
    {
        flash_total_test();
      
        led_shrinking();
    }

}
