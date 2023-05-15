#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "TIMER.h"
#include "SPI.h"

void SPIConfigMaster (void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable SPI clock
	2. Configure the Control Register 1
	3. Configure the CR2
	************************************************/	
	
	//1. Enable SPI clock
	RCC->APB2ENR |= (1<<12);  // Enable SPI1 CLock
	RCC->AHB1ENR |= (1<<0);  //Enable GPIOA CLock
	
	//2. GPIO Registers configuration(MOSI, MISO, SCK)
	//PA4->Chip/Slave Select
	//PA5->MOSI
	//PA6->MISO
	//PA7->SCK
	
	//a) Mode Set
	GPIOA->MODER |= (2 << 8);//PA4 
	GPIOA->MODER |= (2 << 10);//PA5 
	GPIOA->MODER |= (2 << 12);//PA6 
	GPIOA->MODER |= (2 << 14);//PA7
	
	//b)Speed Set
	GPIOA->OSPEEDR |= (3 << 8);//PA4 
	GPIOA->OSPEEDR |= (3 << 10);//PA5 
	GPIOA->OSPEEDR |= (3 << 12);//PA6 
	GPIOA->OSPEEDR |= (3 << 14);//PA7
	
	//c)Alternate Function Set
	GPIOA->AFR[0] |= (5 << 16);//PA4 
	GPIOA->AFR[0] |= (5 << 20);//PA5 
	GPIOA->AFR[0] |= (5 << 24);//PA6 
	GPIOA->AFR[0] |= (5 << 28);//PA7
	
	
	//2. Configure the Control Register 1
	//a) BR bits configuration*****Master****
	SPI1->CR1 |= (7<<3);  // BR[2:0] = 111: fPCLK/16, PCLK2 = 90MHz, SPI clk = 5MHz
	
	//b) CPOL & CPHA bit configuration
	SPI1->CR1 &= ~(1<<1);  // CPOL=0 
	SPI1->CR1 |= (1<<0); //CPHA=1
	
	//c) RXONLY bit configuration
	SPI1->CR1 &= ~(1<<10);  // RXONLY = 0, full-duplex
	
	//d) LSBFIRST bit configuration
	SPI1->CR1 &= ~(1<<7);  // LSBFIRST = 0, MSB first
	
	//e) SSI & SSM bit configuration*****Master******
	SPI1->CR1 |= (1<<8) | (1<<9);  // SSI=1, SSM=1 -> Software Slave Management
	
	//f) MSTR bit configuration*****Master******
	SPI1->CR1 |= (1<<2);  // Master Mode
	
	//g) MSTR bit configuration*****Slave******
	//SPI1->CR1 &= ~(1<<2);  // Slave Mode
	
	//h) DFF bit configuration
	//SPI1->CR1 &= ~(1<<11);  // DFF=0, 8 bit data
	
	//g) SPE bit Enable
	SPI1->CR1 |= (1<<6);  //SPI Enable
	
	
	//3. Configure the CR2 for Reception Interrupt*****Slave****
	//SPI1->CR2 |= (1<<6);
	//NVIC_EnableIRQ(SPI1_IRQn);
	//NVIC_DisableIRQ(USART2_IRQn);
}


void SPIConfigSlave (void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable SPI clock
	2. Configure the Control Register 1
	3. Configure the CR2
	************************************************/	
	
	//1. Enable SPI clock
	RCC->APB2ENR |= (1<<12);  // Enable SPI1 CLock
	RCC->AHB1ENR |= (1<<0);  //Enable GPIOA CLock
	
	//2. GPIO Registers configuration(MOSI, MISO, SCK)
	//PA4->Chip/Slave Select
	//PA5->MOSI
	//PA6->MISO
	//PA7->SCK
	
	//a) Mode Set
	GPIOA->MODER |= (2 << 8);//PA4 
	GPIOA->MODER |= (2 << 10);//PA5 
	GPIOA->MODER |= (2 << 12);//PA6 
	GPIOA->MODER |= (2 << 14);//PA7
	
	//b)Speed Set
	GPIOA->OSPEEDR |= (3 << 8);//PA4 
	GPIOA->OSPEEDR |= (3 << 10);//PA5 
	GPIOA->OSPEEDR |= (3 << 12);//PA6 
	GPIOA->OSPEEDR |= (3 << 14);//PA7
	
	//c)Alternate Function Set
	GPIOA->AFR[0] |= (5 << 16);//PA4 
	GPIOA->AFR[0] |= (5 << 20);//PA5 
	GPIOA->AFR[0] |= (5 << 24);//PA6 
	GPIOA->AFR[0] |= (5 << 28);//PA7
	
	
	//2. Configure the Control Register 1
	//a) BR bits configuration*****Master****
	//SPI1->CR1 |= (7<<3);  // BR[2:0] = 111: fPCLK/16, PCLK2 = 90MHz, SPI clk = 5MHz
	
	//b) CPOL & CPHA bit configuration
	SPI1->CR1 &= ~(1<<1);  // CPOL=0 
	SPI1->CR1 |= (1<<0); //CPHA=1
	
	//c) RXONLY bit configuration
	SPI1->CR1 &= ~(1<<10);  // RXONLY = 0, full-duplex
	
	//d) LSBFIRST bit configuration
	SPI1->CR1 &= ~(1<<7);  // LSBFIRST = 0, MSB first
	
	//e) SSI & SSM bit configuration*****Master******
	//SPI1->CR1 |= (1<<8) | (1<<9);  // SSI=1, SSM=1 -> Software Slave Management
	
	//f) MSTR bit configuration*****Master******
	//SPI1->CR1 |= (1<<2);  // Master Mode
	
	//g) MSTR bit configuration*****Slave******
	SPI1->CR1 &= ~(1<<2);  // Slave Mode
	
	//h) DFF bit configuration
	SPI1->CR1 &= ~(1<<11);  // DFF=0, 8 bit data
	
	//g) SPE bit Enable
	SPI1->CR1 |= (1<<6);  //SPI Enable
	
	
	//3. Configure the CR2 for Reception Interrupt*****Slave****
	SPI1->CR2 |= (1<<6);
	NVIC_EnableIRQ(SPI1_IRQn);
	NVIC_DisableIRQ(USART2_IRQn);
}





