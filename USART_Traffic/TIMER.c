#include "SYS_INIT.h"
#include "CLOCK.h"
#include "TIMER.h"



void TIM6Config(void){
	/************STEPS TO FOLLOW*************
	1. Enable Timer Clock
	2. Set the prescaler and the ARR
	3. Enable the Counter of the Timer and wait for the Update Flag to set
	*****************************************/

	//1. Enable Timer Clock
	 RCC->APB1ENR |= (1<<4); //Enable the Timer6 clock
	
	//2. Set the prescaler and the ARR
	TIM6->PSC = 9000-1; 
	TIM6->ARR = 10000-1;
	TIM6->DIER |=TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	//3. Enable the Counter of the Timer and wait for the Update Flag to set
	TIM6->CR1 |= TIM_CR1_CEN; //Enable the Counter
	//while(!(TIM6->SR & (1<<0))); //UIF: Update Interrupt Flag.. This bit is set when the registers are updated
}
