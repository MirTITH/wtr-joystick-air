#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "ring_buffer.hpp"
#include "uart_io.hpp"
#include "freertos_lock/freertos_lock.hpp"

class AsyncUart
{
private:
    TaskHandle_t task_handle_;
    TaskHandle_t task_to_notify_ = nullptr;
    RingBuffer<512> ring_buffer_; // 缓冲区，尖括号里的是缓冲区大小

public:
    freertos_io::Uart &uart_device;
    freertos_lock::BinarySemphr lock_{true};

    AsyncUart(freertos_io::Uart &uart, const char *thread_name);

    void Write(const char *data, size_t size);
    void Write(const std::string &str)
    {
        Write(str.data(), str.size());
    }
    void WriteInThreadWithoutLock(const char *data, size_t size);
    void WriteInIsr(const char *data, size_t size);

    template <typename T>
    size_t ReadToIdle(T *pData, uint16_t Size)
    {
        return uart_device.ReadToIdle(pData, Size);
    }

    friend void UartDaemonEntry(void *argument);
};
