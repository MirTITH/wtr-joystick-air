#pragma once

#include "lvgl_app.hpp"
#include "As69/as69.hpp"
#include "lvgl/lvgl.h"
#include "FreeRTOS.h"
#include "task.h"

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
    lv_obj_t *dropdown_baud_rate_   = nullptr;
    lv_obj_t *dropdown_parity_      = nullptr;
    lv_obj_t *dropdown_channel_     = nullptr;
    lv_obj_t *dropdown_power_       = nullptr;
    lv_obj_t *dropdown_io_config_   = nullptr;
    lv_obj_t *address_roller[4]     = {};

    static void sw_event_handler(lv_event_t *e)
    {
        auto this_class = (As69Config *)(e->user_data);
        if (this_class == nullptr) {
            return;
        }

        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *obj        = lv_event_get_target(e);

        if (code == LV_EVENT_VALUE_CHANGED) {
            this_class->OnSwUpdated(lv_obj_has_state(obj, LV_STATE_CHECKED));
        }
    }

    static void readBtn_event_handler(lv_event_t *e)
    {
        auto this_class = (As69Config *)(e->user_data);
        if (this_class == nullptr) {
            return;
        }

        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_CLICKED) {
            this_class->OnReadBtnClicked();
        }
    }

    static void writeBtn_event_handler(lv_event_t *e)
    {
        auto this_class = (As69Config *)(e->user_data);
        if (this_class == nullptr) {
            return;
        }

        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_CLICKED) {
            this_class->OnWriteBtnClicked();
        }
    }

    void OnSwUpdated(bool is_sw_checked);

    void SetAddressRoller(uint16_t address)
    {
        for (size_t i = 0; i < 4; i++) {
            lv_roller_set_selected(address_roller[i], (address >> (3 - i) * 4) & 0xF, LV_ANIM_ON);
        }
    }

    uint16_t ReadAddressRoller() const
    {
        uint16_t result = 0;
        for (size_t i = 0; i < 4; i++) {
            result |= lv_roller_get_selected(address_roller[3 - i]) << i * 4;
        }
        return result;
    }

    void OnReadBtnClicked()
    {
        as69_.SetMode(As69::Mode::Sleep);
        if (as69_.ReadConfig() == true) {
            lv_label_set_text_static(btn_readConfigLabel_, "Read success");

            auto baudrate = as69_.GetModuleBaudRate();
            int32_t index;
            index = lv_dropdown_get_option_index(dropdown_baud_rate_, std::to_string(baudrate).c_str());
            if (index != -1) {
                lv_dropdown_set_selected(dropdown_baud_rate_, index);
            }

            auto parity = as69_.GetModuleParity();
            switch (parity) {
                case As69::Parity::None:
                    lv_dropdown_set_selected(dropdown_parity_, 0);
                    break;
                case As69::Parity::Odd:
                    lv_dropdown_set_selected(dropdown_parity_, 1);
                    break;
                case As69::Parity::Even:
                    lv_dropdown_set_selected(dropdown_parity_, 2);
                    break;

                default:
                    break;
            }

            lv_dropdown_set_selected(dropdown_channel_, as69_.GetModuleWirelessChannel());
            lv_dropdown_set_selected(dropdown_power_, as69_.GetModuleSendPower());
            lv_dropdown_set_selected(dropdown_io_config_, as69_.GetModuleIoDrivingMode());
            SetAddressRoller(as69_.GetModuleAddress());

        } else {
            lv_label_set_text_static(btn_readConfigLabel_, "Read failed");
        }
    }

    void OnWriteBtnClicked()
    {
        as69_.SetMode(As69::Mode::Sleep);
        char temp[10];
        lv_dropdown_get_selected_str(dropdown_baud_rate_, temp, sizeof(temp));
        auto baudrate = atoi(temp);

        As69::Parity parity;
        switch (lv_dropdown_get_selected(dropdown_parity_)) {
            case 0:
                parity = As69::Parity::None;
                break;
            case 1:
                parity = As69::Parity::Odd;
                break;
            case 2:
                parity = As69::Parity::Even;
                break;
            default:
                return;
                break;
        }

        uint8_t wireless_channel = lv_dropdown_get_selected(dropdown_channel_);
        uint8_t send_power       = lv_dropdown_get_selected(dropdown_power_);
        uint8_t io_cfg           = lv_dropdown_get_selected(dropdown_io_config_);

        if (as69_.WriteConfig(ReadAddressRoller(), baudrate, parity, wireless_channel, send_power, io_cfg) == true) {
            lv_label_set_text_static(btn_writeConfigLabel_, "Write success");
        } else {
            lv_label_set_text_static(btn_writeConfigLabel_, "Write failed");
        }
    }

    void InitStyles();

    lv_obj_t *CreateTab(lv_obj_t *parent, const std::string &title);
    lv_obj_t *CreateDropdownTab(lv_obj_t *parent, const std::string &title, const char *options);
    lv_obj_t *Create4RollerTab(lv_obj_t *parent, const std::string &title, const char *option);

public:
    As69Config(As69 &as69, const std::string &app_name = "As69Config")
        : LvglApp(app_name), as69_(as69){};

    void Init() override;
};

extern As69Config InternalAs69Config;
