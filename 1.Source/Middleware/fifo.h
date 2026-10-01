#ifndef FIFO_H
#define FIFO_H


#include <stdatomic.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <avr/io.h>

/* Define Ring Buffer Size */
#define RBUFFER_SIZE 32

/* Enable USART0 */
#define USART0_ENABLE
#define UART0_CONSOLE

/* Define error codes */
#define USART_BUFFER_OVERFLOW   0x6400
#define USART_FRAME_ERROR       0x0400
#define USART_PARITY_ERROR      0x0200
#define USART_NO_DATA           0x0100

// RINGBUFFER STRUCT
typedef struct { 
    volatile char     buffer[RBUFFER_SIZE];
    volatile uint8_t  in;
    volatile uint8_t  out;
    volatile uint8_t  count;
} ringbuffer_t;



// USART META STRUCT
typedef struct { 
    volatile ringbuffer_t rb_rx;    // Rx ringbuffer
    volatile ringbuffer_t rb_tx;    // Tx ringbuffer
    volatile uint16_t usart_error;   // Holds error from RXDATAH
} usart_meta_t;

/* Ring buffer definition */
void rbuffer_init(volatile ringbuffer_t* rb);
uint8_t rbuffer_count(volatile ringbuffer_t* rb);
bool rbuffer_full(volatile ringbuffer_t* rb);
bool rbuffer_empty(volatile ringbuffer_t* rb);
void rbuffer_insert(char data, volatile ringbuffer_t* rb);
char rbuffer_remove(volatile ringbuffer_t* rb);


#endif /* FIFO_H */