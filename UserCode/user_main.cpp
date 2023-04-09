#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
#include "user_test.hpp"
#include "lvgl.h"
#include "lv_port_disp.h"

void StartDefaultTask(void const *argument)
{
    (void)argument;
    HAL_Delay(250); // 用于防止进入睡眠模式时无法烧录和调试
    xTaskCreate(TestThreadEntry, "TestThread", 2048, nullptr, osPriorityNormal, nullptr);
    lv_init();
    lv_port_disp_init();
    vTaskDelete(nullptr);
}
