#pragma once

#include "adc.h"
#include <vector>

class Adc
{
private:
    ADC_HandleTypeDef *hadc_;
    uint16_t *buffer_    = nullptr;
    size_t buffer_length = 0;

public:
    Adc(ADC_HandleTypeDef *hadc)
        : hadc_(hadc){};

    void Init()
    {
        buffer_length = 3 * hadc_->Init.NbrOfConversion; // 使用三重缓冲
        // 写这么长是为了自动推导 buffer_ 类型
        buffer_ = new std::remove_reference<decltype(*buffer_)>::type[buffer_length];

        // 校准 ADC （不同 stm32 型号该函数可能不一样或不存在，注意修改）
        HAL_ADCEx_Calibration_Start(hadc_, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED); // For STM32H7
    }

    void StartDma()
    {
        HAL_ADC_Start_DMA(hadc_, (uint32_t *)buffer_, buffer_length);
    }

    void StopDma()
    {
        HAL_ADC_Stop_DMA(hadc_);
    }

    void ConvCpltCallback()
    {
        
    }

    ~Adc()
    {
        if (buffer_ != nullptr) {
            delete[] buffer_;
        }
    };
};
