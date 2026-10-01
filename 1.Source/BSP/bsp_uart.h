#ifndef BSP_UART_H
#define BSP_UART_H
#include <stdint.h>
#include <stdarg.h>

// USART FUNCTIONS
void	usart0_init(void);			//init at 9600 bps, FCPU = 8Mhz
void	usart0_send_char( char c);
void	usart0_send_string(const char* str);
void	usart0_send_array(const char* str, uint8_t	len);
void	usart0_send_string_P(const char* chr);
uint8_t usart0_rx_count(void);
uint16_t usart0_read_char(void);
void	usart0_close(void);

void UARTvprintf(const char *pcString, va_list vaArgP);
void UARTprintf(const char *pcString, ...);
#endif /* BSP_UART_H */