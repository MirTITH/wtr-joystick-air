#pragma once
#include "main.h"

void TestThreadEntry(void *argument);

#ifdef __cplusplus
extern "C" {
#endif

void PreSleepProcessing(uint32_t ulExpectedIdleTime);
void PostSleepProcessing(uint32_t ulExpectedIdleTime);

#ifdef __cplusplus
}
#endif
