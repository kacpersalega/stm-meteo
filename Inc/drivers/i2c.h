#ifndef DRIVERS_I2C_H_
#define DRIVERS_I2C_H_

#include <stdint.h>
#include "cmsis/stm32l476xx.h"
#include "gpio.h"

void I2C1_Init(void);

uint32_t I2C1_ReadByte(uint8_t slave_addr, uint8_t reg_addr);
void I2C1_WriteByte(uint8_t slave_addr, uint8_t control_byte, uint8_t data);
void I2C1_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_size);
void I2C1_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_size);
#endif /* DRIVERS_I2C_H_ */
