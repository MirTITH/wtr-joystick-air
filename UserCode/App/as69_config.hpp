#pragma once

#include "lvgl_app.hpp"
#include "As69/as69.hpp"
#include "lvgl/lvgl.h"

class As69Config : public LvglApp
{
private:
    As69 &as69_;
    lv_obj_t *btn_tab_              = nullptr;
    lv_obj_t *btn_readConfig_       = nullptr;
    lv_obj_t *btn_readConfigLabel_  = nullptr;
    lv_obj_t *btn_writeConfig_      = nullptr;
    lv_obj_t *btn_writeConfigLabel_ = nullptr;
    lv_obj_t *sw_cmd_               = nullptr;
    lv_obj_t *sw_cmd_label_         = nullptr;

    static void sw_event_handler(lv_event_t *e)
    {
        auto this_class = (As69Config *)(e->user_data);
        if (this_class == nullptr) {
            return;
        }

        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *obj        = lv_event_get_target(e);

        if (code == LV_EVENT_VALUE_CHANGED) {
            this_class->SetSwLabel(lv_obj_has_state(obj, LV_STATE_CHECKED));
        }
    }

    void SetSwLabel(bool is_sw_checked)
    {
        lv_label_set_text_static(sw_cmd_label_, is_sw_checked ? "As69\nWork\nMode" : "As69\nConfig\nMode");
        lv_obj_align_to(sw_cmd_label_, sw_cmd_, LV_ALIGN_OUT_TOP_MID, 0, -10);
    }

    void InitStyles();

    lv_obj_t *CreateTab(lv_obj_t *parent, const std::string &title);
    lv_obj_t *CreateSettingTab(lv_obj_t *parent, const std::string &title, const char *options);
    lv_obj_t *Create4RollerTab(lv_obj_t *parent, const std::string &title, const char *option);

public:
    As69Config(As69 &as69, const std::string &app_name = "As69Config")
        : LvglApp(app_name), as69_(as69){};

    void Init() override;
};

extern As69Config InternalAs69Config;
