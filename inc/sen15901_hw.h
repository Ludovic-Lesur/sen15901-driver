/*
 * sen15901_hw.h
 *
 *  Created on: 07 sep. 2024
 *      Author: Ludo
 */

#ifndef __SEN15901_HW_H__
#define __SEN15901_HW_H__

#ifndef SEN15901_DRIVER_DISABLE_FLAGS_FILE
#include "sen15901_driver_flags.h"
#endif
#include "sen15901.h"
#include "types.h"

#ifndef SEN15901_DRIVER_DISABLE

/*** SEN15901 HW structures ***/

/*!******************************************************************
 * \fn SEN15901_HW_gpio_edge_irq_cb_t
 * \brief GPIO edge interrupt callback.
 *******************************************************************/
typedef void (*SEN15901_HW_gpio_edge_irq_cb_t)(void);

/*!******************************************************************
 * \fn SEN15901_HW_tick_second_irq_cb_t
 * \brief 1 second timer interrupt callback.
 *******************************************************************/
typedef void (*SEN15901_HW_tick_second_irq_cb_t)(void);

/*!******************************************************************
 * \struct SEN15901_HW_configuration_t
 * \brief SEN15901 hardware interface parameters.
 *******************************************************************/
typedef struct {
	SEN15901_HW_gpio_edge_irq_cb_t wind_speed_edge_irq_callback;
	SEN15901_HW_gpio_edge_irq_cb_t rainfall_edge_irq_callback;
	SEN15901_HW_tick_second_irq_cb_t tick_second_irq_callback;
} SEN15901_HW_configuration_t;

/*** SEN15901 HW functions ***/

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_HW_init(SEN15901_HW_configuration_t* configuration)
 * \brief Init SEN15901 hardware interface.
 * \param[in]  	configuration: Pointer to the hardware interface parameters structure.
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_HW_init(SEN15901_HW_configuration_t* configuration);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_HW_de_init(void)
 * \brief Release SEN15901 hardware interface.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_HW_de_init(void);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_HW_start_measurements(void)
 * \brief Start continuous measurements.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_HW_start_measurements(void);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_HW_stop_measurements(void)
 * \brief Stop continuous measurements.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_HW_stop_measurements(void);

/*!******************************************************************
 * \fn SEN15901_status_t SEN15901_HW_adc_get_wind_direction_ratio(int32_t* wind_direction_ratio_permille)
 * \brief Read wind direction analog input ratio.
 * \param[in]  	none
 * \param[out] 	wind_direction_ratio_permille: Wind direction analog input ratio in per-mille.
 * \retval		Function execution status.
 *******************************************************************/
SEN15901_status_t SEN15901_HW_adc_get_wind_direction_ratio(int32_t* wind_direction_ratio_permille);

#endif /* SEN15901_DRIVER_DISABLE */

#endif /* __SEN15901_HW_H__ */
