#pragma once

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "in_handle_mode.h"

class LcdFmc
{
private:
    volatile uint16_t *_cmd_address;
    volatile uint16_t *_data_address;
    SemaphoreHandle_t _sem = nullptr;

public:
    /**
     * @param chip_select FMC 的 Chip Select，应为下列值之一：1,2,3,4
     */
    LcdFmc(int chip_select)
    {
        uint32_t lcd_base;
        switch (chip_select) {
            case 1:
                lcd_base = 0x60000000;
                break;
            case 2:
                lcd_base = 0x64000000;
                break;
            case 3:
                lcd_base = 0x68000000;
                break;
            case 4:
                lcd_base = 0x6C000000;
                break;

            default:
                while (1) {}
                break;
        }

        _cmd_address  = (uint16_t *)lcd_base;
        _data_address = (uint16_t *)(lcd_base + 0x3FFFFFE); // 将所有地址为都置1，这样无论 LCD Register Select 设为何值都有效
        _sem          = xSemaphoreCreateBinary();
        xSemaphoreGive(_sem);
    }

    ~LcdFmc()
    {
        vSemaphoreDelete(_sem);
    }

    void Lock(TickType_t tick = portMAX_DELAY)
    {
        if (InHandlerMode()) {
            xSemaphoreTakeFromISR(_sem, nullptr);
        } else {
            xSemaphoreTake(_sem, tick);
        };
    }

    void UnLock()
    {
        if (InHandlerMode()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(_sem, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        } else {
            xSemaphoreGive(_sem);
        }
    }

    uint16_t ReadData()
    {
        return *_data_address;
    }

    void WriteCmd(uint16_t cmd)
    {
        *_cmd_address = cmd;
    }

    void WriteData(uint16_t data)
    {
        *_data_address = data;
    }
};
