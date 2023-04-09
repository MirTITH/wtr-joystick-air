#pragma once

#include "tim.h"

class LcdBacklight
{
private:
    TIM_HandleTypeDef *_htim;
    uint32_t _channel;
    uint32_t _arr;

    void readArr()
    {
        _arr = __HAL_TIM_GET_AUTORELOAD(_htim);
    }

public:
    LcdBacklight(TIM_HandleTypeDef *htim, uint32_t channel)
        : _htim(htim), _channel(channel)
    {
        if (_htim != nullptr) {
            readArr();
            HAL_TIM_PWM_Start(_htim, _channel);
        }
    }

    ~LcdBacklight()
    {
        if (_htim != nullptr) {
            HAL_TIM_PWM_Stop(_htim, _channel);
        }
    }

    /**
     * @brief 设置背光强度
     *
     * @param lightness [0,1]，0 为全黑，1为最亮
     */
    void SetBacklight(float lightness)
    {
        lightness = (lightness < 0) ? 0 : lightness;
        lightness = (lightness > 1) ? 1 : lightness;

        readArr();
        __HAL_TIM_SET_COMPARE(_htim, _channel, _arr * lightness);
    }

    void BacklightOnOff(bool is_on)
    {
        if (is_on) {
            HAL_TIM_PWM_Start(_htim, _channel);
        } else {
            HAL_TIM_PWM_Stop(_htim, _channel);
        }
    }
};
