#include "user_test.hpp"
#include "FreeRTOS.h"
#include "task.h"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

void TestThreadEntry(void *argument)
{
    (void)argument;

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    for (;;) {

        vTaskDelayUntil(&PreviousWakeTime, 500);
    }
}
