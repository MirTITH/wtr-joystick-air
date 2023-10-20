#include "user_test.hpp"
#include "FreeRTOS.h"
#include "task.h"
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
#include "freertos_lock/freertos_lock.hpp"
#include "mpumpl/mpumpl.hpp"

using namespace std;

void TestThreadEntry(void *argument)
{
    (void)argument;

    // auto joystickl_dashboard = dashboard_mgr.NewDashboard(250, "JoystickL");

    stringstream sstr;
    sstr.precision(2);
    sstr.setf(std::ios::fixed);

    printf("Start MPU9250_MPL_Init\n");
    int result = Mpu_Init();
    printf("Result: %d\n", result);
    // vTaskDelay(1000);

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        auto quat = Mpu_GetQuat();
        printf("%f,%f,%f,%f\n", quat[0], quat[1], quat[2], quat[3]);

        vTaskDelayUntil(&PreviousWakeTime, 25);
    }
}
