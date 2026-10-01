#include "bsp_uart.h"
#include "board.h"
#include "fifo.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "types_def.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

 static const char * const g_pcHex = "0123456789abcdef";

#ifdef USART0_ENABLE
	usart_meta_t	UART0_meta;
	usart_meta_t	*p_UART0_meta = &UART0_meta;
#endif

#ifdef UART0_CONSOLE
    #define CONSOLE_UART_WRITE(pdata, len) usart0_send_array(pdata, len)
#else 
    #define CONSOLE_UART_WRITE(data)
#endif

#define USART_RX_ERROR_MASK ((1 << FE0) | (1 << DOR0) | (1 << UPE0))


#ifdef USART0_ENABLE

void usart0_init(void)
{
    /* BSP UART config with baudrate 9600 */
    UART_init(9600, F_CPU);

    /* Fifo initialize config */
    rbuffer_init(&p_UART0_meta->rb_tx);                             // Init Rx buffer
    rbuffer_init(&p_UART0_meta->rb_rx);                             // Init Tx buffer
}


/* USART FUNCTIONS */

void usart0_send_char( char c) {
    while(rbuffer_full(&p_UART0_meta->rb_tx));
    rbuffer_insert(c, &p_UART0_meta->rb_tx);
    UCSR0B |= 1 << UDRE0;                   // Enable Tx buffer empty interrupt 
}

void usart0_send_string(const char* str) {
    while (*str) {
        usart0_send_char(*str++);
    }
}

void	usart0_send_array(const char* str, uint8_t	len)
{
	uint8_t	udx;
	for(udx = 0;udx < len; udx++)	 usart0_send_char(*str++);
}

void usart0_send_string_P(const char* chr) {
    char c;
    while ((c = pgm_read_byte(chr++))) {
        usart0_send_char(c);
    }
}

uint8_t usart0_rx_count(void) {
    return rbuffer_count(&p_UART0_meta->rb_rx);
}

uint16_t usart0_read_char(void) {
    if (!rbuffer_empty(&p_UART0_meta->rb_rx)) {
        return (((p_UART0_meta->usart_error & USART_RX_ERROR_MASK) << 8) | (uint16_t)rbuffer_remove(&p_UART0_meta->rb_rx));
    }
    else {
        return (((p_UART0_meta->usart_error & USART_RX_ERROR_MASK) << 8) | USART_NO_DATA);     // Empty ringbuffer
    }
}

void usart0_close() {
    while(!rbuffer_empty(&p_UART0_meta->rb_tx));                        // Wait for Tx to transmit ALL characters in ringbuffer
    while(!(UCSR0A & (1 << TXC0)));										// Wait for Tx unit to transmit the LAST character of ringbuffer

    _delay_ms(200);                                             // Extra safety for Tx to finish!

	UCSR0B &= ~( (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0) | (1<<	UDRIE0));					//disable TX, RX, RX interrupt
	UCSR0C &= (1<<UCSZ10) | (1<<UCSZ00);
}
#endif

/* ISR Function */
#ifdef USART0_ENABLE
ISR(USART0_RX_vect) {
	
   char	data = UDR0;
    if(!rbuffer_full(&p_UART0_meta->rb_rx)) {
	    rbuffer_insert(data, &p_UART0_meta->rb_rx);
	    p_UART0_meta->usart_error = UCSR0A & USART_RX_ERROR_MASK ;

    }
    else {
	    p_UART0_meta->usart_error = ((UCSR0A & USART_RX_ERROR_MASK) | USART_BUFFER_OVERFLOW>>8);
    }   
}
ISR(USART0_UDRE_vect) {
    if(!rbuffer_empty(&p_UART0_meta->rb_tx)) {
	    UDR0 = rbuffer_remove(&p_UART0_meta->rb_tx);
    }
    else {
	    UCSR0B &= ~(1 << UDRE0);                   // Enable Tx buffer empty interrupt 
    }
}
#endif

void UARTvprintf(const char *pcString, va_list vaArgP)
{
    char buf[80];
    vsnprintf(buf, sizeof(buf), pcString, vaArgP);
    usart0_send_string(buf);
}

void UARTprintf(const char *pcString, ...)
{
    va_list vaArgP;
    va_start(vaArgP, pcString);
    UARTvprintf(pcString, vaArgP);
    va_end(vaArgP);
}
