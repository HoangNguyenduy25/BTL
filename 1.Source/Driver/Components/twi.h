#ifndef TWI_H
#define TWI_H

#include "types_def.h"

void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteACK(void);
uint8 TWI_readByteNACK(void);
uint8 TWI_getStatus(void); 

#endif /* TWI_H */