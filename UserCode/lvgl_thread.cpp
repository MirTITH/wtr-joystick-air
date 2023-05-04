#include "lvgl_thread.h"
#include "lvgl.h"
#include "test_app.hpp"
// #include "demos/lv_demos.h"

static SemaphoreHandle_t LvglMutex;

BaseType_t LvglLock()
{
    return xSemaphoreTakeRecursive(LvglMutex, portMAX_DELAY);
}

BaseType_t LvglUnlock()
{
    return xSemaphoreGiveRecursive(LvglMutex);
}

static void LvglAppRunnerEntry(void *argument)
{
    auto app = (LvglApp *)argument;
    switch (app->app_type) {
        case LvglAppType::Window:
            app->app_screen = lv_obj_create(lv_scr_act());
            lv_obj_set_align(app->app_screen, LV_ALIGN_CENTER);
            break;
        case LvglAppType::FullScreen:
            app->app_screen = lv_obj_create(nullptr);
            break;
        case LvglAppType::NoGUI:
            /* code */
            break;

        default:
            break;
    }
    app->Run();
    vTaskDelete(nullptr);
}

static void LvglThreadEntry(void *argument)
{
    (void)argument;

    // lv_demo_benchmark();

    // LvglApp *app1 = new TestApp();

    // xTaskCreate(LvglAppRunnerEntry, app1->app_name.c_str(), app1->GetStackDepth(), app1, app1->GetPriority(), nullptr);

    // auto background_text = lv_textarea_create(lv_scr_act());
    // lv_textarea_add_text(background_text, "This is the background text!");

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
