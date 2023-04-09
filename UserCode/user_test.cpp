#include "user_test.hpp"
#include "cmsis_os.h"
#include "lcd_st7796.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

LcdSt7796 LCD(1,
              LcdSt7796::Rotation::landscape,
              LcdSt7796::ColorOrder::BGR,
              &htim8, 1,
              &hmdma_mdma_channel40_sw_0);

void TestThreadEntry(void *argument)
{
    (void)argument;

    while (1) {
        vTaskDelay(500);
    }
}
