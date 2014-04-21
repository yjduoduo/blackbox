#ifndef TIMER_ALL_H
#define TIMER_ALL_H

#include "includeall.h"



#ifdef EXTERN_TIMER_ALL
#define EXT_TIMER_ALL
#else
#define EXT_TIMER_ALL extern
#endif
//public func declaretion
EXT_TIMER_ALL void timer_all_init(uint8 timer_num, uint32 TimerInterval);
EXT_TIMER_ALL void enable_timer( uint8 timer_num );
EXT_TIMER_ALL void disable_timer( uint8 timer_num );
EXT_TIMER_ALL void reset_timer( uint8 timer_num );



#endif // TIMER_ALL_H
