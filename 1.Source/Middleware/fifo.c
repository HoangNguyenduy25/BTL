#include "fifo.h"
#include "common_macros.h"

#include <avr/io.h>
#include <avr/interrupt.h>

Fifo_t rx_fifo;

/**
 * Description
 * Ring buffer initialize
 */
void fifo_init(Fifo_t * ring_buffer, void* buffer, uint16_t buffer_size)
{
    ring_buffer -> tail_index = 0;
    ring_buffer -> head_index = 0;
    ring_buffer -> fill_size  = 0;

    ring_buffer -> buffer_size = buffer_size;
    ring_buffer -> buffer = buffer;
}

/**
 * Description
 * Add a character into ring buffer
 */
void fifo_char_put(Fifo_t * ring_buffer, uint8_t c)
{
    uint16_t next_tail_index;
    uint16_t next_head_index;

    ring_buffer -> buffer[ring_buffer -> tail_index] = c;

    next_tail_index = (++ring_buffer -> tail_index) % ring_buffer->buffer_size;
    ring_buffer->tail_index = next_tail_index;

    if(ring_buffer -> fill_size ==  ring_buffer -> buffer_size)
    {
        next_head_index = (++ring_buffer -> head_index) % ring_buffer -> buffer_size;
        ring_buffer -> head_index = next_head_index;
    }
    else
    {
        ring_buffer -> fill_size++;
    }
}

/**
 * Description
 * Get ad character from ring buffer
 */
uint8_t fifo_char_get(Fifo_t * ring_buffer)
{
    uint16_t ret = 0;
    uint16_t next_head_index;
    uint8_t sreg = SREG;
    cli();

    if(ring_buffer->fill_size)
    {
        ret = ring_buffer->buffer[ring_buffer->head_index];

        next_head_index = (++ring_buffer->head_index) % ring_buffer->buffer_size;
        ring_buffer->head_index = next_head_index;

        ring_buffer->fill_size--;
    }

    SREG = sreg;
    return ret;
}

uint8_t ring_buffer_status(Fifo_t * ring_buffer)
{
    uint16_t size;
    uint8_t sreg = SREG;
    cli();
    size = ring_buffer->fill_size;
    SREG = sreg;

    if(size == 0)
    {
        return RING_BUF_EMPTY;
    }
    else if (size >= ring_buffer->buffer_size)
    {
        return RING_BUF_FULL;
    }
    return RING_BUF_NOT_EMPTY;
}