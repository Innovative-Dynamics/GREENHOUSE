/*
 * stepper.h
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "gpio.h"
#include "utils.h"
#include "string.h"
#include "adc.h"
#include "tim.h"
#include "stdio.h"

#define stepsperrev 4096
#define anglepersequence 0.703125 // 360 = 512 sequences

void stepper_set_rpm(int);  // Set rpm--> max 13, min 1,,,  went to 14 rev/min
void stepper_half_drive(int);
void stepper_step_angle(int, int);

_Bool are_nutrs_deployed ();
void reset_are_nutrs_deployed ();

// VARIABLES
extern _Bool nutrs_deployed;

#endif /* INC_STEPPER_H_ */
