#include "lcd_16x2.h"
#include "bsp_lcd.h"
#include "board.h"
#include <avr/io.h>
#include <util/delay.h>

#define LCD_PORT PORTA
#define LCD_DDR  DDRA
#define LCD_RS   PA0
#define LCD_EN   PA1
#define LCD_D4   PA4
#define LCD_D5   PA5
#define LCD_D6   PA6
#define LCD_D7   PA7

/* Public Function */
void BSP_LCD_Init(void)
{
    /* PA0 (RS), PA1 (EN), PA4..PA7 (D4..D7) are outputs */
    LCD_DDR |= (1 << LCD_RS) | (1 << LCD_EN) |
               (1 << LCD_D4) | (1 << LCD_D5) |
               (1 << LCD_D6) | (1 << LCD_D7);

    /* Initialize LCD */
    LCD_Init();
}

/* Override weak functions of LCD driver */
void LCD_RS_HIGH(void)
{
    LCD_PORT |= (1 << LCD_RS);
}

void LCD_RS_LOW(void)
{
    LCD_PORT &= ~(1 << LCD_RS);
}

void LCD_EN_HIGH(void)  
{
    LCD_PORT |= (1 << LCD_EN);
}

void LCD_EN_LOW(void)
{
    LCD_PORT &= ~(1 << LCD_EN);
}

void LCD_WriteBus4(uint8_t nibble)
{
    LCD_PORT = (uint8_t)((LCD_PORT & 0x0F) | ((nibble & 0x0F) << 4));
}

/* Accurate delay functions */
void LCD_DelayMs(uint32_t ms)
{
    while(ms--)
    {
        _delay_ms(1);
    }
}

void LCD_DelayUs(uint32_t us)
{
    while(us--)
    {
        _delay_us(1);
    }
}
