#include "user_test.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "driver_mpu9250_basic.h"
#include "adc.h"
#include "lvgl_thread.h"
#include "lvgl/lvgl.h"
#include <sstream>
#include "HighPrecisionTime/high_precision_time.h"
#include "Mavlink/wtr_mavlink.h"
#include <iomanip>
#include "app_mgr.hpp"
#include "test_app.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

// void MavlinkSenderEntry(void *argument)
// {
//     (void)argument;
//     wtrMavlink_BindChannel(&huart1, MAVLINK_COMM_0);
//     mavlink_joystick_air_t msg = {};

//     Joystick::Point_t pos;

//     vTaskDelay(2000);

//     while (true) {
//         for (size_t i = 0; i < 20; i++) {
//             msg.buttons[i] = Buttons_Read(i + 1);
//         }

//         msg.switchs = 0;
//         msg.switchs |= Buttons_Read(Switch_L);
//         msg.switchs |= Buttons_Read(Switch_R) << 1;

//         pos              = JoystickL.Pos();
//         msg.joystickL[0] = pos.x;
//         msg.joystickL[1] = pos.y;

//         pos              = JoystickR.Pos();
//         msg.joystickR[0] = pos.x;
//         msg.joystickR[1] = pos.y;

//         msg.knobs[0] = KnobEncoderL.Count();
//         msg.knobs[1] = KnobEncoderR.Count();

//         mavlink_msg_joystick_air_send_struct(MAVLINK_COMM_0, &msg);
//         vTaskDelay(10);
//     }
// }

// lv_obj_t *ScreenText;

void TestThreadEntry(void *argument)
{
    (void)argument;

    AppMgr_Init();
    AppMgr_LaunchApp(test_app, MainWindow, (char *)"Hello");
    AppMgr_LaunchApp(test_app2, MainWindow, (char *)"Btn2");

    while (true) {
        vTaskDelay(2000);
        MainWindow.switchToApp(test_app);
    }

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&PreviousWakeTime, 20);
    }
}
