#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "TIMER.h"

void I2C1_Config (void);
void I2C2_Config (void);
void I2C_Start (void);
void I2C_Write (uint8_t data);
void I2C_Address (uint8_t Address);
void I2C_Stop (void);
void I2C_WriteMulti (uint8_t *data);
void I2C_Read (uint8_t Address, uint8_t *buffer, uint8_t size);
void I2CMem_read(uint8_t slave_address,uint8_t reg_address,uint8_t* trimdata,uint8_t size);
//void I2CMem_write(uint8_t slave_address,uint8_t reg_address,uint8_t data);
void I2CMem_write(uint8_t slave_address,uint8_t *data);
void I2C2_EV_IRQHandler(void);