#include "buttons.h"
#include "main.h"
#include "matrix_keyboard.h"

static uint16_t button_matrix_;

static uint8_t ButtonGroupA_Read(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;

    switch (btn->id) {
        case Btn_KnobL:
            return HAL_GPIO_ReadPin(KnobL_Btn_GPIO_Port, KnobL_Btn_Pin);
            break;
        case Btn_KnobR:
            return HAL_GPIO_ReadPin(KnobR_Btn_GPIO_Port, KnobR_Btn_Pin);
            break;
        case Btn_JoystickL:
            return HAL_GPIO_ReadPin(JoystickL_Btn_GPIO_Port, JoystickL_Btn_Pin);
            break;
        case Btn_JoystickR:
            return HAL_GPIO_ReadPin(JoystickR_Btn_GPIO_Port, JoystickR_Btn_Pin);
            break;

        default:
            return 1;
            break;
    }
}

static void ScanButtonMatrix()
{
    button_matrix_ = MatrixKeyboard_Scan();
}

static uint8_t ReadButtonMatrix(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    return (button_matrix_ & (1 << (btn->id - 1))) != 0;
}

__weak void common_btn_evt_cb(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    (void)btn;
}

void Buttons_Init()
{
    // 矩阵键盘
    static flex_button_t buttonMatrix[16];
    for (int i = 0; i < 16; i++) {
        buttonMatrix[i].id                     = i + 1;
        buttonMatrix[i].usr_button_read        = ReadButtonMatrix;
        buttonMatrix[i].cb                     = common_btn_evt_cb;
        buttonMatrix[i].pressed_logic_level    = 1;
        buttonMatrix[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(500);
        buttonMatrix[i].long_press_start_tick  = FLEX_MS_TO_SCAN_CNT(1000);
        buttonMatrix[i].long_hold_start_tick   = FLEX_MS_TO_SCAN_CNT(2000);
        flex_button_register(&buttonMatrix[i]);
    }

    // 旋钮和摇杆的键
    // [17] 左旋钮，[18] 右旋钮，[19] 左摇杆，[20]，右摇杆
    static flex_button_t buttonGroupA[4];

    for (int i = 0; i < 4; i++) {
        buttonGroupA[i].id                     = i + 17;
        buttonGroupA[i].usr_button_read        = ButtonGroupA_Read;
        buttonGroupA[i].cb                     = common_btn_evt_cb;
        buttonGroupA[i].pressed_logic_level    = 0;
        buttonGroupA[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(500);
        buttonGroupA[i].long_press_start_tick  = FLEX_MS_TO_SCAN_CNT(1000);
        buttonGroupA[i].long_hold_start_tick   = FLEX_MS_TO_SCAN_CNT(2000);
        flex_button_register(&buttonGroupA[i]);
    }
}

void Buttons_Scan()
{
    ScanButtonMatrix();
    flex_button_scan();
}
