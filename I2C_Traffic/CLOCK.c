/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "CLOCK.h"
#include "stm32f4xx.h"
/*
* Configure the clock
* Use this function to configure the clock
*/
void initClock(void)
{
 /**** RM0390 chapter 3 Reset and clock Control (RCC)
	* Steps to be followed to configure clock 
	* 1. Enable HSE (High SPeed External Clock) and wait for HSE to become ready 
	*			-- HSE (i) External Crystal/ceramic resonator (ii) HSE external user clock
	* 2. Set the power enable clock and volatge regulator
	* 3. Configure the FLASH PREFETCH and the LATENCY Related Settings 
	* 4. Configure the PRESCALERS HCLK, PCLK1, PCLK2
  * 5. Configugure the main PLL
	* 6. Enable PLL and wait for it to become ready
	* 7. Select clock source and wait for it to be set
	* Clock Set done
	***/
	
/* Turn HSE on and wait for to be ready RCC CR Register 16 th bit set*/
	/* Enable HSE clock See register RCC_CR (RCC clock control register -- 32-bit register
	* Rest value 0x0000 xx83
	* Bit 16 HSEON and bit-17 HSERDY (status)
	*/
/*
* Here we enable our system intended to use HSE clock 
*/	
RCC->CR |=RCC_CR_HSEON; // set CR bit 16 
/** Check if clock is ready RCC CR register 17th bit set*/ 
while(!(RCC->CR & RCC_CR_HSERDY)); //wait for the clock is enabled See RCC CR bit-17; HSE crystal is On
/* Set the POWER enable CLOCK and VOLTAGE REGULATOR */
RCC->APB1ENR |= RCC_APB1ENR_PWREN; //power enable for APB1
PWR->CR |= PWR_CR_VOS; //VOS always correspond to reset value 

/*3. Configure the FLASH PREFETCH and the LATENCY Related Settings */
FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS; //ICEN -- instruction cache, DCEN -- Data Cache, PRFTEN -- prefetch and LAtency;

/* 4. Configure the PRESCALERS HCLK, PCLK1, PCLK2 */
//AHB prescaler 
RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
//APB1 prescaler
RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
//APB2 prescaler
RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
//5. Configugure the main PLL
RCC->PLLCFGR = (PLL_M<<0) | (PLL_N<<6) | (PLL_P<<16) | (RCC_PLLCFGR_PLLSRC_HSE); 
//6. Enable PLL and wait for it to become ready
RCC->CR |= RCC_CR_PLLON;
//Check if PLL clock is ready
while(!(RCC->CR & RCC_CR_PLLRDY))
	; //wait for PLL ready
//7. Select clock source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}


