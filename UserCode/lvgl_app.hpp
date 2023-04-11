#pragma once

#include <string>
#include "lvgl.h"

enum class LvglAppType {
    Window,
    NoGUI,
    FullScreen
};

class LvglApp
{
private:
protected:
    uint16_t _stack_depth; // 栈大小，单位：word (4 bytes)
    uint16_t _priority;    // FreeRTOS 优先级，正常优先级是3，值越高优先级越高。范围：[0, configMAX_PRIORITIES]
    void *_argument;

public:
    lv_obj_t *app_screen = nullptr;
    const std::string app_name; // 应用的名字
    const LvglAppType app_type;

    virtual int Run() = 0;
    virtual int OnLaunch()
    {
        return 0;
    }
    virtual int OnExit()
    {
        return 0;
    }

    LvglApp(std::string app_name = "Untitled",
            LvglAppType app_type = LvglAppType::Window,
            void *argument       = nullptr,
            uint16_t stack_depth = 512,
            uint16_t priority    = 3)
        : _stack_depth(stack_depth),
          _priority(priority),
          _argument(argument),
          app_name(app_name),
          app_type(app_type){};

    ~LvglApp(){};

    uint16_t GetStackDepth() const
    {
        return _stack_depth;
    }

    uint16_t GetPriority() const
    {
        return _priority;
    }

    void ExitApp()
    {
        OnExit();
    }
};
