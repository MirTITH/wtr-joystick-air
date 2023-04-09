/**
 * @file stm32_st7796.hpp
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <stdint.h>
#include "port/lcd_fmc.hpp"
#include "port/lcd_backlight.hpp"

class LcdSt7796 : LcdFmc, LcdBacklight
{
public:
    enum class Rotation {
        portrait,
        landscape,
        inverse_portrait,
        inverse_landscape
    };

    enum class ColorOrder {
        BGR,
        RGB
    };

private:
    ColorOrder _color_order;
    Rotation _rotation = Rotation::landscape;
    int _now_width;  // becasue we can change these later if needed
    int _now_height; // eg. in setRotation()

    void delay(uint32_t delay_ms)
    {
        HAL_Delay(delay_ms - 1);
    }

    void updateRotationAndColorOrder();
    void setDataWritingArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

public:
    const int PHYSICAL_WIDTH  = 320;
    const int PHYSICAL_HEIGHT = 480;

    LcdSt7796(int chip_select, TIM_HandleTypeDef *backlight_tim = nullptr, uint32_t channel = 0, ColorOrder color_order = ColorOrder::BGR, Rotation rotation = Rotation::landscape)
        : LcdFmc(chip_select), LcdBacklight(backlight_tim, channel), _color_order(color_order), _rotation(rotation)
    {
    }

    void InitAll()
    {
        InitDisplay();
        InitBacklight();
    }
    void InitDisplay();
    void InitBacklight()
    {
        SetBacklight(0.5);
    }
    void DisplayOnOff(bool is_display_on);
    void SetRotation(Rotation rotation);
    void SetColorOrder(ColorOrder color_order);
    ColorOrder GetColorOrder() const
    {
        return _color_order;
    }
    void InvertDisplay(bool is_invert);
    void WriteScreen(int x1, int y1, int x2, int y2, uint16_t *data);
    int16_t GetScanline();
};
