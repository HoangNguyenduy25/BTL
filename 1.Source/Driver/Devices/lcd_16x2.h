#ifndef LCD_16x2_H
#define LCD_16x2_H
#include "types_def.h"
#include <stdint.h>

/* Simple API for LCD */
void LCD_Init(void);
void LCD_Clear(void);
void LCD_Home(void);

void LCD_SendCommand(uint8 cmd);
void LCD_SendData(uint8 data);

void LCD_PutChar(char c);
void LCD_PutString(const char *c);
void LCD_GotoXY(uint8 col, uint8 row);
#endif /* LCD_16x2_H */