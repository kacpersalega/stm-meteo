#ifndef DRIVERS_GPIO_H_
#define DRIVERS_GPIO_H_

#include <stdint.h>
#include <stdbool.h>
#include "cmsis/stm32l476xx.h"

// preprocessor directives for readability
#define PIN_0				0U
#define PIN_1				1U
#define PIN_2				2U
#define PIN_3				3U
#define PIN_4				4U
#define PIN_5				5U
#define PIN_6				6U
#define PIN_7				7U
#define PIN_8 				8U
#define PIN_9				9U
#define PIN_10				10U
#define PIN_11				11U
#define PIN_12				12U
#define PIN_13				13U
#define PIN_14				14U
#define PIN_15				15U


#define AF0					0U
#define AF1 				1U
#define AF2					2U
#define AF3					3U
#define AF4					4U
#define AF5					5U
#define AF6					6U
#define AF7					7U
#define AF8					8U
#define AF9					9U
#define AF10				10U
#define AF11				11U
#define AF12				12U
#define AF13				13U
#define AF14				14U
#define AF15				15U


typedef enum{
	INPUT		= 0U,
	OUTPUT,
	ALTERNATE,
	ANALOG,
}GPIO_Mode_t;

typedef enum{
	PUSH_PULL	= 0U,
	OPEN_DRAIN,
}GPIO_OTYPE_t;

typedef enum{
	LOW			= 0U,
	MEDIUM,
	HIGH,
	VERY_HIGH,
}GPIO_Speed_t;

typedef enum{
	PULL_NONE	= 0U,
	PULL_UP,
	PULL_DOWN,
}GPIO_Pull_t;

typedef enum{
	RESET		= 0U,
	SET,
}GPIO_SetReset_t;


void GPIO_Init(GPIO_TypeDef* port,
			   uint8_t pin,
			   GPIO_Mode_t mode,
			   GPIO_OTYPE_t output_type,
			   GPIO_Speed_t output_speed,
			   GPIO_Pull_t pull_up_down);

void GPIO_Write(GPIO_TypeDef* port,
				uint8_t pin,
				GPIO_SetReset_t set_reset);

void GPIO_Toggle(GPIO_TypeDef* port, uint8_t pin);

bool GPIO_Read_Pin(GPIO_TypeDef* port, uint8_t pin);

void SelectAltFunction(GPIO_TypeDef *port, uint16_t pin, uint8_t alt_function);

#endif /* DRIVERS_GPIO_H_ */
