#include "sys_time.h"
#include "types_def.h"
#include <avr/interrupt.h>

static volatile uint32 sys_count = 0;

void update_sys_count(void)
{
    sys_count++;
}

static uint32 get_sys_count_atomic(void)
{
    uint32 count;
    uint8 sreg = SREG;
    cli();
    count = sys_count;
    SREG = sreg;
    return count;
}

void set_timer(uint32 *timer, uint16 val)
{
    if (timer != 0)
    {
        if (val == 0)
        {
            *timer = 0;  // Stop timer
        }
        else
        {
            uint32 now = get_sys_count_atomic();
            *timer = now + (uint32)val;
            if (*timer == 0)
            {
                *timer = 1; // Avoid 0 as 0 indicates stopped timer
            }
        }
    }
}

uint8 isTimerexpired(uint32 *timer)
{
    if (timer == 0 || *timer == 0)
    {
        return TIMERUNFIRED;
    }

    uint32 now = get_sys_count_atomic();
    /* Signed difference handles 32-bit wrap-around correctly */
    if ((sint32)(now - *timer) >= 0)
    {
        return TIMERFIRED; // Timer is expired
    }
    return TIMERUNFIRED;   // Timer is running
}