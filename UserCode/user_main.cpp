#include "user_main.hpp"
#include "main.h"
#include "FreeRTOS.h"
#include "user_test.hpp"
#include "lvgl/lvgl.h"
#include "lv_port_disp.h"
#include "lvgl_thread.h"

void StartDefaultTask(void const *argument)
{
    (void)argument;
    HAL_Delay(250); // 用于防止进入睡眠模式时无法烧录和调试
    lv_init();
    lv_port_disp_init();
    StartLvglThread();
    xTaskCreate(TestThreadEntry, "TestThread", 2048, nullptr, 2, nullptr);
    vTaskDelete(nullptr);
}
