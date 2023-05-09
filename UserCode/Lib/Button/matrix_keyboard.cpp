#include "matrix_keyboard.hpp"
#include "HighPrecisionTime/high_precision_time.h"
#include "main.h"

static void PreInitGpio()
{
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(Kb_MR1_GPIO_Port, Kb_MR1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Kb_MR2_GPIO_Port, Kb_MR2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Kb_MR3_GPIO_Port, Kb_MR3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Kb_MR4_GPIO_Port, Kb_MR4_Pin, GPIO_PIN_SET);

    // HAL_GPIO_WritePin(Kb_MC1_GPIO_Port, Kb_MC1_Pin, GPIO_PIN_SET);
    // HAL_GPIO_WritePin(Kb_MC2_GPIO_Port, Kb_MC2_Pin, GPIO_PIN_SET);
    // HAL_GPIO_WritePin(Kb_MC3_GPIO_Port, Kb_MC3_Pin, GPIO_PIN_SET);
    // HAL_GPIO_WritePin(Kb_MC4_GPIO_Port, Kb_MC4_Pin, GPIO_PIN_SET);

    GPIO_InitTypeDef GPIO_InitStruct = {0, 0, 0, GPIO_SPEED_FREQ_LOW, 0};

    // Row
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    GPIO_InitStruct.Pin = Kb_MR1_Pin;
    HAL_GPIO_Init(Kb_MR1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Kb_MR2_Pin | Kb_MR3_Pin;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Kb_MR4_Pin;
    HAL_GPIO_Init(Kb_MR4_GPIO_Port, &GPIO_InitStruct);

    // Column
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    GPIO_InitStruct.Pin = Kb_MC1_Pin | Kb_MC2_Pin;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Kb_MC3_Pin;
    HAL_GPIO_Init(Kb_MC3_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Kb_MC4_Pin;
    HAL_GPIO_Init(Kb_MC4_GPIO_Port, &GPIO_InitStruct);
}

uint8_t ReadColumn()
{
    uint8_t result = 0xf0;
    result |= ((int)HAL_GPIO_ReadPin(Kb_MC1_GPIO_Port, Kb_MC1_Pin)) << 0;
    result |= ((int)HAL_GPIO_ReadPin(Kb_MC2_GPIO_Port, Kb_MC2_Pin)) << 1;
    result |= ((int)HAL_GPIO_ReadPin(Kb_MC3_GPIO_Port, Kb_MC3_Pin)) << 2;
    result |= ((int)HAL_GPIO_ReadPin(Kb_MC4_GPIO_Port, Kb_MC4_Pin)) << 3;
    return ~result;
}

void SetRow(uint8_t index)
{
    GPIO_PinState row[4] = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET};
    row[index]           = GPIO_PIN_RESET;
    HAL_GPIO_WritePin(Kb_MR1_GPIO_Port, Kb_MR1_Pin, row[0]);
    HAL_GPIO_WritePin(Kb_MR2_GPIO_Port, Kb_MR2_Pin, row[1]);
    HAL_GPIO_WritePin(Kb_MR3_GPIO_Port, Kb_MR3_Pin, row[2]);
    HAL_GPIO_WritePin(Kb_MR4_GPIO_Port, Kb_MR4_Pin, row[3]);
}

void MatrixKeyboard_Init()
{
    PreInitGpio();
}

uint16_t MatrixKeyboard_Scan()
{
    uint16_t result = 0;

    // SetRow(2);
    // HPT_DelayUs(1000);
    // return ReadColumn();

    for (size_t i = 0; i < 4; i++) {
        SetRow(i);
        HPT_DelayUs(1000);
        result |= ReadColumn() << (i * 4);
    }

    return result;
}
