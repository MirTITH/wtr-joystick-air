#include "user_main.hpp"
#include "main.h"
#include "FreeRTOS.h"
#include "user_test.hpp"
#include "lvgl/lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl_thread.h"
#include "Joystick/joystick_define.hpp"
#include "Encoder/encoder_define.hpp"
#include "Button/buttons.h"
#include "Led/led_define.hpp"
#include "Encoder/encoder_define.hpp"

void StartDefaultTask(void const *argument)
{
    (void)argument;
    HAL_Delay(500); // 等待到整个系统稳定

    Joystick_Init();

    KnobEncoderL.Init();
    KnobEncoderR.Init();

    Buttons_Init();

    KeyboardLed.Init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    StartLvglThread();
    xTaskCreate(TestThreadEntry, "TestThread", 2048, nullptr, 2, nullptr);
    vTaskDelete(nullptr);
}
