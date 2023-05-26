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
#include "LvglApp/app_mgr.hpp"
#include "test_app.hpp"
#include "App/dashboard.hpp"
#include "Joystick/joystick_define.hpp"
#include "Encoder/encoder_define.hpp"
#include "as69_config.hpp"
#include "TouchScreen/GT911/gt911_define.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

DashboardMgr dashboard_mgr("dashboard_mgr");

static TickType_t last_3_points_time = 0;

void TestThreadEntry(void *argument)
{
    (void)argument;

    AppMgr_Init();
    AppMgr_LaunchApp(InternalAs69Config, MainWindow, nullptr);
    AppMgr_LaunchApp(dashboard_mgr, MainWindow, nullptr);
    auto time_dashboard      = dashboard_mgr.NewDashboard(0, "Time");
    auto encoder_dashboard   = dashboard_mgr.NewDashboard(1, "Encoder");
    auto joystickl_dashboard = dashboard_mgr.NewDashboard(2, "JoystickL");
    auto joystickr_dashboard = dashboard_mgr.NewDashboard(3, "JoystickR");

    stringstream sstr;
    sstr.precision(4);
    sstr.setf(std::ios::fixed);

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        time_dashboard->SetMsg(xTaskGetTickCount() / 1000.0);
        sstr.str("");
        sstr << JoystickL.Pos().x << "," << JoystickL.Pos().y;
        joystickl_dashboard->SetMsg(sstr.str());
        sstr.str("");
        sstr << JoystickR.Pos().x << "," << JoystickR.Pos().y;
        joystickr_dashboard->SetMsg(sstr.str());
        sstr.str("");
        sstr << KnobEncoderR.Count() << "," << KnobEncoderL.Count();
        encoder_dashboard->SetMsg(sstr.str());

        if (TouchScreen.NumberOfTouchPoint() == 3) {
            if (xTaskGetTickCount() - last_3_points_time > 500) {
                MainWindow.SwitchToPage(0);
            }
            last_3_points_time = xTaskGetTickCount();
        }

        vTaskDelayUntil(&PreviousWakeTime, 50);
    }
}
