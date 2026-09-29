#include "app_uart.h"
#include "fifo.h"
#include "bsp_led.h"
#include "data_dht.h"
#include "uart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char cmd_buf[32];
static uint8_t cmd_idx = 0;

void app_uart_process(void)
{
    while (ring_buffer_status(&rx_fifo) != RING_BUF_EMPTY)
    {
        /* get character from fifo */
        char c = (char)fifo_char_get(&rx_fifo); 

        if (c == '\r' || c == '\n') /* Check the newline character*/
        {
            if (cmd_idx > 0) /* Check the command is empty or not */
            {
                cmd_buf[cmd_idx] = '\0';

                // Command On/Off LED
                if (strcmp(cmd_buf, "LED:1") == 0 || strcmp(cmd_buf, "LED:ON") == 0 || strcmp(cmd_buf, "LED ON") == 0)
                {
                    BSP_LED_On();
                    UART_sendString((const uint8*)"ACK:LED_ON\r\n");
                }
                else if (strcmp(cmd_buf, "LED:0") == 0 || strcmp(cmd_buf, "LED:OFF") == 0 || strcmp(cmd_buf, "LED OFF") == 0)
                {
                    BSP_LED_Off();
                    UART_sendString((const uint8*)"ACK:LED_OFF\r\n");
                }
                // Config temperature
                else if (strncmp(cmd_buf, "SET_TEMP=", 9) == 0 || strncmp(cmd_buf, "SET_TEMP =", 10) == 0)
                {
                    char *val_str = strchr(cmd_buf, '=');
                    if (val_str != NULL)
                    {
                        val_str++; 
                        while (*val_str == ' ') val_str++; // Bo qua dau cach neu co
                        int val = atoi(val_str);
                        if (val >= 16 && val <= 30)
                        {
                            data_dht_set_target_temperature((uint8_t)val);
                            char resp[24];
                            sprintf(resp, "ACK:TEMP=%d\r\n", val);
                            UART_sendString((const uint8*)resp);
                        }
                        else
                        {
                            UART_sendString((const uint8*)"ERR:OUT_OF_RANGE_16_TO_30\r\n");
                        }
                    }
                }
                // Read Temp from DHT11
                else if (strcmp(cmd_buf, "TEMP?") == 0 || strcmp(cmd_buf, "TEMP") == 0 || strcmp(cmd_buf, "GET:TEMP") == 0)
                {
                    char resp[20];
                    uint8_t t = data_dht_get_temperature();
                    sprintf(resp, "TEMP:%dC\r\n", t);
                    UART_sendString((const uint8*)resp);
                }
                // Read hum from DHT11
                else if (strcmp(cmd_buf, "HUM?") == 0 || strcmp(cmd_buf, "HUM") == 0 || strcmp(cmd_buf, "GET:HUM") == 0)
                {
                    char resp[20];
                    uint8_t h = data_dht_get_humidity();
                    sprintf(resp, "HUM:%d%%\r\n", h);
                    UART_sendString((const uint8*)resp);
                }
                // Read the whole system
                else if (strcmp(cmd_buf, "ALL?") == 0 || strcmp(cmd_buf, "STATUS?") == 0)
                {
                    char resp[50];
                    uint8_t t = data_dht_get_temperature();
                    uint8_t h = data_dht_get_humidity();
                    uint8_t st = data_dht_get_target_temperature();
                    uint8_t led = GPIO_readPin(PORTB_ID, PIN0_ID);
                    sprintf(resp, "TEMP:%dC,HUM:%d%%,SET:%dC,LED:%s\r\n", t, h, st, (led == LOGIC_HIGH) ? "ON" : "OFF");
                    UART_sendString((const uint8*)resp);
                }
                else
                {
                    UART_sendString((const uint8*)"ERR:UNKNOWN_CMD\r\n");
                }
                cmd_idx = 0; // Reset receive buffer
            }
        }

        /**
         * Buffer overflow protection 
         * If PC send a string which the width is over 32-byte without
         * '\n', this code will remove the leftover
         */
        else if (cmd_idx < sizeof(cmd_buf) - 1)
        {
            cmd_buf[cmd_idx++] = c;
        }
    }
}