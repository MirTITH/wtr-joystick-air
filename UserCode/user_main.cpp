#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
#include "user_test.hpp"

__attribute__((section(".itcmram"))) void StartDefaultTask(void const *argument)
{
    (void)argument;
    xTaskCreate(TestThreadEntry, "TestThread", 2048, nullptr, osPriorityNormal, nullptr);
    vTaskDelete(nullptr);
}
