#include "mpu_stm32.h"
#include "main.h"
#include "i2c.h"
#include <stdarg.h>
#include <stdio.h>

#define MPU_I2C hi2c2

unsigned char *mpl_key = (unsigned char*)"eMPL 5.1";

uint8_t MPU_i2cWrite(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data)
{
    return HAL_I2C_Mem_Write(&MPU_I2C, slave_addr << 1, reg_addr, 1, data, length, 0xff);
}

uint8_t MPU_i2cRead(uint8_t slave_addr, uint8_t reg_addr, uint8_t length, uint8_t *data)
{
    return HAL_I2C_Mem_Read(&MPU_I2C, slave_addr << 1, reg_addr, 1, data, length, 0xff);
}

void mpl_getms(uint32_t *num)
{
    *num = HAL_GetTick();
}

int _MLPrintLog(int priority, const char *tag, const char *fmt, ...)
{
    (void)priority;
    (void)tag;

    va_list arg;
    int done;

    va_start(arg, fmt);
    done = vfprintf(stdout, fmt, arg);
    va_end(arg);

    return done;
}