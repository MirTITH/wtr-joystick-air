#include "joystick_define.hpp"
#include "Adc/adc_class_define.hpp"

Joystick JoystickL(&Adc1, 0, 1, true, true);
Joystick JoystickR(&Adc2, 0, 1, true, true);

void Joystick_Init()
{
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
}
