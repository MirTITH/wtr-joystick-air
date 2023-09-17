#pragma once

#include "main.h"

namespace freertos_io
{

namespace port
{

using huart_handle_t = UART_HandleTypeDef;

class uart_port
{
public:
    uart_port(huart_handle_t &huart)
        : huart_(huart){};

    huart_handle_t &huart_;

protected:
    bool IsAddressValidForDma(const void *pData)
    {
        return false;
        // if ((size_t)pData < (0x10000000 + 64 * 1024) && (size_t)pData >= 0x10000000) {
        //     return false;
        // }
        // return true;
    }

    bool UseDmaRx()
    {
        return huart_.hdmarx != nullptr;
    }

    bool UseDmaTx()
    {
        return huart_.hdmatx != nullptr;
    }

    HAL_StatusTypeDef Write(const uint8_t *pData, uint16_t Size, uint32_t Timeout = HAL_MAX_DELAY)
    {
        return HAL_UART_Transmit(&huart_, pData, Size, Timeout);
    }

    HAL_StatusTypeDef Read(uint8_t *pData, uint16_t Size, uint32_t Timeout = HAL_MAX_DELAY)
    {
        return HAL_UART_Receive(&huart_, pData, Size, Timeout);
    }
    HAL_StatusTypeDef WriteDma(const uint8_t *pData, uint16_t Size)
    {
        return HAL_UART_Transmit_DMA(&huart_, (const uint8_t *)pData, Size);
    }

    HAL_StatusTypeDef ReadDma(uint8_t *pData, uint16_t Size)
    {
        return HAL_UART_Receive_DMA(&huart_, pData, Size);
    }

    HAL_StatusTypeDef ReadToIdleDma(uint8_t *pData, uint16_t Size)
    {
        return HAL_UARTEx_ReceiveToIdle_DMA(&huart_, pData, Size);
    }

    HAL_StatusTypeDef ReadToIdleIt(uint8_t *pData, uint16_t Size)
    {
        return HAL_UARTEx_ReceiveToIdle_IT(&huart_, pData, Size);
    }

    HAL_StatusTypeDef WriteIt(const uint8_t *pData, uint16_t Size)
    {
        return HAL_UART_Transmit_IT(&huart_, (const uint8_t *)pData, Size);
    }

    HAL_StatusTypeDef ReadIt(uint8_t *pData, uint16_t Size)
    {
        auto result = HAL_UART_Receive_IT(&huart_, pData, Size);
        return result;
    }
};

} // namespace port

} // namespace freertos_io
