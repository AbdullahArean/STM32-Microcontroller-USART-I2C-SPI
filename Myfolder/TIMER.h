#include "SYS_INIT.h"
#include "CLOCK.h"

void TIM6Config(void);//Timer6 Configuration
void Delay_us (uint16_t us);//Delay count in Microseconds
void Delay_ms (uint16_t ms);//Delay Count in milliseconds
void TIM6_DAC_IRQHandler(void);