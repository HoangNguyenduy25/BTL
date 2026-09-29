#ifndef SYS_TIME_H
#define SYS_TIME_H
#define TIMERFIRED   0u
#define TIMERUNFIRED 1u

#include "stdint.h"
#include "types_def.h"
#include "avr/io.h"

void update_sys_count(void);
void set_timer(uint32 *timer, uint16 val);
uint8 isTimerexpired(uint32 *timer);
#endif /* SYS_TIME_H*/