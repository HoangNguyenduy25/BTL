#include "twi.h"
#include "types_def.h"
#include "common_macros.h"
#include <avr/io.h>

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

void TWI_init(void)
{
    /* Set SCL frequency to 100kHz with F_CPU = 8MHz */
    TWSR = 0x00; // Prescaler value = 1
    TWBR = 0x20; // Bit rate register value for 100kHz
}

void TWI_start(void)
{
    /* Send START condition */
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (BIT_IS_CLEAR(TWCR, TWINT)); // Wait for TWINT flag set
}

void TWI_stop(void)
{
    /* Send STOP condition */
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

void TWI_writeByte(uint8 data)
{
    /* Load data into TWDR register */
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT); // Clear TWINT to start transmission
    while (BIT_IS_CLEAR(TWCR, TWINT)); // Wait for TWINT flag set
}

uint8 TWI_readByteACK(void)
{
    /* Enable ACK and start receiving data */
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (BIT_IS_CLEAR(TWCR, TWINT)); // Wait for TWINT flag set
    return TWDR;
}

uint8 TWI_readByteNACK(void)
{
    /* Disable ACK and start receiving data */
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (BIT_IS_CLEAR(TWCR, TWINT)); // Wait for TWINT flag set
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    /* Return the status from TWSR register, masking the prescaler bits */
    return (TWSR & 0xF8);
}

