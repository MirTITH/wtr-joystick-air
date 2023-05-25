#include "user_main.hpp"
#include "main.h"
#include "FreeRTOS.h"
#include "user_test.hpp"
#include "lvgl/lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl_thread.h"

void StartDefaultTask(void const *argument)
{
    (void)argument;
    HAL_Delay(1000); // 等待到整个系统稳定
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    StartLvglThread();
    xTaskCreate(TestThreadEntry, "TestThread", 2048, nullptr, 2, nullptr);
    vTaskDelete(nullptr);
}
