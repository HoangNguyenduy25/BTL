#ifndef DATA_DHT_H_
#define DATA_DHT_H_

#include <stdint.h>

void data_dht_set_temperature(uint8_t temperature);
uint8_t data_dht_get_temperature(void);

void data_dht_set_humidity(uint8_t humidity);
uint8_t data_dht_get_humidity(void);

void data_dht_set_target_temperature(uint8_t target);
uint8_t data_dht_get_target_temperature(void);

#endif /* DATA_DHT_H_ */