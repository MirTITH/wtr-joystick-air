#include "as69.hpp"

bool As69::SetBaudRate(uint32_t new_baud_rate)
{
    if (new_baud_rate == 0) return false;
    huart_->Init.BaudRate = new_baud_rate;
    bool result           = (HAL_UART_Init(huart_) == HAL_OK);
    if (result == true) {
        HAL_Delay(200);
    }

    return result;
}

bool As69::Init()
{
    baudrate_ = huart_->Init.BaudRate;
    SetMode(Mode::Sleep);
    if (ReadConfig() == false) return false;
    baudrate_ = GetModuleBaudRate();
    SetMode(Mode::Working);
    return true;
}

void As69::SetMode(Mode mode)
{
    switch (mode) {
        case Mode::Working:
            SetBaudRate(baudrate_);
            HAL_GPIO_WritePin(md1_gpio_port_, md1_gpio_pin_, GPIO_PIN_RESET);
            HAL_Delay(200);
            break;
        case Mode::Sleep:
            HAL_GPIO_WritePin(md1_gpio_port_, md1_gpio_pin_, GPIO_PIN_SET);
            HAL_Delay(200);
            SetBaudRate(9600);
            break;
        default:
            break;
    }
}

As69::Mode As69::GetMode()
{
    if (HAL_GPIO_ReadPin(md1_gpio_port_, md1_gpio_pin_) == GPIO_PIN_RESET) {
        return Mode::Working;
    } else {
        return Mode::Sleep;
    }
}

bool As69::ReadConfig(uint32_t timeout)
{
    if (GetMode() != Mode::Sleep) {
        return false;
    }

    Write(CMD_READ_CONFIG, sizeof(CMD_READ_CONFIG), HAL_MAX_DELAY);
    return Read((uint8_t *)&config_data_, sizeof(config_data_), timeout);
}

uint32_t As69::GetModuleBaudRate() const
{
    uint8_t data = (config_data_.speed & 0x38) >> 3;
    switch (data) {
        case 0x0:
            return 1200;
            break;
        case 0x1:
            return 2400;
            break;
        case 0x2:
            return 4800;
            break;
        case 0x3:
            return 9600;
            break;
        case 0x4:
            return 19200;
            break;
        case 0x5:
            return 38400;
            break;
        case 0x6:
            return 57600;
            break;
        case 0x7:
            return 115200;
            break;
        default:
            return 0;
            break;
    }
}

As69::Parity As69::GetModuleParity() const
{
    uint8_t data = (config_data_.speed & 0xC0) >> 6;
    switch (data) {
        case 0x0:
            return Parity::None;
            break;
        case 0x1:
            return Parity::Odd;
            break;
        case 0x2:
            return Parity::Even;
            break;
        case 0x3:
            return Parity::None;
            break;
        default:
            return Parity::None;
            break;
    }
}
