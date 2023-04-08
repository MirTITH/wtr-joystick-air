#include "user_test.hpp"
#include "cmsis_os.h"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

int TestValue = 0;

void TestThreadEntry(void *argument)
{
    (void)argument;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = Led_Pin;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Led_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(Led_Port, Led_Pin, GPIO_PIN_SET);

    while (1) {
        TestValue++;
        // HAL_GPIO_TogglePin(Led_Port, Led_Pin);
        vTaskDelay(500);
    }
}

void PreSleepProcessing(uint32_t ulExpectedIdleTime)
{
    HAL_GPIO_WritePin(Led_Port, Led_Pin, GPIO_PIN_SET);
}

void PostSleepProcessing(uint32_t ulExpectedIdleTime)
{
    HAL_GPIO_WritePin(Led_Port, Led_Pin, GPIO_PIN_RESET);
}
