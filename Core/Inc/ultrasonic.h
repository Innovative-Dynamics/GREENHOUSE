/*
 * ultrasonic.h
 *
 *  Created on: May 29, 2023
 *      Author: Innovative Dynamics
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "gpio.h"
#include "stdio.h"
#include "tim.h"
#include "virtual_main.h"

void echo_callback(void);
void setup_pwm(void);

#endif /* INC_ULTRASONIC_H_ */
