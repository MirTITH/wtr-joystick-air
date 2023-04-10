#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C" {
#endif

void StartLvglThread();
BaseType_t LvglLock(TickType_t block_time);
BaseType_t LvglUnlock();

#ifdef __cplusplus
}
#endif
