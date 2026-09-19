#ifndef DRIVERS_SSD1306_H_
#define DRIVERS_SSD1306_H_

#include <stdio.h>
#include "i2c.h"

// Defines
#define SSD1306_ADDR	(0x3C)
#define SSD1306_CMD		(0x00)
#define SSD1306_DATA	(0x40)
#define DISPLAY_SIZE 	(128 * 8)

extern uint8_t ssd1306_buffer[DISPLAY_SIZE];

void ssd1306_init(void);
void ssd1306_cmd(uint8_t cmd);
void ssd1306_data(uint8_t data);
void ssd1306_clear_display(void);
void ssd1306_clear_buffer(void);
void ssd1306_update(void);
void ssd1306_draw_pixel(uint8_t x, uint8_t y);
void ssd1306_draw_char(uint8_t x, uint8_t y, char c);
void ssd1306_draw_string(uint8_t x, uint8_t y, char* str);

#endif /* DRIVERS_SSD1306_H_ */
