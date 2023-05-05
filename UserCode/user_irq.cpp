#include "main.h"
#include "lcd_st7796.hpp"
#include "lvgl.h"
#include "adc.h"
#include "Encoder/encoder_define.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     extern uint32_t Adc3CpltCount;
//     extern uint32_t Adc1CpltCount;
//     if (hadc->Instance == hadc1.Instance) {
//         Adc1CpltCount++;
//     } else if (hadc->Instance == hadc3.Instance) {
//         Adc3CpltCount++;
//     }
// }

// void vApplicationTickHook(void)
// {
//     lv_tick_inc(1000 / configTICK_RATE_HZ);
// }

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin) {
        case KnobL_A_Pin:
            KnobEncoderL.ExtiCallback(HAL_GPIO_ReadPin(KnobL_A_GPIO_Port, KnobL_A_Pin), HAL_GPIO_ReadPin(KnobL_B_GPIO_Port, KnobL_B_Pin));
            break;
        case KnobL_B_Pin:
            KnobEncoderL.ExtiCallback(HAL_GPIO_ReadPin(KnobL_A_GPIO_Port, KnobL_A_Pin), HAL_GPIO_ReadPin(KnobL_B_GPIO_Port, KnobL_B_Pin));
            break;
        case KnobR_A_Pin:
            KnobEncoderR.ExtiCallback(HAL_GPIO_ReadPin(KnobR_A_GPIO_Port, KnobR_A_Pin), HAL_GPIO_ReadPin(KnobR_B_GPIO_Port, KnobR_B_Pin));
            break;
        case KnobR_B_Pin:
            KnobEncoderR.ExtiCallback(HAL_GPIO_ReadPin(KnobR_A_GPIO_Port, KnobR_A_Pin), HAL_GPIO_ReadPin(KnobR_B_GPIO_Port, KnobR_B_Pin));
            break;

        default:
            break;
    }
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