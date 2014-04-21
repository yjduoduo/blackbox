#ifndef HARDWARE_SETS_H
#define HARDWARE_SETS_H


//´®¿Ú0²¨ÌØÂÊ
#define UART0_BPS 9600
//#define UART0_BPS 115200

#ifdef HARDWARE_EXT
#define EXT_HARDWARE
#else
#define EXT_HARDWARE extern
#endif

//public function declaration
EXT_HARDWARE void hardware_init(void);




#endif // HARDWARE_SETS_H
