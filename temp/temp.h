#ifndef TEMP_H_
#define TEMP_H_

#include <inttypes.h>

void temp_init(void);
uint8_t temp_read_celsius(void);
uint8_t temp_read_fahrenheit(void);

#endif /* TEMP_H_ */