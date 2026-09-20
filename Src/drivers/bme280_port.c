#include "drivers/bme280_port.h"
#include "drivers/i2c.h"
#include "drivers/timers.h"

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t slave_addr = *(uint8_t*)intf_ptr;
    I2C1_Read(slave_addr, reg_addr, reg_data, (uint8_t)len);
    return 0;
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t slave_addr = *(uint8_t*)intf_ptr;
    I2C1_Write(slave_addr, reg_addr, (uint8_t*)reg_data, (uint8_t)len);
    return 0;
}

void user_delay_us(uint32_t period, void *intf_ptr)
{
    SysTick_Delay_us(period);
}

