/*
 * sen15901.c
 *
 *  Created on: 07 sep. 2024
 *      Author: Ludo
 */

#include "sen15901.h"

#ifndef SEN15901_DRIVER_DISABLE_FLAGS_FILE
#include "sen15901_driver_flags.h"
#endif
#include "error.h"
#include "math.h"
#include "sen15901_hw.h"
#include "types.h"

#ifndef SEN15901_DRIVER_DISABLE

/*** SEN15901 local macros ***/

#define SEN15901_WIND_SPEED_1HZ_TO_MH                           2400
#define SEN15901_WIND_DIRECTIONS_NUMBER                         16

#define SEN15901_RAIN_EDGE_TO_UM                                279

#define SEN15901_RESISTOR_DIVIDER_RATIO(rw)                     ((MATH_PERMILLE_MAX * rw) / (rw + SEN15901_DRIVER_WIND_DIRECTION_PULL_UP_RESISTOR_OHMS))
#define SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(rw1, rw2)     ((SEN15901_RESISTOR_DIVIDER_RATIO(rw1) + SEN15901_RESISTOR_DIVIDER_RATIO(rw2)) >> 1)

/*** SEN15901 local structures ***/

/*******************************************************************/
typedef struct {
    // State machine.
    SEN15901_process_cb_t process_callback;
    volatile uint8_t tick_second_flag;
    uint8_t wind_measurement_enable_flag;
    // Wind speed.
    volatile uint8_t wind_speed_seconds_count;
    volatile uint32_t wind_speed_edge_count;
    uint32_t wind_speed_data_count;
    uint32_t wind_speed_mh;
    uint32_t wind_speed_mh_average;
    uint32_t wind_speed_mh_peak;
    // Wind direction.
    volatile uint8_t wind_direction_seconds_count;
    uint32_t wind_direction_degrees;
    int32_t wind_direction_trend_point_x;
    int32_t wind_direction_trend_point_y;
    // Rainfall.
    volatile uint32_t rain_edge_count;
} SEN15901_context_t;

/*** SEN15901 local global variables ***/

// @formatter:off
static const int32_t SEN15901_WIND_DIRECTION_RATIO_THRESHOLD[SEN15901_WIND_DIRECTIONS_NUMBER] = {
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(688,   891),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(891,   1000),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(1000,  1410),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(1410,  2200),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(2200,  3140),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(3140,  3900),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(3900,  6570),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(6570,  8200),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(8200,  14120),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(14120, 16000),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(16000, 21880),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(21880, 33000),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(33000, 42120),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(42120, 64900),
    SEN15901_RESISTOR_DIVIDER_RATIO_THRESHOLD(64900, 120000),
    MATH_PERMILLE_MAX
};
// @formatter:on
static const int32_t SEN15901_WIND_DIRECTION_ANGLE_DEGREES[SEN15901_WIND_DIRECTIONS_NUMBER] = { 112, 67, 90, 157, 135, 202, 180, 22, 45, 247, 225, 337, 0, 292, 315, 270 };
static SEN15901_context_t sen15901_ctx;

/*** SEN15901 local functions ***/

/*******************************************************************/
static void _SEN15901_wind_speed_edge_callback(void) {
    // Wind speed.
    sen15901_ctx.wind_speed_edge_count++;
}

/*******************************************************************/
static void _SEN15901_rainfall_edge_callback(void) {
    // Increment edge count.
    sen15901_ctx.rain_edge_count++;
}

/*******************************************************************/
static void _SEN15901_tick_second_callback(void) {
    // Check enable flag.
    if (sen15901_ctx.wind_measurement_enable_flag != 0) {
        // Update local flags.
        sen15901_ctx.wind_speed_seconds_count++;
        sen15901_ctx.wind_direction_seconds_count++;
        sen15901_ctx.tick_second_flag = 1;
        // Ask for processing.
        if (sen15901_ctx.process_callback != NULL) {
            sen15901_ctx.process_callback();
        }
    }
}

/*** SEN15901 functions ***/

/*******************************************************************/
SEN15901_status_t SEN15901_init(SEN15901_process_cb_t process_callback) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    SEN15901_HW_configuration_t hw_config;
    // Reset data.
    SEN15901_reset_measurements();
    // Check parameter.
    if (process_callback == NULL) {
        status = SEN15901_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Init context.
    sen15901_ctx.wind_measurement_enable_flag = 0;
    sen15901_ctx.process_callback = process_callback;
    // Init hardware interface.
    hw_config.wind_speed_edge_irq_callback = &_SEN15901_wind_speed_edge_callback;
    hw_config.rainfall_edge_irq_callback = &_SEN15901_rainfall_edge_callback;
    hw_config.tick_second_irq_callback = &_SEN15901_tick_second_callback;
    status = SEN15901_HW_init(&hw_config);
    if (status != SEN15901_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
SEN15901_status_t SEN15901_de_init(void) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    // Release hardware interface.
    status = SEN15901_HW_de_init();
    if (status != SEN15901_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
SEN15901_status_t SEN15901_set_wind_measurement(uint8_t enable) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    // Update local enable flag.
    sen15901_ctx.wind_measurement_enable_flag = enable;
    // Check enable bit.
    if (enable == 0) {
        // Reset second counters.
        sen15901_ctx.wind_speed_seconds_count = 0;
        sen15901_ctx.wind_direction_seconds_count = 0;
        sen15901_ctx.tick_second_flag = 0;
    }
    // Set interrupt state.
    status = SEN15901_HW_set_wind_speed_interrupt(enable);
    if (status != SEN15901_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
SEN15901_status_t SEN15901_set_rainfall_measurement(uint8_t enable) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    // Set interrupt state.
    status = SEN15901_HW_set_rainfall_interrupt(enable);
    if (status != SEN15901_SUCCESS) goto errors;
errors:
    return status;
}

/*******************************************************************/
SEN15901_status_t SEN15901_process(void) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    int32_t wind_direction_ratio_permille = 0;
    uint8_t ratio_valid = 0;
    uint8_t idx = 0;
    // Check flag.
    if (sen15901_ctx.tick_second_flag == 0) goto errors;
    // Clear flag.
    sen15901_ctx.tick_second_flag = 0;
    // Update wind speed if period is reached.
    if (sen15901_ctx.wind_speed_seconds_count >= SEN15901_DRIVER_WIND_SPEED_SAMPLING_TIME_SECONDS) {
        // Reset seconds counter.
        sen15901_ctx.wind_speed_seconds_count = 0;
        // Compute new value.
        sen15901_ctx.wind_speed_mh = (sen15901_ctx.wind_speed_edge_count * SEN15901_WIND_SPEED_1HZ_TO_MH) / (SEN15901_DRIVER_WIND_SPEED_SAMPLING_TIME_SECONDS);
        sen15901_ctx.wind_speed_edge_count = 0;
        // Update peak value if required.
        if (sen15901_ctx.wind_speed_mh > sen15901_ctx.wind_speed_mh_peak) {
            sen15901_ctx.wind_speed_mh_peak = sen15901_ctx.wind_speed_mh;
        }
        // Update average value.
        MATH_rolling_mean(sen15901_ctx.wind_speed_mh_average, sen15901_ctx.wind_speed_data_count, sen15901_ctx.wind_speed_mh, uint32_t);
    }
    // Update wind direction if period is reached.
    if (sen15901_ctx.wind_direction_seconds_count >= SEN15901_DRIVER_WIND_DIRECTION_SAMPLING_PERIOD_SECONDS) {
        // Reset seconds counter.
        sen15901_ctx.wind_direction_seconds_count = 0;
        // Compute direction only if there is wind.
        if ((sen15901_ctx.wind_speed_mh / 1000) > 0) {
            // Turn external ADC on.
            status = SEN15901_HW_adc_get_wind_direction_ratio(&wind_direction_ratio_permille);
            if (status != SEN15901_SUCCESS) goto errors;
            // Convert ratio to direction.
            for (idx = 0; idx < SEN15901_WIND_DIRECTIONS_NUMBER; idx++) {
                if (wind_direction_ratio_permille <= SEN15901_WIND_DIRECTION_RATIO_THRESHOLD[idx]) {
                    // Update current angle and table index.
                    sen15901_ctx.wind_direction_degrees = SEN15901_WIND_DIRECTION_ANGLE_DEGREES[idx];
                    ratio_valid = 1;
                    break;
                }
            }
            if (ratio_valid == 0) {
                status = SEN15901_ERROR_RESISTOR_DIVIDER_RATIO;
                goto errors;
            }
            // Add new vector weighted by speed.
            sen15901_ctx.wind_direction_trend_point_x += ((int32_t) (sen15901_ctx.wind_speed_mh / 1000)) * ((int32_t) MATH_COS_TABLE[sen15901_ctx.wind_direction_degrees]);
            sen15901_ctx.wind_direction_trend_point_y += ((int32_t) (sen15901_ctx.wind_speed_mh / 1000)) * ((int32_t) MATH_SIN_TABLE[sen15901_ctx.wind_direction_degrees]);
        }
    }
errors:
    return status;
}

/*******************************************************************/
SEN15901_status_t SEN15901_get_wind_speed(int32_t* average_speed_mh, int32_t* peak_speed_mh) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    // Check parameters.
    if ((average_speed_mh == NULL) || (peak_speed_mh == NULL)) {
        status = SEN15901_ERROR_NULL_PARAMETER;
        goto errors;
    }
    (*average_speed_mh) = sen15901_ctx.wind_speed_mh_average;
    (*peak_speed_mh) = sen15901_ctx.wind_speed_mh_peak;
errors:
    return status;
}

/*******************************************************************/
SEN15901_status_t SEN15901_get_wind_direction(int32_t* average_direction_degrees, SEN15901_wind_direction_status_t* direction_status) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    MATH_status_t math_status = MATH_SUCCESS;
    // Check parameters.
    if ((average_direction_degrees == NULL) || (direction_status == NULL)) {
        status = SEN15901_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Reset output status.
    (*direction_status) = SEN15901_WIND_DIRECTION_STATUS_UNDEFINED;
    // Check trend point coordinates.
    if ((sen15901_ctx.wind_direction_trend_point_x != 0) || (sen15901_ctx.wind_direction_trend_point_y != 0)) {
        // Compute trend point angle.
        math_status = MATH_atan2(sen15901_ctx.wind_direction_trend_point_x, sen15901_ctx.wind_direction_trend_point_y, average_direction_degrees);
        MATH_exit_error(SEN15901_ERROR_BASE_MATH);
        // Update output status.
        (*direction_status) = SEN15901_WIND_DIRECTION_STATUS_AVAILABLE;
    }
errors:
    return status;
}

/*******************************************************************/
SEN15901_status_t SEN15901_get_rainfall(int32_t* rainfall_um) {
    // Local variables.
    SEN15901_status_t status = SEN15901_SUCCESS;
    // Check parameter.
    if (rainfall_um == NULL) {
        status = SEN15901_ERROR_NULL_PARAMETER;
        goto errors;
    }
    // Convert edge count to mm of rain.
    (*rainfall_um) = (sen15901_ctx.rain_edge_count * SEN15901_RAIN_EDGE_TO_UM);
errors:
    return status;
}

/*******************************************************************/
void SEN15901_reset_measurements(void) {
    // Wind speed.
    sen15901_ctx.wind_speed_seconds_count = 0;
    sen15901_ctx.wind_speed_edge_count = 0;
    sen15901_ctx.wind_speed_data_count = 0;
    sen15901_ctx.wind_speed_mh = 0;
    sen15901_ctx.wind_speed_mh_average = 0;
    sen15901_ctx.wind_speed_mh_peak = 0;
    // Wind direction.
    sen15901_ctx.wind_direction_seconds_count = 0;
    sen15901_ctx.wind_direction_degrees = 0;
    sen15901_ctx.wind_direction_trend_point_x = 0;
    sen15901_ctx.wind_direction_trend_point_y = 0;
    // Rainfall.
    sen15901_ctx.rain_edge_count = 0;
}

#endif /* SEN15901_DRIVER_DISABLE */
