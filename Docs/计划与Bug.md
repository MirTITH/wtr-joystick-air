# 计划与小虫子

## 计划
### 高优先级
- [ ] DTCMRAM 的 MPU 配置
- [ ] 尝试使用 ITCMRAM
- [ ] 更好的 bss 段 初始化

### 低优先级
- [ ] 完成摇杆驱动
- [ ] 编码器驱动
- [ ] 统一 IO 设备框架
- [ ] mavlink
- [ ] 消息接收与屏幕显示
- [ ] 波形图绘制
- [ ] 设置界面
- [ ] 使用板载 Flash 芯片
- [ ] 优化 ScreenConsole

### 已完成

## Bugs
- [ ] SD 卡有时读取失败，f_mount 挂载失败，SDMMC_ERROR_DATA_TIMEOUT
    > 怀疑和温度有关，低温时出现  
    > 在 stm32h7xx_hal_sd.c:3386 处 return HAL_SD_ERROR_DATA_TIMEOUT;  
    > hsd1.Instance.STA = 0x101008
