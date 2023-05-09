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
#include "Button/joystick_button.hpp"
#include "Button/matrix_keyboard.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

lv_obj_t *ScreenText;

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

    KnobEncoderL.Init();
    KnobEncoderR.Init();

    LvglLock();

    ScreenText = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(ScreenText, lv_pct(100), lv_pct(100));

    // uint64_t start_ns, end_ns, self_ns;

    // uint32_t sum_ns       = 0;
    // uint32_t sum_ns_count = 0;

    LvglUnlock();

    JoystickButtonInit();
    MatrixKeyboard_Init();

    stringstream sstr;

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        // start_ns = HPT_GetNs();
        // self_ns  = HPT_GetNs();

        sstr.str("");

        // auto pos = JoystickL.Pos();

        // sstr << pos.x << " " << pos.y << "    ";

        // pos = JoystickR.Pos();
        // sstr << pos.x << " " << pos.y;

        // sstr << endl;

        // sstr << KnobEncoderL.Count() << " " << KnobEncoderL.ErrorCount() << "    ";
        // sstr << KnobEncoderR.Count() << " " << KnobEncoderR.ErrorCount();
        // sstr << endl;

        // sstr << battery.GetVoltage() << endl;

        // end_ns = HPT_GetNs();

        // sum_ns += end_ns - self_ns - (self_ns - start_ns);
        // sum_ns_count++;

        // sstr << "self:" << self_ns - start_ns << " time:" << sum_ns / sum_ns_count;
        auto result = MatrixKeyboard_Scan();
        // sstr << std::hex << (int);
        for (size_t i = 0; i < 16; i++) {
            sstr << ((result & (1 << i)) != 0);
        }

        sstr << endl;

        SetRow(0);
        vTaskDelay(2);
        sstr << HAL_GPIO_ReadPin(Kb_MC1_GPIO_Port, Kb_MC1_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MC2_GPIO_Port, Kb_MC2_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MC3_GPIO_Port, Kb_MC3_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MC4_GPIO_Port, Kb_MC4_Pin);
        sstr << " ";
        sstr << HAL_GPIO_ReadPin(Kb_MR1_GPIO_Port, Kb_MR1_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MR2_GPIO_Port, Kb_MR2_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MR3_GPIO_Port, Kb_MR3_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MR4_GPIO_Port, Kb_MR4_Pin);
        sstr << endl;

        SetRow(2);
        vTaskDelay(2);
        sstr << HAL_GPIO_ReadPin(Kb_MC1_GPIO_Port, Kb_MC1_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MC2_GPIO_Port, Kb_MC2_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MC3_GPIO_Port, Kb_MC3_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MC4_GPIO_Port, Kb_MC4_Pin);
        sstr << " ";
        sstr << HAL_GPIO_ReadPin(Kb_MR1_GPIO_Port, Kb_MR1_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MR2_GPIO_Port, Kb_MR2_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MR3_GPIO_Port, Kb_MR3_Pin);
        sstr << HAL_GPIO_ReadPin(Kb_MR4_GPIO_Port, Kb_MR4_Pin);

        LvglLock();
        lv_textarea_set_text(ScreenText, sstr.str().c_str());
        LvglUnlock();
        // flex_button_scan();
        vTaskDelayUntil(&PreviousWakeTime, 50);
    }
}
