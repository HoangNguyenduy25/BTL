#ifndef BSP_UART_H
#define BSP_UART_H
#include <stdint.h>

void bsp_uart_init(void);
void UART_Rx_Interrupt(uint8_t received_byte);
#endif /* BSP_UART_H */