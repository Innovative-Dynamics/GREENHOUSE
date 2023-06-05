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
void echo_callback(void);
void setup_pwm(void);

// VARIABLES
extern float water_level;

#endif /* INC_ULTRASONIC_H_ */
