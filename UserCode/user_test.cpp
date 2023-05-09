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
#include "Encoder/encoder_define.hpp"
#include "HighPrecisionTime/high_precision_time.h"
#include "Button/buttons.h"
#include "Led/led_define.hpp"
// #include <iomanip>

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

lv_obj_t *ScreenText;

void TestThreadEntry(void *argument)
{
    (void)argument;

    Adc3.Init();
    Adc3.StartDma();

    JoystickL.x_max    = 0.9;
    JoystickL.x_middle = 0.4475;
    JoystickL.x_min    = 0.05;

    JoystickL.y_max    = 0.95;
    JoystickL.y_middle = 0.505;
    JoystickL.y_min    = 0.05;
    JoystickL.Init();

    JoystickR.x_max    = 0.95;
    JoystickR.x_middle = 0.526;
    JoystickR.x_min    = 0.05;

    JoystickR.y_max    = 0.95;
    JoystickR.y_middle = 0.51;
    JoystickR.y_min    = 0.05;
    JoystickR.Init();

    KnobEncoderL.Init();
    KnobEncoderR.Init();

    LvglLock();

    ScreenText = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(ScreenText, lv_pct(100), lv_pct(100));

    // uint64_t start_ns, end_ns, self_ns;

    // uint32_t sum_ns       = 0;
    // uint32_t sum_ns_count = 0;

    LvglUnlock();

    Buttons_Init();

    stringstream sstr;
    sstr.precision(4);
    sstr.setf(std::ios::fixed);

    KeyboardLed.Init();

    float r = 0, g = 0, b = 0, lightfactor = 1;

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {

        sstr.str("");

        auto pos = JoystickL.Pos();
        sstr << "JoystickL: " << pos.x << " " << pos.y;
        pos = JoystickL.RawPos();
        sstr << " RowValue: " << pos.x << " " << pos.y << endl;

        pos = JoystickR.Pos();
        sstr << "JoystickR: " << pos.x << " " << pos.y;
        pos = JoystickR.RawPos();
        sstr << " RowValue: " << pos.x << " " << pos.y << endl;

        sstr << "KnobL: " << KnobEncoderL.Count() << " ErrCount: " << KnobEncoderL.ErrorCount() << "    ";
        sstr << "KnobR: " << KnobEncoderR.Count() << " ErrCount: " << KnobEncoderR.ErrorCount() << endl;

        Buttons_Scan();

        if (Buttons_Read(Btn_LeftCrossUp)) {
            g += 0.001;
        }
        if (Buttons_Read(Btn_LeftCrossDown)) {
            g -= 0.001;
        }
        if (Buttons_Read(Btn_LeftCrossLeft)) {
            b -= 0.001;
        }
        if (Buttons_Read(Btn_LeftCrossRight)) {
            b += 0.001;
        }
        if (Buttons_Read(Btn_RightCrossLeft)) {
            r -= 0.001;
        }
        if (Buttons_Read(Btn_RightCrossRight)) {
            r += 0.001;
        }
        if (Buttons_Read(Btn_RightCrossUp)) {
            lightfactor += 0.01;
        }
        if (Buttons_Read(Btn_RightCrossDown)) {
            lightfactor -= 0.01;
        }

        KeyboardLed.SetColor(r, g, b, lightfactor);

        sstr << "r g b l:" << r << " " << g << " " << b << " " << lightfactor << endl;

        sstr << "Keys: ";

        for (size_t i = 1; i <= 22; i++) {
            sstr << (int)Buttons_Read(i);
        }

        sstr << endl;

        sstr << "Voltage: " << Batt.GetVoltage() << "V; "
             << "Single battery: " << Batt.GetVoltage() / 2 << "V" << endl;

        // start_ns    = HPT_GetNs();
        // self_ns     = HPT_GetNs();

        // end_ns      = HPT_GetNs();
        // sum_ns += end_ns - self_ns - (self_ns - start_ns);
        // sum_ns_count++;
        // sstr << "Avg time:" << (float)sum_ns / sum_ns_count << endl;

        sstr << "Temperature: " << GetCoreTemperature() << endl;

        LvglLock();
        lv_textarea_set_text(ScreenText, sstr.str().c_str());
        LvglUnlock();

        vTaskDelayUntil(&PreviousWakeTime, 20);
    }
}
