#include "user_test.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "adc.h"
#include "lvgl_thread.h"
#include "lvgl/lvgl.h"
#include <sstream>
#include "HighPrecisionTime/high_precision_time.h"
#include "Mavlink/wtr_mavlink.h"
#include <iomanip>
#include "LvglApp/app_mgr.hpp"
#include "test_app.hpp"
#include "App/dashboard.hpp"
#include "Joystick/joystick_define.hpp"
#include "Encoder/encoder_define.hpp"
#include "as69_config.hpp"
#include "TouchScreen/GT911/gt911_define.hpp"
#include "Button/buttons.h"
#include "freertos_lock/freertos_lock.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "eMPL_outputs.h"
#include "mltypes.h"
// #include "mpu.h"
// #include "log.h"
// #include "mpumpl/driver/stm32L/packet.h"

#ifdef __cplusplus
}
#endif

using namespace std;

#define DEFAULT_MPU_HZ  (200)
#define COMPASS_READ_MS (100)

// 陀螺仪方向设置
static signed char gyro_orientation[9] = {1, 0, 0,
                                          0, 1, 0,
                                          0, 0, 1};
// 磁力计方向设置
static signed char comp_orientation[9] = {0, 1, 0,
                                          1, 0, 0,
                                          0, 0, -1};

/**
 * @brief 9250自测试
 *
 * @return 0则正常
 */
uint8_t run_self_test(void)
{
    int result;
    // char test_packet[4] = {0};
    long gyro[3], accel[3];
    result = mpu_run_6500_self_test(gyro, accel, 0);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        unsigned short accel_sens;
        float gyro_sens;

        mpu_get_gyro_sens(&gyro_sens);
        gyro[0] = (long)(gyro[0] * gyro_sens);
        gyro[1] = (long)(gyro[1] * gyro_sens);
        gyro[2] = (long)(gyro[2] * gyro_sens);
        // inv_set_gyro_bias(gyro, 3);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        // inv_set_accel_bias(accel, 3);
        dmp_set_accel_bias(accel);
        return 0;
    } else
        return 1;
}

static void tap_cb(unsigned char direction, unsigned char count)
{
    // switch (direction) {
    //     case TAP_X_UP:
    //         MPL_LOGI("Tap X+ ");
    //         break;
    //     case TAP_X_DOWN:
    //         MPL_LOGI("Tap X- ");
    //         break;
    //     case TAP_Y_UP:
    //         MPL_LOGI("Tap Y+ ");
    //         break;
    //     case TAP_Y_DOWN:
    //         MPL_LOGI("Tap Y- ");
    //         break;
    //     case TAP_Z_UP:
    //         MPL_LOGI("Tap Z+ ");
    //         break;
    //     case TAP_Z_DOWN:
    //         MPL_LOGI("Tap Z- ");
    //         break;
    //     default:
    //         return;
    // }
    // MPL_LOGI("x%d\n", count);
    // return;
}

static void android_orient_cb(unsigned char orientation)
{
    // switch (orientation) {
    //     case ANDROID_ORIENT_PORTRAIT:
    //         MPL_LOGI("Portrait\n");
    //         break;
    //     case ANDROID_ORIENT_LANDSCAPE:
    //         MPL_LOGI("Landscape\n");
    //         break;
    //     case ANDROID_ORIENT_REVERSE_PORTRAIT:
    //         MPL_LOGI("Reverse Portrait\n");
    //         break;
    //     case ANDROID_ORIENT_REVERSE_LANDSCAPE:
    //         MPL_LOGI("Reverse Landscape\n");
    //         break;
    //     default:
    //         return;
    // }
}

uint8_t MPU9250_MPL_Init(void)
{
    uint8_t res = 0;
    struct int_param_s int_param;
    uint8_t accel_fsr;
    uint16_t gyro_rate, gyro_fsr;
    uint16_t compass_fsr;

    if (!mpu_init(&int_param)) {
        res = inv_init_mpl(); // 初始化MPL
        if (res)
            return 1;
        inv_enable_quaternion();
        inv_enable_9x_sensor_fusion();
        inv_enable_fast_nomot();
        inv_enable_gyro_tc();
        inv_enable_vector_compass_cal();
        inv_enable_in_use_auto_calibration();
        // inv_enable_magnetic_disturbance();
        inv_enable_eMPL_outputs();
        res = inv_start_mpl(); // 开启MPL
        if (res) return 1;
        printf("mpl start OK!\n");
        res = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS); // 设置所需要的传感器
        if (res) return 2;
        printf("sensor set OK!\n");
        res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL); // 设置FIFO
        if (res) return 3;
        printf("FIFO set OK!\n");
        res = mpu_set_sample_rate(DEFAULT_MPU_HZ); // 设置采样率//宏定义在头文件COMPASS_READ_MS
        if (res) return 4;
        printf("Sample Rate set OK!\n");
        res = mpu_set_compass_sample_rate(1000 / COMPASS_READ_MS); // 设置磁力计采样率
        if (res) return 5;
        printf("Compass_Sample_Rate set OK!\n");
        mpu_get_sample_rate(&gyro_rate);
        mpu_get_gyro_fsr(&gyro_fsr);
        mpu_get_accel_fsr(&accel_fsr);
        mpu_get_compass_fsr(&compass_fsr);
        inv_set_gyro_sample_rate(1000000L / gyro_rate);
        inv_set_accel_sample_rate(1000000L / gyro_rate);
        inv_set_compass_sample_rate(COMPASS_READ_MS * 1000L); // 宏定义在头文件COMPASS_READ_MS
        inv_set_gyro_orientation_and_scale(inv_orientation_matrix_to_scalar(gyro_orientation), (long)gyro_fsr << 15);
        inv_set_accel_orientation_and_scale(inv_orientation_matrix_to_scalar(gyro_orientation), (long)accel_fsr << 15);
        inv_set_compass_orientation_and_scale(inv_orientation_matrix_to_scalar(comp_orientation), (long)compass_fsr << 15);

        res = dmp_load_motion_driver_firmware(); // 加载dmp固件
        if (res) return 6;
        printf("DMP load OK!\n");

        res = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)); // 设置陀螺仪方向
        if (res) return 7;
        printf("DMP set orientation OK!\n");

        dmp_register_tap_cb(tap_cb);
        dmp_register_android_orient_cb(android_orient_cb);

        res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
                                 DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
                                 DMP_FEATURE_GYRO_CAL);
        // res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT |
        //                          DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL);
        if (res) return 8;
        printf("DMP feature set OK!\n");
        res = dmp_set_fifo_rate(DEFAULT_MPU_HZ); // 设置DMP输出速率(最大不超过200Hz)
        if (res)
            return 9;
        printf("DMP output rate set OK!\n");

        res = run_self_test(); // 自检
        if (res) {
            printf("Self test failed! Make sure the device face up\n");
            return 10;
        }
        printf("self test OK!\n");

        res = mpu_set_dmp_state(1); // 使能DMP
        if (res)
            return 11;
        printf("DMP enabled\n");
        return 0;
    } else
        return 1;
}

/**
 * @brief 获取MPL处理后的数据
 *
 * @param ptich 俯仰角 精度0.1 范围 -90<-->90
 * @param roll 	横滚角 精度0.1 范围-180<-->180
 * @param yaw 	航向角 精度0.1 范围-180<-->180
 * @return 0则正常，其他则失败
 */
uint8_t MPU9250_MPL_getData(float *pitch, float *roll, float *yaw)
{
    unsigned long sensor_timestamp;
    inv_time_t timestamp;
    short gyro[3], accel_short[3], compass_short[3], sensors;
    unsigned char more;
    long compass[3], accel[3], quat[4], temperature;
    long data[9];
    int8_t accuracy;

    // q30，q16格式,long转float时的除数.
    // static const float q30 = 1073741824.0f;
    static const float q16 = 65536.0f;

    if (dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more))
        return 1;

    if (sensors & INV_XYZ_GYRO) {
        inv_build_gyro(gyro, sensor_timestamp); // 把新数据发送给MPL
        mpu_get_temperature(&temperature, &sensor_timestamp);
        inv_build_temp(temperature, sensor_timestamp); // 把温度值发给MPL，只有陀螺仪需要温度值
    }

    if (sensors & INV_XYZ_ACCEL) {
        accel[0] = (long)accel_short[0];
        accel[1] = (long)accel_short[1];
        accel[2] = (long)accel_short[2];
        inv_build_accel(accel, 0, sensor_timestamp); // 把加速度值发给MPL
    }

    if (sensors & INV_WXYZ_QUAT) {
        inv_build_quat(quat, 0, sensor_timestamp);
    }

    if (!mpu_get_compass_reg(compass_short, &sensor_timestamp)) {
        compass[0] = (long)compass_short[0];
        compass[1] = (long)compass_short[1];
        compass[2] = (long)compass_short[2];
        inv_build_compass(compass, 0, sensor_timestamp); // 把磁力计值发给MPL
    }
    inv_execute_on_data();
    inv_get_sensor_type_euler(data, &accuracy, &timestamp);

    *roll  = (data[0] / q16);
    *pitch = -(data[1] / q16);
    *yaw   = -data[2] / q16;
    return 0;
}

freertos_lock::BinarySemphr kMpuSem;
uint32_t kMpuIntCount = 0;

void MpuInt()
{
    kMpuIntCount++;
    kMpuSem.unlock_from_isr();
}

void MpuLoop()
{
    unsigned long sensor_timestamp;
    inv_time_t timestamp;
    short gyro[3], accel_short[3], compass_short[3], sensors;
    unsigned char more;
    long compass[3], accel[3], quat[4], temperature;
    long data[9];
    int8_t accuracy;

    // q30，q16格式,long转float时的除数.
    static const float q30 = 1073741824.0f;
    static const float q16 = 65536.0f;

    if (dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more)) {
        return;
    }

    static int count_xyz_gyro = 0;
    if (sensors & INV_XYZ_GYRO) {
        count_xyz_gyro++;
        inv_build_gyro(gyro, sensor_timestamp); // 把新数据发送给MPL
        mpu_get_temperature(&temperature, &sensor_timestamp);
        inv_build_temp(temperature, sensor_timestamp); // 把温度值发给MPL，只有陀螺仪需要温度值
    }

    static int count_xyz_accel = 0;
    if (sensors & INV_XYZ_ACCEL) {
        count_xyz_accel++;
        accel[0] = (long)accel_short[0];
        accel[1] = (long)accel_short[1];
        accel[2] = (long)accel_short[2];
        inv_build_accel(accel, 0, sensor_timestamp); // 把加速度值发给MPL
    }

    static int count_wxyz_quat = 0;
    if (sensors & INV_WXYZ_QUAT) {
        count_wxyz_quat++;
        inv_build_quat(quat, 0, sensor_timestamp);
    }

    static int count_compass        = 0;
    static uint32_t next_compass_ms = HAL_GetTick();
    if (HAL_GetTick() > next_compass_ms) {
        if (!mpu_get_compass_reg(compass_short, &sensor_timestamp)) {
            next_compass_ms += 100;
            count_compass++;
            compass[0] = (long)compass_short[0];
            compass[1] = (long)compass_short[1];
            compass[2] = (long)compass_short[2];
            inv_build_compass(compass, 0, sensor_timestamp); // 把磁力计值发给MPL
        }
    }

    inv_execute_on_data();

    // inv_get_sensor_type_euler(data, &accuracy, &timestamp);
    // inv_get_sensor_type_quat

    // float roll  = (data[0] / q16);
    // float pitch = -(data[1] / q16);
    // float yaw   = -data[2] / q16;

    // printf("%f,%f,%f,%d,%d,%d,%d,%d\n", pitch, yaw, roll, accuracy,
    //        count_xyz_gyro, count_xyz_accel, count_wxyz_quat, count_compass);

    inv_get_sensor_type_quat(data, &accuracy, &timestamp);
    printf("%f,%f,%f,%f,%d,%d,%lu\n", data[0] / q30, data[1] / q30, data[2] / q30, data[3] / q30, accuracy, more, kMpuIntCount);
}

void TestThreadEntry(void *argument)
{
    (void)argument;

    // auto joystickl_dashboard = dashboard_mgr.NewDashboard(250, "JoystickL");

    stringstream sstr;
    sstr.precision(2);
    sstr.setf(std::ios::fixed);

    printf("Start MPU9250_MPL_Init\n");
    int result = MPU9250_MPL_Init();
    printf("MPU9250_MPL_Init: %d\n", result);
    // vTaskDelay(1000);

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    while (true) {
        // float pitch, roll, yaw;
        // if (!MPU9250_MPL_getData(&pitch, &roll, &yaw))
        //     printf("%f,%f,%f\n", pitch, yaw, roll);
        // else
        //     printf("error:cannot get data\n");
        kMpuSem.lock();
        MpuLoop();
        // vTaskDelay(10);
        // vTaskDelayUntil(&PreviousWakeTime, 25);
    }
}
