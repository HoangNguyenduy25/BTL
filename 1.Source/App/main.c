#include "board.h"
#include "bsp_led.h"
#include "bsp_lcd.h"
#include "bsp_dht.h"
#include "bsp_uart.h"
#include "bsp_timer.h"
#include "lcd_16x2.h"
#include "data_dht.h"
#include "sys_time.h"
#include "app_uart.h"
#include "app_lcd.h"
#include <avr/interrupt.h>

int main(void)
{
    // Khởi tạo phần cứng
    BSP_LED_Init();
    BSP_LCD_Init();

    BSP_DHT_Init();
    usart0_init();
    BSP_Timer1_Init();

    // Bật ngắt toàn cục
    sei();

    LCD_Clear();
    LCD_GotoXY(0, 0);
    LCD_PutString("System Ready!");

    uint32 timer_dht = 0;
    uint32 timer_lcd = 0;

    set_timer(&timer_dht, 2000); // Đọc DHT11 mỗi 2 giây
    set_timer(&timer_lcd, 500);  // Làm mới LCD mỗi 500ms

    while (1)
    {
        App_LCD_Update();
        // Xử lý gói tin và giao tiếp UART
        app_uart_process();

        // Read data from DHT11 sensor
        if (isTimerexpired(&timer_dht) == TIMERFIRED)
        {
            uint8_t hum = 0, temp = 0;
            if (BSP_DHT_Read(&hum, &temp) == 0)
            {
                data_dht_set_temperature(temp);
                data_dht_set_humidity(hum);
            }
            set_timer(&timer_dht, 2000);
        }

        // LCD Update
        if (isTimerexpired(&timer_lcd) == TIMERFIRED)
        {
            App_LCD_Update();
            set_timer(&timer_lcd, 500);
        }

        
          
    }
}