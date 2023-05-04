#include "joystick_define.hpp"
#include "Adc/adc_class_define.hpp"

Joystick JoystickL(&Adc1, 0, 1, true, true);
Joystick JoystickR(&Adc2, 0, 1, true, true);
