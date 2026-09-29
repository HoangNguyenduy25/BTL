#include "lcd_16x2.h"
#include "data_dht.h"
#include "gpio.h"
#include <stdio.h>

void App_LCD_Update(void)
{
    char line[17];
    uint8_t temp = data_dht_get_temperature();
    uint8_t hum  = data_dht_get_humidity();
    uint8_t led_state = GPIO_readPin(PORTB_ID, PIN0_ID);
    uint8_t target_temp = data_dht_get_target_temperature();

    // Dong 1: Nhiet do moi truong (T: xx C), Do am (H: xx%) va Trang thai LED (ON/OFF)
    LCD_GotoXY(0, 0);
    if (temp > 0)
    {
        sprintf(line, "T:%2d%cC H:%2d%% %3s", temp, 0xDF, hum, (led_state == LOGIC_HIGH) ? " ON" : "OFF");
    }
    else
    {
        sprintf(line, "T:--%cC H:--%% %3s", 0xDF, (led_state == LOGIC_HIGH) ? " ON" : "OFF");
    }
    LCD_PutString(line);

    // Dong 2: Nhiet do cai dat tu PC (Vi du: "TEMP = 27")
    LCD_GotoXY(0, 1);
    sprintf(line, "TEMP = %2d       ", target_temp);
    LCD_PutString(line);
}