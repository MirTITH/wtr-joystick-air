#include <string.h>

/**
 * @brief 清理内存的函数
 * @param start_pos 开始地址
 * @param end_pos 结束地址
 */
static void ClearMem(void *start_pos, void *end_pos)
{
    memset(start_pos, 0, (size_t)end_pos - (size_t)start_pos);
}

/**
 * @brief 将 FLASH 中的数据复制到内存中
 *
 */
static void CopyData(void *sidata, void *sdata, void *edata)
{
    memcpy(sdata, sidata, (size_t)edata - (size_t)sdata);
}

/**
 * @brief 用于在 main() 之前初始化内存
 * @note stm32 官方库自带的 startup.s 只会初始化一块 bss 段内存，如果在多个内存区域存在 bss 段则不会初始化
 * @note 于是我修改了 startup.s，在 main() 之前调用这个函数初始化这些内存
 * @note （才不是因为不会写汇编才用 c 语言初始化内存）
 *
 */
void __UserSystemInit()
{
    // __HAL_RCC_D2SRAM1_CLK_ENABLE();
    // __HAL_RCC_D2SRAM2_CLK_ENABLE();
    // __HAL_RCC_D2SRAM3_CLK_ENABLE();

    // 各区域bss段的开始结束地址（定义在 ld 文件中）
    extern size_t _sbss_d1, _sbss_d2, _sbss_d3, _sbss_itcm, _sbss_dtcm;
    extern size_t _ebss_d1, _ebss_d2, _ebss_d3, _ebss_itcm, _ebss_dtcm;

    // 清零各区域的 bss 段
    ClearMem(&_sbss_d1, &_ebss_d1);
    ClearMem(&_sbss_d2, &_ebss_d2);
    ClearMem(&_sbss_d3, &_ebss_d3);
    ClearMem(&_sbss_itcm, &_ebss_itcm);
    ClearMem(&_sbss_dtcm, &_ebss_dtcm);

    extern size_t _sidata_itcm, _sdata_itcm, _edata_itcm;
    CopyData(&_sidata_itcm, &_sdata_itcm, &_edata_itcm);
}
