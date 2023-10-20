#include "mpumpl.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "freertos_lock/freertos_lock.hpp"
#include <mutex>

#ifdef __cplusplus
extern "C" {
#endif

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "eMPL_outputs.h"
#include "mltypes.h"

#ifdef __cplusplus
}
#endif

#define DEFAULT_MPU_HZ  (200) // MPU 数据输出速率
#define COMPASS_READ_MS (100) // 磁力计读取周期(ms)

// 陀螺仪方向设置
static signed char gyro_orientation[9] = {1, 0, 0,
                                          0, 1, 0,
                                          0, 0, 1};
// 磁力计方向设置
static signed char comp_orientation[9] = {0, 1, 0,
                                          1, 0, 0,
                                          0, 0, -1};

// q30，q16格式,long转float时的除数
static const float q30 = 1073741824.0f;
static const float q16 = 65536.0f;

static freertos_lock::BinarySemphr kMpuInterruptSem;
freertos_lock::Mutex kMpuDataMutex;
uint32_t kMpuIntCount = 0;

struct MpuData {
    int16_t gyro[3], accel[3], compass[3];
    int32_t quat[4];
    uint32_t sensor_timestamp;
    int16_t sensors;
    uint8_t more;
} kMpuData;

void Mpu_InterruptCallback()
{
    kMpuIntCount++;
    kMpuInterruptSem.unlock_from_isr();
}

__weak void tap_cb(unsigned char direction, unsigned char count)
{
    (void)direction;
    (void)count;
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
}

__weak void android_orient_cb(unsigned char orientation)
{
    (void)orientation;
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

/**
 * @brief 9250自测试
 *
 * @return 0则正常
 */
// static uint8_t run_self_test()
// {
//     int result;
//     // char test_packet[4] = {0};
//     long gyro[3], accel[3];
//     result = mpu_run_6500_self_test(gyro, accel, 0);
//     if (result == 0x7) {
//         /* Test passed. We can trust the gyro data here, so let's push it down
//          * to the DMP.
//          */
//         unsigned short accel_sens;
//         float gyro_sens;

//         mpu_get_gyro_sens(&gyro_sens);
//         gyro[0] = (long)(gyro[0] * gyro_sens);
//         gyro[1] = (long)(gyro[1] * gyro_sens);
//         gyro[2] = (long)(gyro[2] * gyro_sens);
//         // inv_set_gyro_bias(gyro, 3);
//         dmp_set_gyro_bias(gyro);
//         mpu_get_accel_sens(&accel_sens);
//         accel[0] *= accel_sens;
//         accel[1] *= accel_sens;
//         accel[2] *= accel_sens;
//         // inv_set_accel_bias(accel, 3);
//         dmp_set_accel_bias(accel);
//         return 0;
//     } else
//         return 1;
// }

static int Mpu_LibInit()
{
    int res = 0;
    struct int_param_s int_param;
    uint8_t accel_fsr;
    uint16_t gyro_rate, gyro_fsr;
    uint16_t compass_fsr;

    res = mpu_init(&int_param);
    if (res) return 255;

    res = inv_init_mpl(); // 初始化MPL
    if (res) return 254;

    inv_enable_quaternion();
    inv_enable_9x_sensor_fusion();
    inv_enable_fast_nomot();
    inv_enable_gyro_tc();
    inv_enable_vector_compass_cal();
    inv_enable_in_use_auto_calibration();
    // inv_enable_magnetic_disturbance(); // 开启磁场抗干扰（检测到磁场干扰时自动切换到6轴算法）
    inv_enable_eMPL_outputs();

    res = inv_start_mpl(); // 开启MPL
    if (res) return 1;

    res = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS); // 设置所需要的传感器
    if (res) return 2;

    res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL); // 设置FIFO
    if (res) return 3;

    res = mpu_set_sample_rate(DEFAULT_MPU_HZ); // 设置采样率
    if (res) return 4;

    res = mpu_set_compass_sample_rate(1000 / COMPASS_READ_MS); // 设置磁力计采样率
    if (res) return 5;

    mpu_get_sample_rate(&gyro_rate);
    mpu_get_gyro_fsr(&gyro_fsr);
    mpu_get_accel_fsr(&accel_fsr);
    mpu_get_compass_fsr(&compass_fsr);
    inv_set_gyro_sample_rate(1000000L / gyro_rate);
    inv_set_accel_sample_rate(1000000L / gyro_rate);
    inv_set_compass_sample_rate(COMPASS_READ_MS * 1000L);
    inv_set_gyro_orientation_and_scale(inv_orientation_matrix_to_scalar(gyro_orientation), (long)gyro_fsr << 15);
    inv_set_accel_orientation_and_scale(inv_orientation_matrix_to_scalar(gyro_orientation), (long)accel_fsr << 15);
    inv_set_compass_orientation_and_scale(inv_orientation_matrix_to_scalar(comp_orientation), (long)compass_fsr << 15);

    res = dmp_load_motion_driver_firmware(); // 加载dmp固件
    if (res) return 6;

    res = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)); // 设置陀螺仪方向
    if (res) return 7;

    dmp_register_tap_cb(tap_cb);                       // 敲击检测回调函数
    dmp_register_android_orient_cb(android_orient_cb); // 旋转检测回调函数

    res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
                             DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
                             DMP_FEATURE_GYRO_CAL);
    if (res) return 8;

    res = dmp_set_fifo_rate(DEFAULT_MPU_HZ); // 设置DMP输出速率(最大不超过200Hz)
    if (res) return 9;

    // 自检需要保持遥控器朝上
    // res = run_self_test(); // 自检
    // if (res) return 10;

    res = mpu_set_dmp_state(1); // 使能DMP
    if (res) return 11;
    return 0;
}

static void Mpu_ThreadEntry(void *)
{
    while (1) {
        kMpuInterruptSem.lock(); // 等待 MPU9250 中断信号

        std::lock_guard lock(kMpuDataMutex);
        if (dmp_read_fifo(kMpuData.gyro, kMpuData.accel, kMpuData.quat, &kMpuData.sensor_timestamp, &kMpuData.sensors, &kMpuData.more)) {
            continue;
        }

        // 以下代码用于 9 轴融合。（9 轴融合需要 8 字校准，且容易受干扰，不使用）
        // static int count_xyz_gyro = 0;
        // if (sensors & INV_XYZ_GYRO) {
        //     count_xyz_gyro++;
        //     inv_build_gyro(gyro, sensor_timestamp); // 把新数据发送给MPL
        //     mpu_get_temperature(&temperature, &sensor_timestamp);
        //     inv_build_temp(temperature, sensor_timestamp); // 把温度值发给MPL，只有陀螺仪需要温度值
        // }

        // static int count_xyz_accel = 0;
        // if (sensors & INV_XYZ_ACCEL) {
        //     count_xyz_accel++;
        //     accel[0] = (long)accel_short[0];
        //     accel[1] = (long)accel_short[1];
        //     accel[2] = (long)accel_short[2];
        //     inv_build_accel(accel, 0, sensor_timestamp); // 把加速度值发给MPL
        // }

        // static int count_wxyz_quat = 0;
        // if (sensors & INV_WXYZ_QUAT) {
        //     count_wxyz_quat++;
        //     inv_build_quat(quat, 0, sensor_timestamp);
        // }

        // static int count_compass        = 0;
        // static uint32_t next_compass_ms = HAL_GetTick();
        // if (HAL_GetTick() > next_compass_ms) {
        //     if (!mpu_get_compass_reg(compass_short, &sensor_timestamp)) {
        //         next_compass_ms += 100;
        //         count_compass++;
        //         compass[0] = (long)compass_short[0];
        //         compass[1] = (long)compass_short[1];
        //         compass[2] = (long)compass_short[2];
        //         inv_build_compass(compass, 0, sensor_timestamp); // 把磁力计值发给MPL
        //     }
        // }

        // inv_execute_on_data();
    }
}

int Mpu_Init()
{
    int result = Mpu_LibInit();
    if (result) return result;
    xTaskCreate(Mpu_ThreadEntry, "MpuThread", 512, nullptr, 3, nullptr);
    return 0;
}

std::array<float, 4> Mpu_GetQuat()
{
    std::array<float, 4> result;

    std::lock_guard lock(kMpuDataMutex);
    for (size_t i = 0; i < result.size(); i++) {
        result[i] = kMpuData.quat[i] / q30;
    }

    return result;
}
