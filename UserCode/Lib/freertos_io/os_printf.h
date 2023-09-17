#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 线程安全的 printf
 * @note 只在线程中调用是安全的；只在一个中断中调用是安全的；在多个优先级相同的中断中调用是安全的
 * @note 同时在中断和线程中调用不安全；在多个优先级不同的中断中调用不安全；
 * @note 这些不安全行为应该只会导致打印出的东西错位，应该不会造成严重内存溢出
 *
 */
int os_printf(const char *format, ...) __attribute__((__format__(__printf__, 1, 2)));

#ifdef __cplusplus
}
#endif
