#ifndef DRIVERS_TIMERS_H_
#define DRIVERS_TIMERS_H_

#include <stdint.h>
#include "cmsis/stm32l476xx.h"
#include <stdio.h>

void SysTick_Delay_us(uint32_t microseconds);

#endif /* DRIVERS_TIMERS_H_ */
