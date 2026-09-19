#include "drivers/i2c.h"

/**
 * PB6 ---- SCL
 * PB7 ---- SDA
 */
void I2C1_Init(void)
{

	// Enable CLK for GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// PB6 - SCL; PB7 - SDA
	GPIO_Init(GPIOB, PIN_6, ALTERNATE, OPEN_DRAIN, LOW, PULL_UP); // SCL
	GPIO_Init(GPIOB, PIN_7, ALTERNATE, OPEN_DRAIN, LOW, PULL_UP); // SDA

	// Alternate Functions
	SelectAltFunction(GPIOB, PIN_6, AF4);
	SelectAltFunction(GPIOB, PIN_7, AF4);

	// Enable CLK for I2C1
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

	// RESET I2C
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST;

	// Turn off I2C reset
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;

    // DISABLE I2C
    I2C1->CR1 &= ~I2C_CR1_PE;

    // Hardcoded for now
    I2C1->TIMINGR = (uint32_t)(0x00100D14);

    /* Enable I2C */
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_size)
{
	// Clear SADD field
	I2C1->CR2 &= ~I2C_CR2_SADD_Msk;

	// Set slave address
	I2C1->CR2 |= (slave_addr << 1U);

	// Write mode
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	// Clear NBYTES field
	I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk;

	// Numbers of bytes to write.
	I2C1->CR2 |= ((data_size + 1) << I2C_CR2_NBYTES_Pos);

	//  Automatic end mode
	I2C1->CR2 |= I2C_CR2_AUTOEND;

	// Start Trigger
	I2C1->CR2 |= I2C_CR2_START;

	int timeout = 10000;
	// Wait until TX register is empty and ready for next byte
	while(!(I2C1->ISR & I2C_ISR_TXIS))
	{
		timeout--;
		if (timeout == 0)
		{
			return;
		}
	}

	// Send reg_addr
	I2C1->TXDR = reg_addr;

	// loop through every byte in payload
	for (int i = 0; i < data_size; i++)
	{
		timeout = 10000;
		// Wait until TX register is empty and ready for next byte
		while(!(I2C1->ISR & I2C_ISR_TXIS))
		{
			timeout--;
			if (timeout == 0)
			{
				return;
			}
		}

		// Send ith data_byte
		I2C1->TXDR = data[i];

	}

	timeout = 10000;
	// Wait until STOP condition is automatically generated on the bus
	while(!(I2C1->ISR & I2C_ISR_STOPF))
	{
		timeout--;
		if (timeout == 0)
		{
			return;
		}
	}

	// Clear STOP flag
	I2C1->ICR = I2C_ICR_STOPCF;

}

void I2C1_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_size)
{
	// Clear SADD field
	I2C1->CR2 &= ~I2C_CR2_SADD_Msk;

	// Set slave address
	I2C1->CR2 |= (slave_addr << 1U);

	// Write mode
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	// Clear NBYTES field
	I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk;

	// Only 1 byte because we are sending reg_addr
	I2C1->CR2 |= (1 << I2C_CR2_NBYTES_Pos);

	// Prevents false stop condition
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;

	// Start Trigger
	I2C1->CR2 |= I2C_CR2_START;

	int timeout = 10000;
	// Wait until TX register is empty and ready for next byte
	while(!(I2C1->ISR & I2C_ISR_TXIS))
	{
		timeout--;
		if (timeout == 0)
		{
			return;
		}
	}

	// Send reg_addr
	I2C1->TXDR = reg_addr;

	timeout = 10000;
	// Wait for transfer complete flag
	while (!(I2C1->ISR & I2C_ISR_TC))
	{
		timeout--;
		if (timeout == 0)
		{
			return;
		}
	}

	// Read mode
	I2C1->CR2 |= I2C_CR2_RD_WRN;

	// Clear NBYTES field
	I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk;

	// How many bytes to be read
	I2C1->CR2 |= (data_size << I2C_CR2_NBYTES_Pos);

	// Automatic end mode
	I2C1->CR2 |= I2C_CR2_AUTOEND;

	// Start Trigger
	I2C1->CR2 |= I2C_CR2_START;

	// read data_size number of bytes
	for (int i = 0; i < data_size; i++)
	{
		timeout = 10000;

		// wait for Receive data register not empty flag
		while(!(I2C1->ISR & I2C_ISR_RXNE))
		{
			timeout--;
			if (timeout == 0)
			{
				return;
			}
		}

		// Read ith byte into data array
		data[i] = I2C1->RXDR;
	}

	timeout = 10000;
	// Wait until STOP condition is automatically generated on the bus
	while(!(I2C1->ISR & I2C_ISR_STOPF))
	{
		timeout--;
		if (timeout == 0)
		{
			return;
		}
	}

	// Clear STOP flag
	I2C1->ICR = I2C_ICR_STOPCF;
}
