#pragma once
#include "flexible_button.h"
#define Btn_LeftCrossUp     2
#define Btn_LeftCrossDown   1
#define Btn_LeftCrossLeft   13
#define Btn_LeftCrossRight  5
#define Btn_LeftCrossMid    9
#define Btn_RightCrossUp    4
#define Btn_RightCrossDown  3
#define Btn_RightCrossLeft  7
#define Btn_RightCrossRight 15
#define Btn_RightCrossMid   11
#define Btn_Btn0            6
#define Btn_Btn1            10
#define Btn_Btn2            14
#define Btn_Btn3            8
#define Btn_Btn4            12
#define Btn_Btn5            16
#define Btn_KnobL           17
#define Btn_KnobR           18
#define Btn_JoystickL       19
#define Btn_JoystickR       20

#ifdef __cplusplus
extern "C" {
#endif

void Buttons_Init();
void Buttons_Scan();
uint8_t Buttons_Read(uint8_t Btn);

#ifdef __cplusplus
}
#endif
