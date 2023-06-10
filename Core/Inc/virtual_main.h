/*
 * virtual_,ain.h
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#ifndef INC_VIRTUAL_MAIN_H_
#define INC_VIRTUAL_MAIN_H_

#include "utils.h"
#include "gpio.h"
#include <stdio.h>
#include "ultrasonic.h"
#include "stepper.h"
#include "usart.h"
#include <string.h>
#include "lights.h"
#include "ecph.h"
#include "dht11.h"

#define MIN_WATER_LVL 8
#define MAX_WATER_LVL 25
#define EC_LOWER_BOUND 1
#define EC_UPPER_BOUND 1.5
#define EC_SETPOINT 1.3
#define PH_SETPOINT 5.5
#define WATER_LOAD_TIME 2e+7
#define WATER_UNLOAD_TIME 1e+7
#define LIGHTS_SWITCH_TIME 12e+7
#define SEND_DATA_TIME 6e+7
#define ROOT_LEVEL 195
#define MIN_GT_WATER_LEVEL 100

typedef enum{WATER_LOAD_PROCEDURE,
			WATER_UNLOAD_PROCEDURE,
			ECPH_PROCEDURE,
			MIX_PROCEDURE,
			DHT11_PROCEDURE,
			HOME,
			INIT,
			TEST} machine;

void virtual_main();

extern uint32_t water_level_gt;
extern float water_level;
extern _Bool nutrs_deployed;

#endif /* INC_VIRTUAL_MAIN_H_ */
