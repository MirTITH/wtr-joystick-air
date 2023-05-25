#pragma once

#include "lvgl_app.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl_thread.h"

class TestApp : public LvglApp
{
private:
    /* data */
public:
    using LvglApp::LvglApp; // 使用基类的构造函数

    // virtual int Run() override
    // {
    //     return 0;
    // }

    void Init() override
    {
        auto button = lv_btn_create(app_main_page);
        lv_obj_center(button);
        lv_obj_set_size(button, 80, 50);
        if (app_property.argument != nullptr) {
            auto label = lv_label_create(button);
            lv_label_set_text(label, (const char *)app_property.argument);
            lv_obj_center(label);
        }
    }
};

extern TestApp test_app;
extern TestApp test_app2;
