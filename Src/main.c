#include <stdint.h>
#include "cmsis/stm32l476xx.h"
#include "drivers/gpio.h"
#include "drivers/i2c.h"
#include "drivers/ssd1306.h"
#include "drivers/bme280.h"
#include "drivers/timers.h"
#include "drivers/bme280_port.h"



int main(void)
{
	// Enable FPU
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));

	// Initialize hardware
	I2C1_Init();
	ssd1306_init();
	ssd1306_clear_display();

	// BME Setup
	struct bme280_dev bme;
	uint8_t dev_addr = BME280_I2C_ADDR_PRIM;

	bme.intf_ptr = &dev_addr;
	bme.intf = BME280_I2C_INTF;
	bme.read = user_i2c_read;
	bme.write = user_i2c_write;
	bme.delay_us = user_delay_us;

	int8_t rslt = bme280_init(&bme);

	struct bme280_settings settings;
	uint8_t settings_sel;

	// Set oversampling rates
	settings.osr_h = BME280_OVERSAMPLING_1X;
	settings.osr_p = BME280_OVERSAMPLING_16X;
	settings.osr_t = BME280_OVERSAMPLING_2X;

	// Set the IIR filter and standby time (time between automatic measurements)
	settings.filter = BME280_FILTER_COEFF_16;
	settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

	// Tell the API which settings you want to apply
	settings_sel = BME280_SEL_OSR_PRESS | BME280_SEL_OSR_TEMP | BME280_SEL_OSR_HUM | BME280_SEL_FILTER | BME280_SEL_STANDBY;

	// Write the settings to the sensor
	bme280_set_sensor_settings(settings_sel, &settings, &bme);

	bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &bme);

	struct bme280_data comp_data;

	double temp = 0.0;
	double press = 0.0;
	double hum = 0.0;

	char temp_string[32];
	char press_string[32];
	char hum_string[32];

	while (1)
	{
		// Delay 70ms to ensure a new measurement is ready.
		// (Standby time is 62.5ms + ~7ms active measurement time)
		bme.delay_us(70000, bme.intf_ptr);

		rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &bme);

		if (rslt == BME280_OK)
		{
	        // Temperature in Degrees Celsius
	        temp = comp_data.temperature;

	        // Pressure in Pascals
	        press = comp_data.pressure;

	        // Relative Humidity in %
	        hum = comp_data.humidity;
		}

		press = press / 100.0; // to get hPa

		snprintf(temp_string, sizeof(temp_string), "TEMPERATURE: %.2f C", temp);
		snprintf(press_string, sizeof(press_string), "PRESSURE: %.1f hPa", press);
		snprintf(hum_string, sizeof(hum_string), "HUMIDITY: %.1f %%", hum);

	    ssd1306_clear_buffer();
	    // Draw strings at (X, Y) pixel coordinates
		ssd1306_draw_string(0, 5, temp_string);
		ssd1306_draw_string(0, 30, press_string);
		ssd1306_draw_string(0, 55, hum_string);
		ssd1306_update();


	}
}
