#include "user_test.hpp"
#include "cmsis_os.h"
#include "lcd_st7796.hpp"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA

uint16_t screen_buffer[480][320];

LcdSt7796 LCD(1,
              LcdSt7796::Rotation::landscape,
              LcdSt7796::ColorOrder::BGR,
              &htim8, 1,
              &hmdma_mdma_channel40_sw_0);

void TestThreadEntry(void *argument)
{
    (void)argument;

    LCD.InitAll();
    LCD.FillScreen(0x0000);

    uint16_t pixel = 0x0000;
    while (1) {
        for (int i = 0; i < 480; i++) {
            for (int j = 0; j < 320; j++) {
                screen_buffer[i][j] = rand();
            }
        }
        LCD.WriteScreenDma(0, 0, 479, 319, (uint16_t *)screen_buffer);

        if (pixel == 0x0000) {
            pixel = 0xffff;
        } else {
            pixel = 0x0000;
        }

        LCD.FillArea(239, 10, 241, 12, pixel);
        // vTaskDelay(500);
    }
}
