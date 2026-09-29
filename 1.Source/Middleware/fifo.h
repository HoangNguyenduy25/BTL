#ifndef FIFO_H
#define FIFO_H


#include <stdatomic.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define RING_BUF_EMPTY 0u
#define RING_BUF_FULL  1u
#define RING_BUF_NOT_EMPTY 2u

/**
 * FIFO struct
 */
typedef struct
{
    uint8_t *buffer;               /* pointer to the start of the FIFO buffer */
    volatile uint16_t fill_size;   /* number of elements in the FIFO buffer */
    volatile uint16_t head_index;  /* index to where next element will be removed */
    volatile uint16_t tail_index;  /* index to where next element will be inserted */
    uint16_t buffer_size;          /* total buffer capacity */
}Fifo_t;

extern Fifo_t rx_fifo;

/**
 * Description
 * Ring buffer initialize
 */
void fifo_init(Fifo_t * ring_buffer, void* buffer, uint16_t buffer_size);

/**
 * Description
 * Add a character into ring buffer
 */
void fifo_char_put(Fifo_t * ring_buffer, uint8_t c);

/**
 * Description
 * Get ad character from ring buffer
 */
uint8_t fifo_char_get(Fifo_t * ring_buffer);

/**
 * Check ring buffer status
 */
uint8_t ring_buffer_status(Fifo_t * ring_buffer);
#endif /* FIFO_H */