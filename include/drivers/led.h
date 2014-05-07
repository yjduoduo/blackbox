#ifndef LED_H
#define LED_H

#include "includeall.h"

#ifdef EXTERN_LED
#define EXT_LED
#else
#define EXT_LED extern
#endif




EXT_LED void led_init(void);
EXT_LED void led_on(void);
EXT_LED void Led_Off(void);

EXT_LED void Led_USB_UP_Init(void);
EXT_LED void Led_USB_UP_On(void);
EXT_LED void Led_USB_UP_Off(void);

//EXT_LED void Led_Silence_Init(void);
//EXT_LED void Led_Silence_On(void);
//EXT_LED void Led_Silence_Off(void);
////Í¨Ñ¶p2.4
//EXT_LED void Led_Wireless_Init(void);
//EXT_LED void Led_Wireless_On(void);
//EXT_LED void Led_Wireless_Off(void);

////¹ÊÕÏp2.5
//EXT_LED void Led_Fault_Init(void);
//EXT_LED void Led_Fault_On(void);
//EXT_LED void Led_Fault_Off(void);

////»ð¾¯p2.6
//EXT_LED void Led_Fire_Init(void);
//EXT_LED void Led_Fire_On(void);
//EXT_LED void Led_Fire_Off(void);




#endif // LED_H
