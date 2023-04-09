#pragma once

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "in_handle_mode.h"
#include "mdma.h"
#include "dma.h"

class LcdFmc
{
private:
    volatile uint16_t *_cmd_address;
    volatile uint16_t *_data_address;
    SemaphoreHandle_t _sem = nullptr;
    MDMA_HandleTypeDef *_mdma;
    uint32_t remaining_dma_data_num = 0;
    uint16_t *next_dma_src;
    const uint32_t MAX_DMA_DATA_NUM = 65535; // MDMA 一次最大传输 65535 bytes

public:
    /**
     * @param chip_select FMC 的 Chip Select，应为下列值之一：1,2,3,4
     */
    LcdFmc(int chip_select, MDMA_HandleTypeDef *mdma)
        : _mdma(mdma)
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

    void InitDma()
    {
        // 配置DMA
        extern void LcdFmc_DmaXferCpltCallback(MDMA_HandleTypeDef * _hmdma);
        if (_mdma != nullptr) {
            HAL_MDMA_RegisterCallback(_mdma, HAL_MDMA_XFER_CPLT_CB_ID, LcdFmc_DmaXferCpltCallback);
        }
    }

    void DmaXferCpltCallback()
    {
        if (remaining_dma_data_num > 0) {
            WriteDataDma(next_dma_src, remaining_dma_data_num);
        }
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

    void WriteDataDma(uint16_t *data, uint32_t data_num)
    {
        if (_mdma != nullptr) {
            if (data_num <= MAX_DMA_DATA_NUM / 2) {
                HAL_MDMA_Start_IT(_mdma, (uint32_t)data, (uint32_t)_data_address, data_num * 2, 1);
                remaining_dma_data_num = 0;
            } else {
                HAL_MDMA_Start_IT(_mdma, (uint32_t)data, (uint32_t)_data_address, (MAX_DMA_DATA_NUM / 2) * 2, 1); // (MAX_DMA_DATA_NUM / 2) * 2 处理后可以保证一定是最大的偶数
                remaining_dma_data_num = data_num - MAX_DMA_DATA_NUM / 2;
                next_dma_src           = data + MAX_DMA_DATA_NUM / 2;
            }
        }

        // HAL_DMA_Start_IT(&hdma_memtomem_dma1_stream0, (uint32_t)data, (uint32_t)_data_address, data_num);
    }

    MDMA_HandleTypeDef *GetMdma()
    {
        return _mdma;
    }
};
