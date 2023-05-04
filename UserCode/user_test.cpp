#include "user_test.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "driver_mpu9250_basic.h"
#include "adc.h"
#include "lvgl_thread.h"
#include "lvgl.h"
#include <sstream>
#include "Adc/adc_class_define.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

using namespace std;

uint16_t Adc1Data[2];
uint16_t Adc2Data[2];
uint16_t Adc3Data[4];

void TestThreadEntry(void *argument)
{
    (void)argument;

    // HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
    // HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
    // HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);

    // HAL_ADC_Start_DMA(&hadc1, (uint32_t *)Adc1Data, 2);
    // HAL_ADC_Start_DMA(&hadc2, (uint32_t *)Adc2Data, 2);
    // HAL_ADC_Start_DMA(&hadc3, (uint32_t *)Adc3Data, 4);

    Adc1.Init();
    Adc1.StartDma();
    Adc2.Init();
    Adc2.StartDma();
    Adc3.Init();
    Adc3.StartDma();

    LvglLock();

    auto text = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(text, lv_pct(100), lv_pct(100));

    // uint32_t lastAdc1CpltCount = Adc1CpltCount;

    LvglUnlock();

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        stringstream sstr;
        sstr.precision(6);
        sstr.setf(std::ios::fixed);
        for (auto &volt : Adc1.GetAllVoltage()) {
            sstr << volt << " ";
        }
        sstr << endl;
        for (auto &volt : Adc2.GetAllVoltage()) {
            sstr << volt << " ";
        }
        sstr << endl;
        for (auto &volt : Adc3.GetAllVoltage()) {
            sstr << volt << " ";
        }
        sstr << endl;

        LvglLock();
        lv_textarea_set_text(text, sstr.str().c_str());
        LvglUnlock();
        vTaskDelayUntil(&PreviousWakeTime, 100);
    }
}