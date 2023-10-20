/**
 * @file mpumpl.hpp
 * @author X. Y.
 * @brief MPU9250 IMU 驱动
 * @version 0.1
 * @date 2023-10-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "main.h"
#include <array>

void Mpu_InterruptCallback();

int Mpu_Init();

std::array<float, 4> Mpu_GetQuat();
