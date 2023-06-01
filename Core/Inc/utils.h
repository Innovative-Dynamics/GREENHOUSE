/*
 * utils.h
 *
 *  Created on: Jun 1, 2023
 *      Author: envy0
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "stdio.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "ecph.h"

_Bool water_reading_done ();
float get_water_grow_tank_level ();
void reset_water_reading_done ();

_Bool lights_reading_done ();
void reset_lights_reading_done ();
float get_lights_voltage_value ();

#endif /* INC_UTILS_H_ */
