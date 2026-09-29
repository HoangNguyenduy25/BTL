#include "bsp_uart.h"
#include "board.h"
#include "fifo.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define UART_RX_BUFFER_SIZE 64
static uint8_t rx_raw_buffer[UART_RX_BUFFER_SIZE];

void bsp_uart_init(void)
{
    /* BSP UART config with baudrate 9600 */
    UART_init(9600, F_CPU);

    /* Fifo initialize config */
    fifo_init(&rx_fifo, &rx_raw_buffer, UART_RX_BUFFER_SIZE);
    
}

/**
 * ISR enable when there is 1 byte send from PC to USART0
 */
ISR(USART0_RX_vect)
{
    uint8_t ch = UDR0;
    fifo_char_put(&rx_fifo, ch);
}


