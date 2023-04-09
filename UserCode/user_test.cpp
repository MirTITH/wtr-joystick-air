#include "user_test.hpp"
#include "cmsis_os.h"
#include "lcd_st7796.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

int32_t TickDiff = 0;

LcdSt7796 LCD(1, &htim8, 1);

uint16_t lcd_buffer[100][100];

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

    LCD.InitAll();

    while (1) {
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                lcd_buffer[i][j] += 1;
            }
        }

        LCD.WriteScreen(100, 199, 200, 299, (uint16_t *)lcd_buffer);
        // TickDiff = xTaskGetTickCount() - HAL_GetTick();
        // HAL_GPIO_TogglePin(Led_Port, Led_Pin);
        // vTaskDelay(500);
    }
}
