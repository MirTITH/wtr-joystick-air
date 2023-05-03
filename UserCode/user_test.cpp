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

uint16_t Adc3Data[4] = {1, 2, 3, 4};

uint32_t AdcCpltCount = 0;
uint32_t AdcIntCount  = 0;

void TestThreadEntry(void *argument)
{
    (void)argument;

    HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);

    HAL_ADC_Start_DMA(&hadc3, (uint32_t *)Adc3Data, 4);

    LvglLock();

    auto text = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(text, lv_pct(100), lv_pct(100));

    LvglUnlock();

    double adc_voltage[4];

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        for (int i = 0; i < 4; i++) {
            adc_voltage[i] = (double)Adc3Data[i] / (1 << 16) * 3.3;
        }
        stringstream sstr;
        sstr << adc_voltage[0] * 11.5 / 1.5 << " " << adc_voltage[1] << " " << adc_voltage[2] << " " << adc_voltage[3];
        sstr << " " << AdcCpltCount << " " << AdcIntCount << endl;
        LvglLock();
        lv_textarea_add_text(text, sstr.str().c_str());
        LvglUnlock();
        vTaskDelayUntil(&PreviousWakeTime, 500);
    }
}