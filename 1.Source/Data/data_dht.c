#include "data_dht.h"
#include "board.h"

volatile uint8_t temperature = 0;
volatile uint8_t humidity = 0;

void data_dht_set_temperature(uint8_t value)
{
	temperature = value;
}

uint8_t data_dht_get_temperature(void)
{
	return temperature;
}

void data_dht_set_humidity(uint8_t value)
{
	humidity = value;
}

uint8_t data_dht_get_humidity(void)
{
	return humidity;
}

volatile uint8_t target_temperature = 25; 

void data_dht_set_target_temperature(uint8_t value)
{
	target_temperature = value;
}

uint8_t data_dht_get_target_temperature(void)
{
	return target_temperature;
}