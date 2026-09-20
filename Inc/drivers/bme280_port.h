#ifndef DRIVERS_BME280_PORT_H_
#define DRIVERS_BME280_PORT_H_

#include <stdint.h>
#include "bme280.h"

// Prototypes for the Bosch API wrappers
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);

#endif /* DRIVERS_BME280_PORT_H_ */
