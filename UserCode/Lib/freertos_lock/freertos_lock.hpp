#pragma once

#include "FreeRTOS.h"
#include "semphr.h"
#include "in_handle_mode.h"

namespace freertos_lock
{
namespace freertos_lock_internal
{
class FreeRtosSemBase
{
protected:
    StaticSemaphore_t sem_buffer_;
    SemaphoreHandle_t sem_ = nullptr;

public:
    /**
     * @return 如果信号量是计数信号量，则返回信号量的当前计数值。如果信号量是二进制信号量，则当信号量可用时，返回 1，当信号量不可用时，返回 0
     */
    UBaseType_t get_count()
    {
        return uxSemaphoreGetCount(sem_);
    }

    bool lock(TickType_t xTicksToWait = portMAX_DELAY)
    {
        if (InHandlerMode()) {
            return lock_from_isr();
        } else {
            return lock_from_thread(xTicksToWait);
        }
    }

    bool unlock()
    {
        if (InHandlerMode()) {
            return unlock_from_isr();
        } else {
            return unlock_from_thread();
        }
    }

    bool lock_from_thread(TickType_t xTicksToWait = portMAX_DELAY)
    {
        if (xSemaphoreTake(sem_, xTicksToWait) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    }

    bool lock_from_isr()
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        if (xSemaphoreTakeFromISR(sem_, &xHigherPriorityTaskWoken) == pdFALSE) {
            return false;
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return true;
    }

    bool unlock_from_thread()
    {
        if (xSemaphoreGive(sem_) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    }

    bool unlock_from_isr()
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        if (xSemaphoreGiveFromISR(sem_, &xHigherPriorityTaskWoken) == pdFALSE) {
            return false;
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        return true;
    }
};
} // namespace freertos_lock_internal

class CountingSemphr : public freertos_lock_internal::FreeRtosSemBase
{
public:
    /**
     * @brief 创建一个计数信号量
     *
     * @param max_count 最大计数值
     * @param initial_count 初始计数值
     */
    CountingSemphr(UBaseType_t max_count, UBaseType_t initial_count = 0)
    {
        sem_ = xSemaphoreCreateCountingStatic(max_count, initial_count, &sem_buffer_);
    }
};

class BinarySemphr : public freertos_lock_internal::FreeRtosSemBase
{
public:
    /**
     * @brief 创建一个二值信号量
     *
     * @param unlock_on_create 创建时是否先解锁（把计数值设为1）
     */
    BinarySemphr(bool unlock_on_create = false)
    {
        sem_ = xSemaphoreCreateBinaryStatic(&sem_buffer_);
        if (unlock_on_create) {
            unlock();
        }
    }
};

#ifdef configUSE_MUTEXES
class Mutex
{
private:
    StaticSemaphore_t sem_buffer_;
    SemaphoreHandle_t sem_ = nullptr;

public:
    /**
     * @brief 创建一个互斥锁
     * @note 创建时就已经是解锁状态
     */
    Mutex()
    {
        sem_ = xSemaphoreCreateMutexStatic(&sem_buffer_);
    }

    bool lock(TickType_t xTicksToWait = portMAX_DELAY)
    {
        if (xSemaphoreTake(sem_, xTicksToWait) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    }

    bool unlock()
    {
        if (xSemaphoreGive(sem_) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    }
};
#endif // configUSE_MUTEXES

#ifdef configUSE_RECURSIVE_MUTEXES
class RecursiveMutex
{
private:
    StaticSemaphore_t sem_buffer_;
    SemaphoreHandle_t sem_ = nullptr;

public:
    /**
     * @brief 创建一个递归互斥锁
     * @note 创建时就已经是解锁状态
     */
    RecursiveMutex()
    {
        sem_ = xSemaphoreCreateRecursiveMutexStatic(&sem_buffer_);
    }

    bool lock(TickType_t xTicksToWait = portMAX_DELAY)
    {
        if (xSemaphoreTakeRecursive(sem_, xTicksToWait) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    }

    bool unlock()
    {
        if (xSemaphoreGiveRecursive(sem_) == pdTRUE) {
            return true;
        } else {
            return false;
        }
    }
};
#endif // configUSE_RECURSIVE_MUTEXES

} // namespace freertos_lock
