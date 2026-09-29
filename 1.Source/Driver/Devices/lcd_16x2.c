#include "lcd_16x2.h"
#include <stdint.h>

/* Macro WEAK definition */
#ifndef LCD_WEAK
#define LCD_WEAK __attribute__((weak))
#endif

/** 
 *  Hardware - WEAK FUNCTION 
 */

/* Pin RS, EN controller */
LCD_WEAK void LCD_RS_HIGH(void) { (void)0; }
LCD_WEAK void LCD_RS_LOW(void) { (void)0; }
LCD_WEAK void LCD_EN_HIGH(void) { (void)0; }
LCD_WEAK void LCD_EN_LOW(void) { (void)0; }

/* Write to 4 data bus D4...D7 */
LCD_WEAK void LCD_WriteBus4(uint8_t nibble)
{
	(void)nibble;   /* BSP override */
}



LCD_WEAK void LCD_DelayMs(uint32_t ms)
{
    (void)ms;
}

LCD_WEAK void LCD_DelayUs(uint32_t us)
{
    (void)us;
}

/**
 * Core of LCD
 */

 static void LCD_PulseEnable(void);
 static void LCD_Write4Bits(uint8_t nibble);
 static void LCD_Send(uint8_t value, uint8_t isData);

 /* Send Enable pulse */
 static void LCD_PulseEnable(void)
 {
    LCD_EN_HIGH();
    LCD_DelayUs(1);
    LCD_EN_LOW();
    LCD_DelayUs(50);
 }


/* Write nibble on bus D4...D7 */
static void LCD_Write4Bits(uint8_t nibble)
{
    LCD_WriteBus4(nibble & 0x0F);  /* Use only 4-bits low */
    LCD_PulseEnable();
}

/* Send 1 byte: divide 1 byte into 2 nibble */
static void LCD_Send(uint8_t value, uint8_t isData)
{
    if(isData)
    {
        LCD_RS_HIGH();
    }
    else
    {
        LCD_RS_LOW();
    }
    LCD_Write4Bits(value >> 4); /* Send high nibble */
    LCD_Write4Bits(value & 0x0F); /* Send low nibble */

    if(!isData && (value == 0x01u || value == 0x02u))
    {
        LCD_DelayMs(2); /* Clear display or Return home */
    }

}

/**
 * Public API function
 */

 /* Send command to LCD */
 void LCD_SendCommand(uint8_t cmd)
 {
    LCD_Send(cmd, 0u);
 }
 /* Send data to LCD */
 void LCD_SendData(uint8_t data)
 {
    LCD_Send(data, 1u);
 }

 /* LCD initialize*/
 void LCD_Init(void)
 {
    LCD_DelayMs(40); /* Wait for LCD after power-on (datasheet >~ 15ms) */

    LCD_RS_LOW();
    LCD_EN_LOW();

    LCD_Write4Bits(0x03u);
    LCD_DelayMs(5);

    LCD_Write4Bits(0x03u);
    LCD_DelayUs(150);

    LCD_Write4Bits(0x03u);
    LCD_DelayUs(150);

    /* Switch to 4-bit mode */
    LCD_Write4Bits(0x02u);
    LCD_DelayUs(150);

    /* Function Set: 4-bit, 2 lines, 5x8 font */
    LCD_SendCommand(0x28u);

    /* Display OFF: turn off display, cursor, blink */
    LCD_SendCommand(0x08u);

    /* Clear display */
    LCD_Clear();

    /* Entry Mode Set*/
    LCD_SendCommand(0x06u);

    /* Display ON: turn on display, turn off cursor and blink */
    LCD_SendCommand(0x0Cu);
}

/* Clear LCD */
void LCD_Clear(void)
{
    LCD_SendCommand(0x01u);   /* Clear display */
}

/* Put the pointer to home (0,0) */
void LCD_Home(void)
{
    LCD_SendCommand(0x02u); /* Return home */
}

/* Put a character to postition's pointer */
void LCD_PutChar(char c)
{
    LCD_SendData((uint8_t) c);
}
void LCD_PutString(const char *s)
{
    if(s == 0) return;
    while (*s != '\0')
    {
        LCD_PutChar(*s++);
    }
}

/**
 * Move pointer to
 * row: 0 or 1
 * col: 0 to 15
 */
void LCD_GotoXY(uint8_t col, uint8_t row)
{
    uint8_t addr;

    if (col > 15u) {
        col = 15u;
    }

    switch (row) {
    case 0u:
        addr = 0x00u + col;
        break;
    case 1u:
    default:
        addr = 0x40u + col;   /* Line 2 begin at address DDRAM 0x40 */
        break;
    }

    LCD_SendCommand(0x80u | addr);  /* Set DDRAM address */
}