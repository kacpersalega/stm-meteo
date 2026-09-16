#include "drivers/gpio.h"

void GPIO_Init(GPIO_TypeDef* port,
			   uint8_t pin,
			   GPIO_Mode_t mode,
			   GPIO_OTYPE_t output_type,
			   GPIO_Speed_t output_speed,
			   GPIO_Pull_t pull_up_down)
{

	// Clear pins respective registers
	port->MODER &= ~(0x3U << (pin * 2U));
	port->OTYPER &= ~(0x1U << pin);
	port->OSPEEDR &= ~(0x3U << (pin * 2U));
	port->PUPDR &= ~(0x3U << (pin * 2U));

	// Set pins respective registers
	port->MODER |= (uint32_t)(mode << (pin * 2U));
	port->OTYPER |= (uint32_t)(output_type << pin);
	port->OSPEEDR |= (uint32_t)(output_speed << (pin * 2U));
	port->PUPDR |= (uint32_t)(pull_up_down << (pin * 2U));

}

void GPIO_Write(GPIO_TypeDef* port,
				uint8_t pin,
				GPIO_SetReset_t set_reset)
{
	if (set_reset == RESET)
	{
		port->BSRR = (uint32_t)(1U << (pin + 16U));
	}
	else
	{
		port->BSRR = (uint32_t)(1U << pin);
	}
}

void GPIO_Toggle(GPIO_TypeDef* port, uint8_t pin)
{
	if (port->ODR & (1U << pin))
	{
		port->BSRR = (uint32_t)(1U << (pin + 16U));
	}
	else
	{
		port->BSRR = (uint32_t)(1U << pin);
	}

}

bool GPIO_Read_Pin(GPIO_TypeDef* port, uint8_t pin)
{
	bool state = (port->IDR & (1U << pin));
	return state;
}

void SelectAltFunction(GPIO_TypeDef *port, uint16_t pin, uint8_t alt_function)
{
	// makes sure that only 4 LSB bits are taken into account
	alt_function &= 0xFU;

	if (pin <= 7)
	{
		// Clear the corresponding 4 bits of particular PIN
		port->AFR[0] &= ~(0xFUL << (pin * 4U));

		// Set the pin to alternate function
		port->AFR[0] |= ((uint32_t)alt_function << (pin * 4U));
	}
	else
	{
		// Clear the corresponding 4 bits of particular PIN
		port->AFR[1] &= ~(0xFUL << ((pin - 8U) * 4U));

		// Set the pin to alternate function
		port->AFR[1] |= ((uint32_t)alt_function << ((pin - 8U) * 4U));
	}
}
