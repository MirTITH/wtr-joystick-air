#pragma once

#include "lvgl_app.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl_thread.h"

class TestApp : public LvglApp
{
private:
    bool isGoingToExit = false;

public:
    int Run() override
    {
        LvglLock();
        auto textarea = lv_textarea_create(app_screen);
        lv_obj_set_height(textarea, lv_pct(100));
        lv_obj_set_width(textarea, lv_pct(100));
        std::string str;
        str = "Created by " + app_name;
        lv_textarea_set_text(textarea, str.c_str());
        LvglUnlock();
        while (true) {
            vTaskDelay(100);
        }

        return 0;
    }

    int OnLaunch() override
    {
        return 0;
    }
    int OnExit() override
    {
        return 0;
    }

    TestApp(std::string app_name = "Untitled",
            void *argument       = nullptr,
            uint16_t stack_depth = 512,
            uint16_t priority    = 3)
        : LvglApp(app_name, LvglAppType::Window, argument, stack_depth, priority)
    {
    }

    ~TestApp()
    {
    }
};
