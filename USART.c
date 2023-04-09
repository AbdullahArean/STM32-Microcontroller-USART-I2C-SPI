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

#include "USART.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CLOCK.h"
#include "stm32f446xx.h"
#include "SYS_INIT.h"
#include "GPIO.h"
#include "time.h"
#include "TIMER.h"






void _USART_WRITE(USART_TypeDef *usart,uint8_t *s)
{
	while (*s) UART_SendChar(usart,*s++);
}
/*****Modify according to your need *****/
uint8_t _USART_READ(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		if(i==0 && buff[i]!= 0x03F){continue;}
		n=i;
	}
	return n;
}

uint8_t _USART_READ_STR(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		n=i;
		if(buff[i]=='\0' || buff[i] == '\n' || buff[i] == ' ')
		{ 	
			buff[i]='\0';
			break;
		}
	}
	return n;
}


void UART_SendChar(USART_TypeDef *usart,uint8_t c){
	usart->DR = c;
	while(!(usart->SR & (1<<7)));
}

// Send a string to the UART PORTx
void UART_SendString(USART_TypeDef *usart,const char *s){
	while (*s) {UART_SendChar(usart,*s);s++;}
}

uint8_t UART_GetChar(USART_TypeDef *usart){
	uint8_t tmp;
	while(!(usart->SR & (1<<5)));
	tmp=(uint8_t)usart->DR;
	return tmp;
}
void UART_GetString(USART_TypeDef *uart,uint16_t size,uint8_t* buff)
{
	uint16_t i=0;
	while(size--)
	{
		uint8_t x=UART_GetChar(uart);
		buff[i]=x;
		i++;
	}
	buff[i]='\0';
		
}

/*
void USART2_IRQHandler(void){
			USART2->CR1 &= ~USART_CR1_RXNEIE;
			getInputString(currentMessage);
			USART2->CR1 |= ~USART_CR1_RXNEIE;
	}
*/

/**********************************
* USART 2 Configuration
***********************************/
void USART2_Config(void){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB1ENR |= (1<<17); //enable UART 2
	RCC->AHB1ENR |= (1<<0); //enable GPIOA clock
	RCC->AHB1ENR |= (1<<1); //enable GPIOB clock
	RCC->AHB1ENR |= (1<<2); //enable GPIOC clock
	
		
	//2. Configure UART pin for Alternate function
	GPIOA->MODER |= (2<<4); //bits [5:4] -> 1:0 -->Alternate function for pin PA2
	GPIOA->MODER |= (2<<6); //bits [7:6] -> 1:0 -->Alternate function for PA3
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6); //bits [5:4] -> 1:1 -> high speed PA2; bits [7:6] -> 1:1 -> high speed PA3 
	
	GPIOA->AFR[0] |= (7<<8);//Bytes (11:10:09:08) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA2
	GPIOA->AFR[0] |= (7<<12); //Bytes (15:14:13:12) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA3
	
	//3. Enable UART on USART_CR1 rgister
	USART2->CR1 = 0x00000000; //clear USART
	USART2->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART2->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length

	//5. Select the baud rate using the USART_BRR register.
	USART2->BRR |= (7<<0) | (24<<4); //115200

	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART2->CR1 |= (1<<2); // enable RE for receiver 
	USART2->CR1 |= (1<<3); //enable TE for transmitter
	
	//7. Enable RXNEIE for USART2
	NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= USART_CR1_RXNEIE;
	
	//To set priority to this interrupt we write
	//NVIC_SetPriority(USART2_IRQn, 1); // replace IRQHandler with IRQn 
//lower the number, higher the priority
}

void USART1_Config(void){
		/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB2ENR |= (1<<4); //enable USART 1
	RCC->AHB1ENR |= (1<<0); //enable GPIOA clock
	RCC->AHB1ENR |= (1<<1); //enable GPIOB clock
	RCC->AHB1ENR |= (1<<2); //enable GPIOC clock
		
	//2. Configure UART pin for Alternate function
	GPIOA->MODER |= (2<<18); //bits [19:18] -> 1:0 -->Alternate function for pin PA9
	GPIOA->MODER |= (2<<20); //bits [21:20] -> 1:0 -->Alternate function for PA10
	
	GPIOA->OSPEEDR |= (3<<18) | (3<<20); //bits [19:18] -> 1:1 -> high speed PA9; bits [21:20] -> 1:1 -> high speed PA10 
	
	GPIOA->AFR[1] |= (7<<4);//Bytes (07:06:05:04) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PA9
	GPIOA->AFR[1] |= (7<<8); //Bytes (11:10:09:08) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PA10
	
	//3. Enable UART on USART_CR1 rgister
	USART1->CR1 = 0x00000000; //clear USART
	USART1->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART1->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length

	//5. Select the baud rate using the USART_BRR register.
	USART1->BRR |= (7<<0) | (24<<4); //115200

	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART1->CR1 |= (1<<2); // enable RE for receiver 
	USART1->CR1 |= (1<<3); //enable TE for transmitter
	
	//7. Enable RXNEIE for USART1
	NVIC_EnableIRQ(USART1_IRQn);
	USART1->CR1 |= USART_CR1_RXNEIE;
	
	//To set priority to this interrupt we write
	//NVIC_SetPriority(USART2_IRQn, 1); // replace IRQHandler with IRQn 
//lower the number, higher the priority
}

void USART6_Config(void){
				/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB2ENR |= (1<<5); //enable USART 1
	RCC->AHB1ENR |= (1<<0); //enable GPIOA clock
	RCC->AHB1ENR |= (1<<1); //enable GPIOB clock
	RCC->AHB1ENR |= (1<<2); //enable GPIOC clock
		
	//2. Configure UART pin for Alternate function
	GPIOC->MODER |= (2<<12); //bits [13:12] -> 1:0 -->Alternate function for pin PC6
	GPIOC->MODER |= (2<<14); //bits [15:14] -> 1:0 -->Alternate function for PC7
	
	GPIOC->OSPEEDR |= (3<<12) | (3<<14); //bits [13:12] -> 1:1 -> high speed PC6; bits [15:14] -> 1:1 -> high speed PC7 
	
	GPIOC->AFR[0] |= (8<<24);//Bytes (27:26:25:24) = 1:0:0:0 --> AF8 Alternate function for USART6 at pin PC6
	GPIOC->AFR[0] |= (8<<28); //Bytes (31:30:29:28) = 1:0:0:0 --> AF8 Alternate function for USART6 at pin PC7
	
	//3. Enable UART on USART_CR1 rgister
	USART6->CR1 = 0x00000000; //clear USART
	USART6->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART6->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length

	//5. Select the baud rate using the USART_BRR register.
	USART6->BRR |= (7<<0) | (24<<4); //115200

	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART6->CR1 |= (1<<2); // enable RE for receiver 
	USART6->CR1 |= (1<<3); //enable TE for transmitter
	
	//7. Enable RXNEIE for USART6
	NVIC_EnableIRQ(USART6_IRQn);
	USART6->CR1 |= USART_CR1_RXNEIE;
	
	//To set priority to this interrupt we write
	//NVIC_SetPriority(USART2_IRQn, 1); // replace IRQHandler with IRQn 
//lower the number, higher the priority
}



