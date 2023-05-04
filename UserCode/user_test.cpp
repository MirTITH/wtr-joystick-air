#include "user_test.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "driver_mpu9250_basic.h"
#include "adc.h"
#include "lvgl_thread.h"
#include "lvgl.h"
#include <sstream>

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

uint16_t Adc1Data[2];
uint16_t Adc2Data[2];
uint16_t Adc3Data[12];

uint32_t Adc1CpltCount = 0;
uint32_t Adc2CpltCount = 0;
uint32_t Adc3CpltCount = 0;
uint32_t Adc3IRQCount  = 0;

void TestThreadEntry(void *argument)
{
    (void)argument;

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)Adc1Data, 2);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)Adc2Data, 2);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *)Adc3Data, 12);

    LvglLock();

    auto text = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(text, lv_pct(100), lv_pct(100));

    // uint32_t lastAdc1CpltCount = Adc1CpltCount;

    LvglUnlock();

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        SCB_InvalidateDCache_by_Addr(Adc1Data, sizeof(Adc1Data));
        SCB_InvalidateDCache_by_Addr(Adc2Data, sizeof(Adc2Data));
        SCB_InvalidateDCache_by_Addr(Adc3Data, sizeof(Adc3Data));
        stringstream sstr;
        sstr.precision(6);
        sstr.setf(std::ios::fixed);
        for (size_t i = 0; i < 12; i++) {
            sstr << (double)Adc3Data[i] * 11.5 / 1.5 * 3.3 / (1 << 16) << " ";
        }

        sstr << endl;

        // for (int i = 0; i < 2; i++) {
        //     sstr << (double)Adc1Data[i] * 3.3 / (1 << 16) << " ";
        // }
        // for (int i = 0; i < 2; i++) {
        //     sstr << (double)Adc2Data[i] * 3.3 / (1 << 16) << " ";
        // }

        sstr << Adc3CpltCount << " " << Adc3IRQCount << endl;
        LvglLock();
        lv_textarea_set_text(text, sstr.str().c_str());
        LvglUnlock();
        vTaskDelayUntil(&PreviousWakeTime, 100);
    }
}