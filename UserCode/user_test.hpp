#pragma once
#include "main.h"
#include "FreeRTOS.h"

void TestThreadEntry(void *argument);

#ifdef __cplusplus
extern "C" {
#endif

void MpuInt();

#ifdef __cplusplus
}
#endif
