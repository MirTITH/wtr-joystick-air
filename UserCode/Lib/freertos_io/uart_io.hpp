#pragma once

#include "uart_port.hpp"
#include "FreeRTOS.h"
#include "freertos_lock/freertos_lock.hpp"
#include <string>
#include <mutex>

namespace freertos_io
{
class Uart : public port::uart_port
{
private:
    freertos_lock::BinarySemphr write_lock_{true};
    freertos_lock::BinarySemphr read_lock_{true};
    using lk_guard             = std::lock_guard<freertos_lock::BinarySemphr>;
    uint16_t read_to_idle_size = 0;

public:
    using uart_port::uart_port;
    uint32_t total_tx_size_ = 0;

    /**
     * @brief 发送完成回调
     *
     * @note 在 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) 中调用
     *
     */
    void TxCpltCallback()
    {
        write_lock_.unlock();
    }

    /**
     * @brief 接收完成回调
     *
     * @note 在 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 中调用
     *
     */
    void RxCpltCallback()
    {
        read_lock_.unlock();
    }

    /**
     * @brief 接收事件回调，目前用于空闲中断事件
     *
     * @note 在 void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) 中调用
     *
     * @param size
     */
    void RxEventCallback(uint16_t size)
    {
        read_to_idle_size = size;
        read_lock_.unlock();
    }

    auto Write(const std::string &str, uint32_t Timeout = HAL_MAX_DELAY)
    {
        total_tx_size_ += str.size();
        lk_guard lock(write_lock_);
        return uart_port::Write((const uint8_t *)str.data(), str.size(), Timeout);
    }

    template <typename T>
    auto Write(const T *pData, uint16_t Size, uint32_t Timeout = HAL_MAX_DELAY)
    {
        total_tx_size_ += Size;
        lk_guard lock(write_lock_);
        return uart_port::Write((const uint8_t *)pData, Size, Timeout);
    }

    /**
     * @brief 不使用锁保护的写入
     *
     * @tparam T
     * @param pData 要写入的数据
     * @param Size 数据大小 (byte)
     * @param Timeout 超时时长 (ms)
     * @return auto
     */
    template <typename T>
    auto WriteDirectly(const T *pData, uint16_t Size, uint32_t Timeout = HAL_MAX_DELAY)
    {
        total_tx_size_ += Size;
        return uart_port::Write((const uint8_t *)pData, Size, Timeout);
    }

    template <typename T>
    auto Read(T *pData, uint16_t Size, uint32_t Timeout = HAL_MAX_DELAY)
    {
        lk_guard lock(read_lock_);
        return uart_port::Read((uint8_t *)pData, Size, Timeout);
    }

    template <typename T>
    auto WriteNonBlock(const T *pData, uint16_t Size)
    {
        if (Size == 0) {
            return HAL_OK;
        }

        total_tx_size_ += Size;
        write_lock_.lock();

        if (UseDmaTx() && IsAddressValidForDma(pData)) {
            return uart_port::WriteDma((const uint8_t *)pData, Size);
        }

        return uart_port::WriteIt((const uint8_t *)pData, Size);
    }

    auto WriteNonBlock(const std::string &str)
    {
        return WriteNonBlock(str.data(), str.size());
    }

    template <typename T>
    auto ReadNonBlock(T *pData, uint16_t Size)
    {
        if (Size == 0) {
            return HAL_OK;
        }

        read_lock_.lock();

        if (UseDmaRx() && IsAddressValidForDma(pData)) {
            return uart_port::ReadDma((uint8_t *)pData, Size);
        }

        return uart_port::ReadIt((uint8_t *)pData, Size);
    }

    template <typename T>
    size_t ReadToIdle(T *pData, uint16_t Size)
    {
        if (Size == 0) {
            return HAL_OK;
        }

        read_lock_.lock();

        HAL_StatusTypeDef result;

        if (UseDmaRx() && IsAddressValidForDma(pData)) {
            result = uart_port::ReadToIdleDma((uint8_t *)pData, Size);
        } else {
            result = uart_port::ReadToIdleIt((uint8_t *)pData, Size);
        }

        if (result != HAL_OK) {
            return 0;
        }

        {
            lk_guard lock(read_lock_);
            return read_to_idle_size;
        }
    }

    void WaitForWriteCplt()
    {
        lk_guard lock(write_lock_);
    }

    void WaitForReadCplt()
    {
        lk_guard lock(read_lock_);
    }
};

} // namespace freertos_io
