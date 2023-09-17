#pragma once

#include "main.h"

void mpl_getms(uint32_t *num);
uint8_t MPU_i2cWrite(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);
uint8_t MPU_i2cRead(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data);

#define i2c_write MPU_i2cWrite
#define i2c_read  MPU_i2cRead
#define delay_ms  HAL_Delay
#define get_ms    mpl_getms
#define log_i     printf
#define log_e     printf
#define min(a, b) ((a < b) ? a : b)
