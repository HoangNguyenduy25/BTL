#ifndef BSP_TIMER_H
#define BSP_TIMER_H

#include <stdint.h>

/* Timer 1 initilize */
void BSP_Timer1_Init(void);

uint32_t BSP_GetSysTimeMs(void);
/* Blocking delay in ms using Timer1/sys_time_count */
void BSP_DelayMs(uint32_t delayMs);
#endif /* BSP_TIMER_H_ */