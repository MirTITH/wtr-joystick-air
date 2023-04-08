#include "user_test.hpp"
#include "cmsis_os.h"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

int32_t TickDiff = 0;

void TestThreadEntry(void *argument)
{
    (void)argument;
    GPIO_InitTypeDef GPIO_InitStruct = {};
    GPIO_InitStruct.Pin              = Led_Pin;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Led_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(Led_Port, Led_Pin, GPIO_PIN_RESET);

    while (1) {
        TickDiff = xTaskGetTickCount() - HAL_GetTick();
        HAL_GPIO_TogglePin(Led_Port, Led_Pin);
        vTaskDelay(500);
    }
}
