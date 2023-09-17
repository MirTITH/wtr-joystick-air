#include "async_uart.hpp"
#include "thread_priority_def.h"
#include "in_handle_mode.h"

void UartDaemonEntry(void *argument)
{
    auto this_class       = (AsyncUart *)argument;
    auto &uart_device     = this_class->uart_device;
    auto &ring_buffer_    = this_class->ring_buffer_;
    auto &task_to_notify_ = this_class->task_to_notify_;

    size_t continous_read_size, size;

    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        taskENTER_CRITICAL();
        continous_read_size = ring_buffer_.get_continous_read_size(); // 可以一次连续写入串口的数据大小
        size                = ring_buffer_.size();                    // 总写入串口的数据大小
        taskEXIT_CRITICAL();

        if (size == continous_read_size) {
            // 可以一次连续写入，所以一次写入
            uart_device.WriteNonBlock(ring_buffer_.get_read_address(), continous_read_size);
        } else {
            // 不可以一次连续写入，需要分两次写
            uart_device.WriteNonBlock(ring_buffer_.get_read_address(), continous_read_size);      // 第一部分
            uart_device.WriteNonBlock(ring_buffer_.get_buffer_ptr(), size - continous_read_size); // 第二部分（折返到缓冲区开头）
        }

        // 等待写入完成
        uart_device.WaitForWriteCplt();

        taskENTER_CRITICAL();
        // 从缓冲区中删掉已写入的数据
        ring_buffer_.remove_front(size);
        taskEXIT_CRITICAL();

        if (task_to_notify_ != nullptr) {
            auto temp       = task_to_notify_;
            task_to_notify_ = nullptr;
            xTaskNotifyGive(temp);
        }
    }
}

AsyncUart::AsyncUart(freertos_io::Uart &uart, const char *thread_name)
    : uart_device(uart)
{
    // 优先级高于或等于 AsyncUart 的线程，由于在调用 Write 时不会发生线程切换（除非缓冲区不够），可以获得最快的 Write 速度
    xTaskCreate(UartDaemonEntry, thread_name, 256, this, PriorityBelowNormal, &task_handle_);
}

void AsyncUart::Write(const char *data, size_t size)
{
    if (InHandlerMode()) {
        WriteInIsr(data, size);
    } else {
        // 上锁，防止其他线程操作
        lock_.lock_from_thread();
        WriteInThreadWithoutLock(data, size);
        lock_.unlock_from_thread();
    }
}

void AsyncUart::WriteInThreadWithoutLock(const char *data, size_t size)
{
    // 在线程中。如果缓冲区剩余大小不足 size，则分多次传输
    size_t written_size; // 已写入缓冲区的数据大小
    while (true) {
        taskENTER_CRITICAL();
        written_size = ring_buffer_.push_data_back((const uint8_t *)data, size); // 写入缓冲区
        taskEXIT_CRITICAL();

        if (written_size < size) {
            size -= written_size; // 剩余传输大小
            data += written_size;

            task_to_notify_ = xTaskGetCurrentTaskHandle(); // 告诉守护线程，传输完成后要通知当前线程
            xTaskNotifyGive(task_handle_);                 // 通知守护线程开始传输

            // 等待守护线程通知传输完成
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        } else {
            // 全部 data 已写入缓冲区
            xTaskNotifyGive(task_handle_); // 通知守护线程开始传输
            break;                         // 退出循环。由于所有数据都已在缓冲区，不用等待
        }
    }
}

void AsyncUart::WriteInIsr(const char *data, size_t size)
{
    // 在中断中。如果缓冲区剩余空间不足 size，舍弃多出来的 data
    UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    ring_buffer_.push_data_back((const uint8_t *)data, size);
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);

    // 通知守护线程传输
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(task_handle_, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
