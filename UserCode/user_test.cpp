#include "user_test.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "driver_mpu9250_basic.h"
#include "adc.h"
#include "lvgl_thread.h"
#include "lvgl.h"
#include <sstream>
#include "Adc/adc_class_define.hpp"
#include "Battery/battery_define.hpp"
#include "Joystick/joystick_define.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

void TestThreadEntry(void *argument)
{
    (void)argument;

    Adc3.Init();
    Adc3.StartDma();
    JoystickL.x_max    = 0.96;
    JoystickL.x_middle = 0.4435;
    JoystickL.x_min    = 0;
    JoystickL.y_max    = 0.9987;
    JoystickL.y_middle = 0.505;
    JoystickL.y_min    = 0;
    JoystickL.Init();

    JoystickR.x_max    = 0.7;
    JoystickR.x_middle = 0.526;
    JoystickR.x_min    = 0;
    JoystickR.y_max    = 0.9995;
    JoystickR.y_middle = 0.51;
    JoystickR.y_min    = 0;
    JoystickR.Init();

    LvglLock();

    auto text = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(text, lv_pct(100), lv_pct(100));

    LvglUnlock();

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        stringstream sstr;
        sstr.precision(6);
        sstr.setf(std::ios::fixed);

        auto pos = JoystickL.Pos();
        sstr << pos.x << " " << pos.y << "\t";

        pos = JoystickR.Pos();
        sstr << pos.x << " " << pos.y;

        sstr << endl;
        // sstr << JoystickL.RawPos().x << " " << JoystickL.RawPos().y << "\t";
        // sstr << JoystickR.RawPos().x << " " << JoystickR.RawPos().y;
        // sstr << endl;

        // for (auto &volt : Adc1.GetAllVoltage()) {
        //     sstr << volt << " ";
        // }
        // for (auto &volt : Adc2.GetAllVoltage()) {
        //     sstr << volt << " ";
        // }
        sstr << endl;

        sstr << battery.GetVoltage();

        LvglLock();
        lv_textarea_set_text(text, sstr.str().c_str());
        LvglUnlock();
        vTaskDelayUntil(&PreviousWakeTime, 20);
    }
}
