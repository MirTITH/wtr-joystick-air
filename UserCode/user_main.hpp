#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 系统初始化，必须在使能MPU前调用
 *
 */
void SysInit();

void StartDefaultTask(void *argument);

#ifdef __cplusplus
}
#endif
