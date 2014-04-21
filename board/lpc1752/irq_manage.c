#include "irq_manage.h"


void irq_disable_all(void)
{
    disable_timer(0);
}
void irq_enable_all(void)
{
    enable_timer(0);
}
