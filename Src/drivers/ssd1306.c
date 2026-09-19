#include"drivers/ssd1306.h"
#include"drivers/font.h"

uint8_t ssd1306_buffer[DISPLAY_SIZE];

void ssd1306_init(void)
{
    /* Turn display off during configuration */
	ssd1306_cmd(0xAE);

    /* Set memory addressing mode */
    ssd1306_cmd(0x20);

    /* Use horizontal addressing mode */
    ssd1306_cmd(0x00);

    /* Remap display columns */
    ssd1306_cmd(0xA1);

    /* Set COM output scan direction */
    ssd1306_cmd(0xC8);

    /* Set display contrast */
    ssd1306_cmd(0x81);

    /* Set contrast value */
    ssd1306_cmd(0x7F);

    /* Set normal (non-inverted) display mode */
    ssd1306_cmd(0xA6);

    /* Set multiplex ratio */
    ssd1306_cmd(0xA8);

    /* Configure multiplex value for 64 rows */
    ssd1306_cmd(0x3F);

    /* Set display offset */
    ssd1306_cmd(0xD3);

    /* No display offset */
    ssd1306_cmd(0x00);

    /* Set display clock divide ratio */
    ssd1306_cmd(0xD5);

    /* Set recommended clock setting */
    ssd1306_cmd(0x80);

    /* Enable charge pump regulator */
    ssd1306_cmd(0x8D);

    /* Use internal charge pump */
    ssd1306_cmd(0x14);

    /* Turn display on */
    ssd1306_cmd(0xAF);
}

void ssd1306_cmd(uint8_t cmd)
{
	I2C1_Write(SSD1306_ADDR, SSD1306_CMD, &cmd, 1);
}

void ssd1306_data(uint8_t data) {

    /* Send data byte to SSD1306 */
	I2C1_Write(SSD1306_ADDR, SSD1306_DATA, &data, 1);
}

void ssd1306_clear_display(void)
{
    // Set full column address range
    ssd1306_cmd(0x21);
    ssd1306_cmd(0);
    ssd1306_cmd(127);

    // Set full page address range
    ssd1306_cmd(0x22);
    ssd1306_cmd(0);
    ssd1306_cmd(7);

    uint8_t clear = 0;

    // Clear entire display GDDRAM
    for (int i = 0; i < DISPLAY_SIZE; i++)
    {
        I2C1_Write(SSD1306_ADDR, SSD1306_DATA, &clear, 1);
    }
}

void ssd1306_clear_buffer(void)
{
	for (int i = 0; i < DISPLAY_SIZE; i++)
	{
		ssd1306_buffer[i] = 0;
	}
}

void ssd1306_update(void)
{
	for (int i = 0; i < DISPLAY_SIZE; i++)
	{
		ssd1306_data(ssd1306_buffer[i]);
	}
}

void ssd1306_draw_pixel(uint8_t x, uint8_t y)
{
	 if (x < 0 || x > 127 || y < 0 || y > 63)
	 {
		 return;
	 }

	 uint16_t index = (uint16_t)(x + ((y / 8) * 128));
	 uint8_t bit = (uint8_t)(1U << (y % 8));

	 ssd1306_buffer[index] |= bit;
}

void ssd1306_draw_char(uint8_t x, uint8_t y, char c)
{
	if (c < ' ' || c > '~')
	{
		return;
	}

	uint8_t char_index = c - 32;

	for (int i = 0; i < FONT_WIDTH; i++)
	{

		uint8_t column_byte = font5x7[char_index][i];

		for (int j = 0; j < 8; j++)
		{
			if (column_byte & (1 << j))
			{
				ssd1306_draw_pixel(x + i, y + j);
			}
		}
	}
}

void ssd1306_draw_string(uint8_t x, uint8_t y, char* str)
{
	while (*str)
	{
		ssd1306_draw_char(x, y, *str);
		x += FONT_WIDTH + 1; // Shifts cursor
		str++;
	}
}



