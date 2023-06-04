/*
 * ultrasonic.h
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

// INCLUDES
#include "gpio.h"
#include "stdio.h"
#include "tim.h"
#include "virtual_main.h"

// FUNCTIONS
void trigger_measurement();
void reset_trigger();
void start_counting();
_Bool check_counting_elapsed(uint32_t);
void read_value1();
void read_value2();
void read_water_level ();

// VARIABLES
extern uint16_t water_level;
extern _Bool water_level_readed;

#endif /* INC_ULTRASONIC_H_ */
