#include "BSP_DHT.h"
#include "BOARD.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


/*
 * DHT DATA = OUTPUT, MCU dieu khien DATA
 */
static void DHT_SetOutput(void)
{
    DHT_DDR |= (1 << DHT_DATA_PIN);
}


/*
 * DHT DATA = INPUT, DHT11 gui du lieu ve cho MCU (giu pull-up)
 */
static void DHT_SetInput(void)
{
    DHT_DDR  &= ~(1 << DHT_DATA_PIN);
    DHT_PORT |=  (1 << DHT_DATA_PIN);
}


/*
 * Set DATA = LOW
 */
static void DHT_WriteLow(void)
{
    DHT_PORT &= ~(1 << DHT_DATA_PIN);
}


/*
 * Set DATA = HIGH
 */
static void DHT_WriteHigh(void)
{
    DHT_PORT |= (1 << DHT_DATA_PIN);
}


/*
 * Doc muc hien tai cua chan DATA
 */
static uint8_t DHT_ReadPin(void)
{
    return (DHT_PIN & (1 << DHT_DATA_PIN)) != 0;
}


/*
 * Cho DATA dat toi muc mong muon.
 *
 * level:
 *      0 -> cho LOW
 *      1 -> cho HIGH
 *
 * timeout:
 *      so vong lap toi da (~1us moi vong)
 *
 * return:
 *      1 = thanh cong
 *      0 = timeout
 */
static uint8_t DHT_WaitForLevel(uint8_t level, uint16_t timeout)
{
    while (DHT_ReadPin() != level)
    {
        if (timeout == 0)
        {
            return 0;
        }

        timeout--;
        _delay_us(1);
    }

    return 1;
}


/*
 * MCU gui START signal cho DHT11.
 *
 * Sequence:
 *   DATA LOW   >= 18 ms
 *   DATA HIGH  ~ 20-40 us
 *   DATA INPUT (nha bus cho DHT11 keo)
 */
uint8_t DHT_SendStart(void)
{
    DHT_SetOutput();

    /* START LOW */
    DHT_WriteLow();
    _delay_ms(20);

    /* Nha bus len HIGH, giu 30us theo datasheet */
    DHT_WriteHigh();
    _delay_us(30);

    /* Nha bus cho DHT11 dieu khien */
    DHT_SetInput();

    return 1;
}


/*
 * Kiem tra response cua DHT11: LOW ~80us -> HIGH ~80us -> LOW (bat dau bit dau tien)
 *
 * return:
 *      4 = OK
 *      1/2/3 = loi tai tung buoc
 */
uint8_t DHT_CheckResponse(void)
{
    /* DHT keo LOW ~80us */
    if (!DHT_WaitForLevel(0, 200))
    {
        return 1;   /* E1: khong thay LOW */
    }

    /* DHT tha HIGH ~80us */
    if (!DHT_WaitForLevel(1, 150))
    {
        return 2;   /* E2: khong thay HIGH */
    }

    /* DHT keo LOW de bat dau bit dau tien */
    if (!DHT_WaitForLevel(0, 150))
    {
        return 3;   /* E3: khong thay LOW ke tiep */
    }

    return 4;   /* OK */
}


/*
 * Doc 1 bit: cho HIGH bat dau, sample sau 40us, roi cho LOW (= bat dau bit ke)
 */
static uint8_t DHT_ReadBit(uint8_t *bit)
{
    /* Cho LOW ket thuc -> HIGH bat dau */
    if (!DHT_WaitForLevel(1, 100))
    {
        return 0;
    }

    _delay_us(40);

    *bit = DHT_ReadPin() ? 1 : 0;

    /* Cho HIGH ket thuc, dong thoi la LOW bat dau bit ke tiep */
    if (!DHT_WaitForLevel(0, 100))
    {
        return 0;
    }

    return 1;
}


/*
 * Doc 1 byte (8 bit, MSB truoc)
 */
static uint8_t DHT_ReadByte(uint8_t *data)
{
    uint8_t i;
    uint8_t bit;

    *data = 0;

    for (i = 0; i < 8; i++)
    {
        if (!DHT_ReadBit(&bit))
        {
            return 0;
        }

        *data <<= 1;
        *data |= bit;
    }

    return 1;
}


void BSP_DHT_Init(void)
{
    /* DATA = input, bat pull-up noi */
    DHT_DDR  &= ~(1 << DHT_DATA_PIN);
    DHT_PORT |=  (1 << DHT_DATA_PIN);
}


/*
 * Doc humidity + temperature tu DHT11
 *
 * return:
 *      0 = OK
 *      1 = start fail
 *      2 = response fail
 *      3 = doc bit fail
 *      4 = checksum sai
 */
uint8_t BSP_DHT_Read(uint8_t *humidity, uint8_t *temperature)
{
    uint8_t data[5];
    uint8_t i;
    uint8_t checksum;
    uint8_t sreg;

    /* START */
    if (!DHT_SendStart())
    {
        return 1;
    }

    /* Critical section: Microsecond-sensitive 1-wire timing (~4ms) */
    sreg = SREG;
    cli();

    /* RESPONSE */
    if (DHT_CheckResponse() != 4)
    {
        SREG = sreg;
        return 2;
    }

    /* READ 40 BIT */
    for (i = 0; i < 5; i++)
    {
        if (!DHT_ReadByte(&data[i]))
        {
            SREG = sreg;
            return 3;
        }
    }

    SREG = sreg; /* Re-enable interrupts */

    /* CHECKSUM */
    checksum = data[0] + data[1] + data[2] + data[3];

    if (checksum != data[4])
    {
        return 4;
    }

    *humidity    = data[0];
    *temperature = data[2];

    return 0;
}