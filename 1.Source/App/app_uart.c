#include "app_uart.h"
#include "bsp_uart.h"
#include "bsp_led.h"
#include "data_dht.h"
#include "fifo.h"
#include "gpio.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CMD_BUFFER_SIZE 32

/*
 * Hàm xử lý các lệnh nhập vào từ PC
 */
static void app_uart_execute_command(char *cmd)
{
    if (cmd == NULL || cmd[0] == '\0')
    {
        return;
    }

    // Điều khiển LED
    if (strcmp(cmd, "LED ON") == 0 || strcmp(cmd, "LED:ON") == 0)
    {
        BSP_LED_On();
        UARTprintf("ACK:LED_ON\r\n");
    }
    else if (strcmp(cmd, "LED OFF") == 0 || strcmp(cmd, "LED:OFF") == 0)
    {
        BSP_LED_Off();
        UARTprintf("ACK:LED_OFF\r\n");
    }
    // Cài đặt nhiệt độ
    else if (strncmp(cmd, "TEMP=", 5) == 0 || strncmp(cmd, "SET_TEMP=", 9) == 0)
    {
        char *p_val = (cmd[0] == 'T') ? (cmd + 5) : (cmd + 9);
        int target = atoi(p_val);

        if (target >= 16 && target <= 30)
        {
            data_dht_set_target_temperature((uint8_t)target);
            UARTprintf("ACK:TEMP=%d\r\n", target);
        }
        else
        {
            UARTprintf("ERR:TEMP_OUT_OF_RANGE (16-30)\r\n");
        }
    }
    // Đọc nhiệt độ môi trường hiện tại từ DHT11
    else if (strcmp(cmd, "TEMP?") == 0 || strcmp(cmd, "GET:TEMP") == 0)
    {
        uint8_t temp = data_dht_get_temperature();
        UARTprintf("TEMP=%dC\r\n", temp);
    }
    // Đọc độ ẩm môi trường hiện tại từ DHT11
    else if (strcmp(cmd, "HUM?") == 0 || strcmp(cmd, "GET:HUM") == 0)
    {
        uint8_t hum = data_dht_get_humidity();
        UARTprintf("HUM=%d%%\r\n", hum);
    }
    // Lệnh không hợp lệ
    else
    {
        UARTprintf("ERR:UNKNOWN_CMD: %s\r\n", cmd);
    }
}

/** 
 * Hàm app_uart_process
 * Sử dụng usart0_rx_count() và usart0_read_char() từ bsp_uart.c để lấy từng ký tự từ FIFO RX.
 * Ghi ký tự vào cmd_buffer cho đến khi gặp ký tự kết thúc dòng ('\r' hoặc '\n').
 * Gọi app_uart_execute_command() để thực thi lệnh và gửi phản hồi qua UARTprintf() ra terminal.
 */
void app_uart_process(void)
{
    static char cmd_buffer[CMD_BUFFER_SIZE];
    static uint8_t cmd_idx = 0;

    // Đọc tất cả các ký tự hiện có trong RX FIFO
    while (usart0_rx_count() > 0)
    {
        uint16_t rx_data = usart0_read_char();

        // Kiểm tra cờ USART_NO_DATA
        if (rx_data & USART_NO_DATA)
        {
            break;
        }

        char c = (char)(rx_data & 0xFF);

        // Kiểm tra ký tự kết thúc dòng
        if (c == '\r' || c == '\n')
        {
            if (cmd_idx > 0)
            {
                cmd_buffer[cmd_idx] = '\0'; // Kết thúc chuỗi
                app_uart_execute_command(cmd_buffer);
                cmd_idx = 0; // Đặt lại index cho lệnh tiếp theo
            }
        }
        else
        {
            // Chỉ nhận các ký tự ASCII in được
            if (c >= ' ' && c <= '~')
            {
                if (cmd_idx < (CMD_BUFFER_SIZE - 1))
                {
                    cmd_buffer[cmd_idx++] = c;
                }
                else
                {
                    // Tràn buffer dòng lệnh
                    cmd_idx = 0; // reset buffer
                    UARTprintf("ERR:BUFFER_OVERFLOW\r\n");
                }
            }
        }
    }
}
