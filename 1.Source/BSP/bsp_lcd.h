#ifndef BSP_LCD_H
#define BSP_LCD_H

#include <stdint.h>

/* Function prototype */
void BSP_LCD_Init(void);
void LCD_DelayUs(uint32_t us);
void LCD_DelayMs(uint32_t ms);
#endif /* BSP_LCD_H */