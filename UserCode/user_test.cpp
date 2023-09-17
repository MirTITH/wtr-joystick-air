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
#include "Button/buttons.h"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

// static TickType_t last_3_points_time = 0;

static void a_receive_callback(uint8_t type)
{
    switch (type) {
        case MPU9250_INTERRUPT_MOTION: {
            mpu9250_interface_debug_print("mpu9250: irq motion.\n");

            break;
        }
        case MPU9250_INTERRUPT_FIFO_OVERFLOW: {
            mpu9250_interface_debug_print("mpu9250: irq fifo overflow.\n");

            break;
        }
        case MPU9250_INTERRUPT_FSYNC_INT: {
            mpu9250_interface_debug_print("mpu9250: irq fsync int.\n");

            break;
        }
        case MPU9250_INTERRUPT_DMP: {
            mpu9250_interface_debug_print("mpu9250: irq dmp\n");

            break;
        }
        case MPU9250_INTERRUPT_DATA_READY: {
            mpu9250_interface_debug_print("mpu9250: irq data ready\n");

            break;
        }
        default: {
            mpu9250_interface_debug_print("mpu9250: irq unknown code.\n");

            break;
        }
    }
}

void TestThreadEntry(void *argument)
{
    (void)argument;

    // auto joystickl_dashboard = dashboard_mgr.NewDashboard(250, "JoystickL");
    // auto joystickr_dashboard = dashboard_mgr.NewDashboard(251, "JoystickR");
    // auto encoder_dashboard   = dashboard_mgr.NewDashboard(252, "Encoder");
    // auto button_dashboard    = dashboard_mgr.NewDashboard(253, "Buttons(0-10 bit)");
    // auto button2_dashboard   = dashboard_mgr.NewDashboard(254, "Buttons(11-21 bit)");
    // auto mpu_result_dashboard = dashboard_mgr.NewDashboard(100, "MPU9250 result");
    // auto mpu_g_dashboard      = dashboard_mgr.NewDashboard(101, "MPU9250 g");
    // auto mpu_dps_dashboard    = dashboard_mgr.NewDashboard(102, "MPU9250 dps");
    // auto mpu_ut_dashboard     = dashboard_mgr.NewDashboard(103, "MPU9250 ut");
    // auto mpu_temp_dashboard   = dashboard_mgr.NewDashboard(104, "MPU9250 temp");
    // auto mpu_pitch_dashboard = dashboard_mgr.NewDashboard(105, "MPU9250 pitch");
    // auto mpu_roll_dashboard  = dashboard_mgr.NewDashboard(106, "MPU9250 roll");
    // auto mpu_yaw_dashboard   = dashboard_mgr.NewDashboard(107, "MPU9250 yaw");

    stringstream sstr;
    sstr.precision(2);
    sstr.setf(std::ios::fixed);


    uint32_t PreviousWakeTime = xTaskGetTickCount();

    // extern volatile uint32_t MavTotalBytesSent;
    // extern volatile uint32_t MavTotalBytesGot;

    // float g[3]{};
    // float dps[3]{};
    // float ut[3]{};
    // float temperature = 0;

    while (true) {
        // time_dashboard->SetMsg(xTaskGetTickCount() / 1000.0);
        // Buttons_Scan();

        // mpu_pitch_dashboard->SetMsgValue(gs_pitch[0]);
        // mpu_roll_dashboard->SetMsgValue(gs_roll[0]);
        // mpu_yaw_dashboard->SetMsgValue(gs_yaw[0]);

        // sstr.str("");
        // sstr << gs_accel_g[0] << "," << gs_accel_g[1] << "," << gs_accel_g[2];
        // mpu_g_dashboard->SetMsg(sstr.str());

        // sstr.str("");
        // sstr << dps[0] << "," << dps[1] << "," << dps[2];
        // mpu_dps_dashboard->SetMsg(sstr.str());

        // sstr.str("");
        // sstr << ut[0] << "," << ut[1] << "," << ut[2];
        // mpu_ut_dashboard->SetMsg(sstr.str());

        // mpu9250_basic_read_temperature(&temperature);
        // mpu_temp_dashboard->SetMsgValue(temperature);

        // sstr << JoystickL.Pos().x << "," << JoystickL.Pos().y;
        // joystickl_dashboard->SetMsg(sstr.str());
        // sstr.str("");
        // sstr << JoystickR.Pos().x << "," << JoystickR.Pos().y;
        // joystickr_dashboard->SetMsg(sstr.str());
        // sstr.str("");
        // sstr << KnobEncoderL.Count() << "," << KnobEncoderR.Count();
        // encoder_dashboard->SetMsg(sstr.str());

        // sstr.str("");
        // for (size_t i = 1; i <= 11; i++) {
        //     sstr << (int)Buttons_Read(i);
        // }
        // button_dashboard->SetMsg(sstr.str());

        // sstr.str("");
        // for (size_t i = 12; i <= 22; i++) {
        //     sstr << (int)Buttons_Read(i);
        // }
        // button2_dashboard->SetMsg(sstr.str());

        // if (TouchScreen.NumberOfTouchPoint() == 3) {
        //     if (xTaskGetTickCount() - last_3_points_time > 500) {
        //         MainWindow.SwitchToPage(0);
        //     }
        //     last_3_points_time = xTaskGetTickCount();
        // }

        vTaskDelayUntil(&PreviousWakeTime, 100);
    }
}
