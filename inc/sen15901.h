/*
 * sen15901.h
 *
 *  Created on: 07 sep. 2024
 *      Author: Ludo
 */

#ifndef __SEN15901_H__
#define __SEN15901_H__

#ifndef SEN15901_DRIVER_DISABLE_FLAGS_FILE
#include "sen15901_driver_flags.h"
#endif
#include "math.h"
#include "types.h"

/*** SEN15901 structures ***/

/*!******************************************************************
 * \enum SEN15901_status_t
 * \brief SEN15901 driver error codes.
 *******************************************************************/
typedef enum {
	// Driver errors.
	SEN15901_SUCCESS = 0,
	SEN15901_ERROR_NULL_PARAMETER,
	// Low level drivers errors.
	SEN15901_ERROR_BASE_GPIO = 0x0100,
	SEN15901_ERROR_BASE_TIMER = (SEN15901_ERROR_BASE_GPIO + SEN15901_DRIVER_GPIO_ERROR_BASE_LAST),
	SEN15901_ERROR_BASE_ADC = (SEN15901_ERROR_BASE_TIMER + SEN15901_DRIVER_TIMER_ERROR_BASE_LAST),
	SEN15901_ERROR_BASE_MATH = (SEN15901_ERROR_BASE_ADC + SEN15901_DRIVER_ADC_ERROR_BASE_LAST),
	// Last base value.
	SEN15901_ERROR_BASE_LAST = (SEN15901_ERROR_BASE_MATH + MATH_ERROR_BASE_LAST)
} SEN15901_status_t;

#ifndef SEN15901_DRIVER_DISABLE

/*!******************************************************************
 * \enum SEN15901_wind_direction_status_t
 * \brief SEN15901 driver wind direction data status.
 *******************************************************************/
typedef enum {
	SEN15901_WIND_DIRECTION_STATUS_AVAILABLE = 0,
	SEN15901_WIND_DIRECTION_STATUS_UNDEFINED,
	SEN15901_WIND_DIRECTION_STATUS_LAST
} SEN15901_wind_direction_status_t;

/*!******************************************************************
 * \fn SEN15901_process_cb_t
 * \brief SEN15901 driver process callback.
 *******************************************************************/
typedef void (*SEN15901_process_cb_t)(void);

/*** SEN15901 functions ***/

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_init(void)
 * \brief Init SEN15901 driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_init(void);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_de_init(void)
 * \brief Release SEN15901 driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_de_init(void);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_start_continuous_measure(SEN15901_process_cb_t process_callback)
 * \brief Start wind and rainfall measurements.
 * \param[in]  	process_callback: Function which will be called when the SEN15901 driver has to be processed.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
SEN15901_status_t SEN15901_start_measurements(SEN15901_process_cb_t process_callback);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_stop_continuous_measure(void)
 * \brief Stop wind and rainfall measurements.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
SEN15901_status_t SEN15901_stop_measurements(void);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_process(void)
 * \brief SEN15901 driver process function.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_process(void);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_get_wind_speed(int32_t* average_speed_mh, int32_t* peak_speed_mh)
 * \brief Read wind speeds.
 * \param[in]  	none
 * \param[out] 	average_speed_mh: Pointer to integer that will contain the average wind speed since last reset in m/h.
 * \param[out] 	peak_speed_mh: Pointer to integer that will contain the peak wind speed since last reset in m/h.
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_get_wind_speed(int32_t* average_speed_mh, int32_t* peak_speed_mh);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_get_wind_direction(int32_t* average_direction_degrees, SEN15901_wind_direction_status_t* direction_status)
 * \brief Read wind average direction.
 * \param[in]  	none
 * \param[out] 	average_direction_degrees: Pointer to integer that will contain the average wind direction since last reset in degrees.
 * \param[out]	direction_status: Status of the output data.
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_get_wind_direction(int32_t* average_direction_degrees, SEN15901_wind_direction_status_t* direction_status);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_get_rainfall(int32_t* rainfall_um)
 * \brief Read rainfall.
 * \param[in]  	none
 * \param[out] 	rainfall_mm: Pointer to integer that will contain the rainfall count in mm.
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_get_rainfall(int32_t* rainfall_um);

/*!******************************************************************
 * \fn void SEN15901_reset_measurements(void);
 * \brief Reset wind and rainfall measurements.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SEN15901_reset_measurements(void);

/*******************************************************************/
#define SEN15901_exit_error(base) { ERROR_check_exit(sen15901_status, SEN15901_SUCCESS, base) }

/*******************************************************************/
#define SEN15901_stack_error(base) { ERROR_check_stack(sen15901_status, SEN15901_SUCCESS, base) }

/*******************************************************************/
#define SEN15901_stack_exit_error(base, code) { ERROR_check_stack_exit(sen15901_status, SEN15901_SUCCESS, base, code) }

#endif /* SEN15901_DRIVER_DISABLE */

#endif /* __SEN15901_H__ */
