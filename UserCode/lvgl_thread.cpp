#include "lvgl_thread.h"
#include "lvgl/lvgl.h"
#include "App/status_bar.hpp"

static SemaphoreHandle_t LvglMutex;

BaseType_t LvglLock()
{
    return xSemaphoreTakeRecursive(LvglMutex, portMAX_DELAY);
}

BaseType_t LvglUnlock()
{
    return xSemaphoreGiveRecursive(LvglMutex);
}

static void LvglThreadEntry(void *argument)
{
    (void)argument;

    StatusBar_Init(lv_scr_act());

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    for (;;) {
        LvglLock();
        lv_timer_handler();
        LvglUnlock();

        vTaskDelayUntil(&PreviousWakeTime, 5);
    }
}

void StartLvglThread()
{
    LvglMutex = xSemaphoreCreateRecursiveMutex();
    xTaskCreate(LvglThreadEntry, "lvgl_thread", 2048, nullptr, 3, nullptr);
}
