#include "Adc/adc_class.hpp"
#include "adc_class.hpp"

void Adc::Init()
{
    // 写这么长是为了自动推导 data_ 类型
    data_ = new std::remove_reference<decltype(*data_)>::type[hadc_->Init.NbrOfConversion];

    // 校准 ADC （不同 stm32 型号该函数可能不一样或不存在，注意修改）
    HAL_ADCEx_Calibration_Start(hadc_, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED); // For STM32H7
}

std::remove_reference<decltype(*Adc::data_)>::type Adc::GetData(size_t index) const
{
    if (index < hadc_->Init.NbrOfConversion) {
        SCB_InvalidateDCache_by_Addr(data_, sizeof(data_));
        return data_[index];
    } else {
        return 0;
    }
}

std::vector<std::remove_reference<decltype(*Adc::data_)>::type> Adc::GetAllData() const
{
    std::vector<std::remove_reference<decltype(*data_)>::type> result(hadc_->Init.NbrOfConversion);
    SCB_InvalidateDCache_by_Addr(data_, sizeof(data_));
    for (size_t i = 0; i < hadc_->Init.NbrOfConversion; i++) {
        result.at(i) = data_[i];
    }
    return result;
}

float Adc::GetVoltage(size_t index) const
{
    if (index < hadc_->Init.NbrOfConversion) {
        // 转换到 16 bit 对应的值，再乘以电压系数
        return __LL_ADC_CONVERT_DATA_RESOLUTION(GetData(index), hadc_->Init.Resolution, LL_ADC_RESOLUTION_16B) * (vref_ / 0xffff);
    } else {
        return 0;
    }
}

std::vector<float> Adc::GetAllVoltage() const
{
    auto adc_data = GetAllData();
    std::vector<float> result(hadc_->Init.NbrOfConversion);
    for (size_t i = 0; i < hadc_->Init.NbrOfConversion; i++) {
        result.at(i) = __LL_ADC_CONVERT_DATA_RESOLUTION(adc_data.at(i), hadc_->Init.Resolution, LL_ADC_RESOLUTION_16B) * (vref_ / 0xffff);
    }
    return result;
}

float Adc::GetTemperature(size_t temperature_sensor_index)
{
    /**
     * Calculation formula:
     *     Temperature = ((TS_ADC_DATA - TS_CAL1)
     *                     * (TS_CAL2_TEMP - TS_CAL1_TEMP))
     *                   / (TS_CAL2 - TS_CAL1) + TS_CAL1_TEMP
     *     with TS_ADC_DATA = temperature sensor raw data measured by ADC
     *          Avg_Slope = (TS_CAL2 - TS_CAL1)
     *                      / (TS_CAL2_TEMP - TS_CAL1_TEMP)
     *          TS_CAL1   = equivalent TS_ADC_DATA at temperature
     *                      TEMP_DEGC_CAL1 (calibrated in factory)
     *          TS_CAL2   = equivalent TS_ADC_DATA at temperature
     *                      TEMP_DEGC_CAL2 (calibrated in factory)
     */
    auto adcValue16Bit = __LL_ADC_CONVERT_DATA_RESOLUTION(GetData(temperature_sensor_index), hadc_->Init.Resolution, LL_ADC_RESOLUTION_16B);
    return (adcValue16Bit * (vref_ * 1000.0f / TEMPSENSOR_CAL_VREFANALOG) - (*TEMPSENSOR_CAL1_ADDR)) *
               ((float)(TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP) / (*TEMPSENSOR_CAL2_ADDR - *TEMPSENSOR_CAL1_ADDR)) +
           TEMPSENSOR_CAL1_TEMP;
}
