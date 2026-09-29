#ifndef BSP_DHT_H_
#define BSP_DHT_H_

#include <stdint.h>

/* DHT11 DATA pin */
#define DHT_PORT        PORTC
#define DHT_PIN         PINC
#define DHT_DDR         DDRC
#define DHT_DATA_PIN    PC6


void BSP_DHT_Init(void);

uint8_t BSP_DHT_Read(uint8_t *humidity,
                      uint8_t *temperature);

uint8_t DHT_SendStart(void);
uint8_t DHT_CheckResponse(void);

#endif /* BSP_DHT_H_ */