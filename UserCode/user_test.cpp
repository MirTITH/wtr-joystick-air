#include "user_test.hpp"
#include "cmsis_os.h"
#include "lvgl.h"
#include "demos/lv_demos.h"

#define Led_Pin  GPIO_PIN_1
#define Led_Port GPIOA
static SemaphoreHandle_t LvglMutex;

static void anim_x_cb(void *var, int32_t v)
{
    lv_obj_set_x((_lv_obj_t *)var, v);
}

static void anim_y_cb(void *var, int32_t v)
{
    lv_obj_set_y((_lv_obj_t *)var, v);
}

static void anim_color_cb(void *var, int32_t v)
{
    lv_obj_set_style_bg_color((_lv_obj_t *)var, lv_color_hsv_to_rgb(v, 50, 70), 0);
}

// static void anim_size_cb(void *var, int32_t v)
// {
//     lv_obj_set_size((_lv_obj_t *)var, v, v);
// }

/**
 * Create a playback animation
 */
void lv_example_anim_2(void)
{
    lv_obj_t *obj = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BROWN), 0);

    lv_obj_set_size(obj, lv_pct(100), 40);
    // lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_playback_time(&a, 1000);

    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_anim_set_exec_cb(&a, anim_y_cb);
    lv_anim_set_values(&a, -40, 320);
    lv_anim_start(&a);
}

void lv_example_anim_1(void)
{
    lv_obj_t *obj = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_PURPLE), 0);

    lv_obj_set_size(obj, 40, lv_pct(100));
    // lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 0, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_playback_time(&a, 1000);

    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_values(&a, 10, 480);
    lv_anim_start(&a);
}

void lv_example_anim_3(void)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, lv_scr_act());
    lv_anim_set_time(&a, 1500);

    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_anim_set_exec_cb(&a, anim_color_cb);
    lv_anim_set_values(&a, 0, 359);
    lv_anim_start(&a);
}

BaseType_t LvglLock(TickType_t block_time = portMAX_DELAY)
{
    return xSemaphoreTakeRecursive(LvglMutex, block_time);
}

BaseType_t LvglUnlock()
{
    return xSemaphoreGiveRecursive(LvglMutex);
}

void TestThreadEntry(void *argument)
{
    (void)argument;

    LvglMutex = xSemaphoreCreateRecursiveMutex();

    // lv_example_anim_1();
    // lv_example_anim_2();
    // lv_example_anim_3();

    lv_demo_benchmark();

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    for (;;) {
        LvglLock();
        lv_timer_handler();
        LvglUnlock();

        vTaskDelayUntil(&PreviousWakeTime, 5);
    }
}
