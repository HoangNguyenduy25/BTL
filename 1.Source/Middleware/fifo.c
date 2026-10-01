#include "fifo.h"
#include "common_macros.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void rbuffer_init(volatile ringbuffer_t* rb) {
    
        rb->in = 0;
        rb->out = 0;
        rb->count = 0;
    
}

uint8_t rbuffer_count(volatile ringbuffer_t* rb) {
    return rb->count;
}

bool rbuffer_full(volatile ringbuffer_t* rb) {
    return (rb->count == (uint8_t)RBUFFER_SIZE);
}

bool rbuffer_empty(volatile ringbuffer_t* rb) {
    return (rb->count == 0);
}

void rbuffer_insert(char data, volatile ringbuffer_t* rb) {   
    *(rb->buffer + rb->in) = data;
    rb->in = (rb->in + 1) & ((uint8_t)RBUFFER_SIZE - 1);
    rb->count++;
    
}

char rbuffer_remove(volatile ringbuffer_t* rb) {
    char data = *(rb->buffer + rb->out);
        rb->out = (rb->out + 1) & ((uint8_t)RBUFFER_SIZE - 1);
        rb->count--;
    return data;
}