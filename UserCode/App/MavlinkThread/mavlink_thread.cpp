#include "mavlink_thread.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Lib/Mavlink/wtr_mavlink.h"
#include "main.h"
#include "Button/buttons.h"
#include "Button/matrix_keyboard.h"
#include "Joystick/joystick_define.hpp"
#include "Encoder/encoder_define.hpp"
#include "As69/as69_device.hpp"

static mavlink_joystick_air_t msg = {};
static SemaphoreHandle_t mutex_;
static volatile bool run_state_ = false;

void MavlinkThreadEntry(void *argument)
{
    (void)argument;

    Joystick::Point_t pos;

    while (run_state_ == true) {
        for (size_t i = 0; i < 20; i++) {
            msg.buttons[i] = Buttons_Read(i + 1);
        }

        msg.switchs = 0;
        msg.switchs |= Buttons_Read(Switch_L);
        msg.switchs |= Buttons_Read(Switch_R) << 1;

        pos              = JoystickL.Pos();
        msg.joystickL[0] = pos.x;
        msg.joystickL[1] = pos.y;

        pos              = JoystickR.Pos();
        msg.joystickR[0] = pos.x;
        msg.joystickR[1] = pos.y;

        msg.knobs[0] = KnobEncoderL.Count();
        msg.knobs[1] = KnobEncoderR.Count();

        mavlink_msg_joystick_air_send_struct(MAVLINK_COMM_0, &msg);
        vTaskDelay(20);
    }

    vTaskDelete(nullptr);
}

void InitMavlinkThread()
{
    mutex_ = xSemaphoreCreateMutex();
    wtrMavlink_BindChannel(&huart1, MAVLINK_COMM_0);
}

void StartMavlinkThread()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);
    if (run_state_ == false) {
        run_state_ = true;
        xTaskCreate(MavlinkThreadEntry, "MavlinkThread", 1024, nullptr, 3, nullptr);
    }
    xSemaphoreGive(mutex_);
}

void StopMavlinkThread()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);
    run_state_ = false;
    xSemaphoreGive(mutex_);
}
