/*
 * sen15901_hw.c
 *
 *  Created on: 07 sep. 2024
 *      Author: Ludo
 */

#include "sen15901_hw.h"

#ifndef SEN15901_DRIVER_DISABLE_FLAGS_FILE
#include "sen15901_driver_flags.h"
#endif
#include "sen15901.h"
#include "types.h"

#ifndef SEN15901_DRIVER_DISABLE

/*** SEN15901 HW functions ***/

/*******************************************************************/
SEN15901_status_t __attribute__((weak)) SEN15901_HW_init(SEN15901_HW_configuration_t* configuration) {
	// Local variables.
	SEN15901_status_t status = SEN15901_SUCCESS;
	/* To be implemented */
	UNUSED(configuration);
	return status;
}

/*******************************************************************/
SEN15901_status_t __attribute__((weak)) SEN15901_HW_de_init(void) {
	// Local variables.
	SEN15901_status_t status = SEN15901_SUCCESS;
	/* To be implemented */
	return status;
}

/*******************************************************************/
SEN15901_status_t __attribute__((weak)) SEN15901_HW_set_wind_speed_interrupt(uint8_t enable) {
	// Local variables.
	SEN15901_status_t status = SEN15901_SUCCESS;
	/* To be implemented */
	UNUSED(enable);
	return status;
}

/*******************************************************************/
SEN15901_status_t __attribute__((weak)) SEN15901_HW_set_rainfall_interrupt(uint8_t enable) {
	// Local variables.
	SEN15901_status_t status = SEN15901_SUCCESS;
	/* To be implemented */
	UNUSED(enable);
	return status;
}

/*******************************************************************/
SEN15901_status_t __attribute__((weak)) SEN15901_HW_adc_get_wind_direction_ratio(int32_t* wind_direction_ratio_permille) {
	// Local variables.
	SEN15901_status_t status = SEN15901_SUCCESS;
	/* To be implemented */
	UNUSED(wind_direction_ratio_permille);
	return status;
}

#endif /* SEN15901_DRIVER_DISABLE */
