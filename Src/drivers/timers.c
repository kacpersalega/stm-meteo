#include "drivers/timers.h"

void SysTick_Delay_us(uint32_t microseconds)
{
	SysTick->LOAD = (4 - 1); // for 1 microsecond delay

	// Clear
    SysTick->VAL = 0;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (uint32_t i = 0; i < microseconds; i++)
    {
    	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    }

    SysTick->CTRL = 0;

}
