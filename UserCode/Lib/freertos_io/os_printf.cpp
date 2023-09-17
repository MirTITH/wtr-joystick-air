#include "os_printf.h"
#include <stdarg.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "freertos_lock/freertos_lock.hpp"
#include <mutex>

static freertos_lock::BinarySemphr kPrintLock{true};

int os_printf(const char *format, ...)
{
    std::lock_guard lock(kPrintLock);

    va_list arg;
    int done;

    va_start(arg, format);
    done = vfprintf(stdout, format, arg);
    va_end(arg);

    return done;
}
