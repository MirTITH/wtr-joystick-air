#include "main.h"
#include "lcd_st7796.hpp"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / configTICK_RATE_HZ);
}

#ifdef __cplusplus
}
#endif

void LcdFmc_DmaXferCpltCallback(MDMA_HandleTypeDef *_hmdma)
{
    extern LcdSt7796 LCD;

    if (_hmdma->Instance == LCD.GetMdma()->Instance) {
        LCD.DmaXferCpltCallback();
    }
}