#include "lvgl_thread.h"
#include "lvgl.h"
#include "demos/lv_demos.h"

static SemaphoreHandle_t LvglMutex;

BaseType_t LvglLock(TickType_t block_time = portMAX_DELAY)
{
    return xSemaphoreTakeRecursive(LvglMutex, block_time);
}

BaseType_t LvglUnlock()
{
    return xSemaphoreGiveRecursive(LvglMutex);
}

static void LvglThreadEntry(void *argument)
{
    (void)argument;

    LvglMutex = xSemaphoreCreateRecursiveMutex();

    lv_demo_benchmark();

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
    xTaskCreate(LvglThreadEntry, "lvgl_thread", 2048, nullptr, 3, nullptr);
}
