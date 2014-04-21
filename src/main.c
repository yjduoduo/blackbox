#include "includeall.h"
#include "hardware_sets.h"
#include "test_hw.h"




void hello_blackbox(void)
{
//     Debug("===========hello,black box!!!===========\n");
    Debug("SystemFrequency:%d\n",SystemFrequency);
}


int main(void)
{

    hardware_init();
    hello_blackbox();

    testall();
		

    while(1)
    {
      
    }

}
