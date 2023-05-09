#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ReadColumn();

void SetRow(uint8_t index);

void MatrixKeyboard_Init();

uint16_t MatrixKeyboard_Scan();

#ifdef __cplusplus
}
#endif
