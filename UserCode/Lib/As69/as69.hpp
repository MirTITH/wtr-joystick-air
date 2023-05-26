#pragma once

#include "main.h"
#include "usart.h"

class As69
{
private:
    GPIO_TypeDef *const md1_gpio_port_;
    const uint16_t md1_gpio_pin_;

public:
    enum class Mode {
        FullDuplex, // 串口打开，无线打开，全双工传输
        Sleep       // 无线进入休眠状态，可以接收参数配置命令，进行参数配置
    };

    UART_HandleTypeDef *const huart_;

    As69(UART_HandleTypeDef *huart, GPIO_TypeDef *MD1_GPIO_Port, uint16_t MD1_GPIO_Pin)
        : md1_gpio_port_(MD1_GPIO_Port), md1_gpio_pin_(MD1_GPIO_Pin), huart_(huart){};

    bool SetBaudRate(uint32_t new_baud_rate)
    {
        huart_->Init.BaudRate = new_baud_rate;
        return HAL_UART_Init(huart_) == HAL_OK;
    }

    void SetMode(Mode mode)
    {
        switch (mode) {
            case Mode::FullDuplex:
                HAL_GPIO_WritePin(md1_gpio_port_, md1_gpio_pin_, GPIO_PIN_RESET);
                break;
            case Mode::Sleep:
                HAL_GPIO_WritePin(md1_gpio_port_, md1_gpio_pin_, GPIO_PIN_SET);
                break;
            default:
                break;
        }

        HAL_Delay(200);
    }

    Mode GetMode()
    {
        if (HAL_GPIO_ReadPin(md1_gpio_port_, md1_gpio_pin_) == GPIO_PIN_RESET) {
            return Mode::FullDuplex;
        } else {
            return Mode::Sleep;
        }
    }

    bool Read(uint8_t *pData, uint16_t Size, uint32_t Timeout)
    {
        return HAL_UART_Receive(huart_, pData, Size, Timeout) == HAL_OK;
    }

    bool Write(uint8_t *pData, uint16_t Size, uint32_t Timeout)
    {
        return HAL_UART_Transmit(huart_, pData, Size, Timeout) == HAL_OK;
    }
};
