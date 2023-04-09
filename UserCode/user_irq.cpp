#include "main.h"
#include "lcd_st7796.hpp"

void LcdFmc_DmaXferCpltCallback(MDMA_HandleTypeDef *_hmdma)
{
    extern LcdSt7796 LCD;

    if (_hmdma->Instance == LCD.GetMdma()->Instance) {
        LCD.DmaXferCpltCallback();
    }
}