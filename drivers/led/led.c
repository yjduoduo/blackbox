#include "led.h"


void led_init(void)
{
    Led_USB_UP_Init();
}

void led_on(void)
{
    Led_USB_UP_On();
}
void Led_Off(void)
{
    Led_USB_UP_Off();
}
void Led_USB_UP_Init(void)
{
#if 0
    //USB_UP_LED
    PINSEL3 &= ~(0x3<<4);//clear P1.18
    PINSEL3 |= (0x1<<4);//P1.18,Func[1]
#else
    //as gpio func
    PINSEL3 &= ~(0x3<<4);//clear P1.18
    PINSEL3 |= (0x0<<4);//P1.18,Func[0]
    FIO1DIR |= (0x1<<18);//dir P1.18,output
    FIO1CLR &= ~(0x1<<18);//level P1.18,low

#endif

}
void Led_USB_UP_On(void)
{
    FIO1CLR |= (1<<18);

}
void Led_USB_UP_Off(void)
{
    FIO1SET |= (1<<18);
}

void led_test_1sonoff(void)
{
    static uint32 num=0;
    if(num%2)
    {
        Led_USB_UP_On();
    }else
        Led_USB_UP_Off();

    if(!(num%0x9))
        Debug("~~~~running\n");

    num++;


}


////¾²Òôp0.16
//void Led_Silence_Init(void)
//{
//    //gpio,output,high
//    PINSEL1 &= ~(1<<0);//16bit gpio
//    FIO0DIR |= (1<<16);//16bit output
//    FIO0SET |= (1<<16);//16bit high
//}
//void Led_Silence_On(void)
//{
//    FIO0SET |= (1<<16);
//}
//void Led_Silence_Off(void)
//{
//    FIO0CLR |= (1<<16);
//}
////Í¨Ñ¶p2.4
//void Led_Wireless_Init(void)
//{
//    PINSEL4 &=~(1<8);
//    FIO2DIR |= (1<<4);
//    FIO2SET |= (1<<4);
//}
//void Led_Wireless_On(void)
//{
//    FIO2SET |= (1<<4);
//}
//void Led_Wireless_Off(void)
//{
//    FIO2CLR |= (1<<4);
//}

////¹ÊÕÏp2.5
//void Led_Fault_Init(void)
//{
//    PINSEL4 &=~(1<10);
//    FIO2DIR |= (1<<5);
//    FIO2SET |= (1<<5);
//}
//void Led_Fault_On(void)
//{
//    FIO2SET |= (1<<5);
//}
//void Led_Fault_Off(void)
//{
//    FIO2CLR |= (1<<5);
//}

////»ð¾¯p2.6
//void Led_Fire_Init(void)
//{
//    PINSEL4 &=~(1<12);
//    FIO2DIR |= (1<<6);
//    FIO2SET |= (1<<6);
//}
//void Led_Fire_On(void)
//{
//    FIO2SET |= (1<<6);
//}
//void Led_Fire_Off(void)
//{
//    FIO2CLR |= (1<<6);
//}

