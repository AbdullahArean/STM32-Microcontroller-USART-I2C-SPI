#ifndef __SPI_H
#define __SPI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx.h"

void SPIConfig (void);
void SPI_Transmit(uint8_t *data, int size);
void SPI_DataReception (void);
void SPI1_IRQHandler(void);
void SPIConfigMaster (void);
void SPIConfigSlave (void);

#ifdef __cplusplus
}
#endif
#endif
