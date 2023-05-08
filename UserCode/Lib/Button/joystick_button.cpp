#include "joystick_button.hpp"
#include "main.h"
#include "lvgl.h"
#include "lvgl_thread.h"
#include <sstream>

using namespace std;

extern lv_obj_t *ScreenText;

static uint8_t ButtonGroupA_Read(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    switch (btn->id) {
        case 0:
            return HAL_GPIO_ReadPin(KnobL_Btn_GPIO_Port, KnobL_Btn_Pin);
            break;
        case 1:
            return HAL_GPIO_ReadPin(KnobR_Btn_GPIO_Port, KnobR_Btn_Pin);
            break;
        case 2:
            return HAL_GPIO_ReadPin(JoystickL_Btn_GPIO_Port, JoystickL_Btn_Pin);
            break;
        case 3:
            return HAL_GPIO_ReadPin(JoystickR_Btn_GPIO_Port, JoystickR_Btn_Pin);
            break;

        default:
            return 1;
            break;
    }
}

static void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    stringstream sstr;

    sstr << (int)btn->id << " " << (int)btn->event << " " << btn->click_cnt << endl;

    LvglLock();
    lv_textarea_add_text(ScreenText, sstr.str().c_str());
    LvglUnlock();

    // rt_kprintf("id: [%d - %s]  event: [%d - %30s]  repeat: %d\n",
    //            btn->id, enum_btn_id_string[btn->id],
    //            btn->event, enum_event_string[btn->event],
    //            btn->click_cnt);

    // if ((flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_CLICK) &&
    //     (flex_button_event_read(&user_button[USER_BUTTON_1]) == FLEX_BTN_PRESS_CLICK)) {
    //     rt_kprintf("[combination]: button 0 and button 1\n");
    // }
}

void JoystickButtonInit()
{
    // 旋钮和摇杆的键
    // [0] 左旋钮，[1] 右旋钮，[2] 左摇杆，[3]，右摇杆
    static flex_button_t buttonGroupA[4];

    for (int i = 0; i < 4; i++) {
        buttonGroupA[i].id                     = i;
        buttonGroupA[i].usr_button_read        = ButtonGroupA_Read;
        buttonGroupA[i].cb                     = common_btn_evt_cb;
        buttonGroupA[i].pressed_logic_level    = 0;
        buttonGroupA[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(500);
        buttonGroupA[i].long_press_start_tick  = FLEX_MS_TO_SCAN_CNT(1000);
        buttonGroupA[i].long_hold_start_tick   = FLEX_MS_TO_SCAN_CNT(2000);

        flex_button_register(&buttonGroupA[i]);
    }
}
