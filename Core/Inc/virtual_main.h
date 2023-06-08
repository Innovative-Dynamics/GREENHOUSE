/*
 * virtual_,ain.h
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#ifndef INC_VIRTUAL_MAIN_H_
#define INC_VIRTUAL_MAIN_H_


// INCLUDES
#include "utils.h"
#include "gpio.h"
#include <stdio.h>
//#include "adc.h"
#include "ultrasonic.h"
#include "stepper.h"
//#include "dht11.h"
#include "usart.h"
//#include "water_level_sensor.h"
#include <string.h>
#include "lights.h"
#include "ecph.h"
#include "dht11.h"


// DEFINES
// Water Level Defines
#define MIN_WATER_LVL 8 // Double of the height of the pump so it remains always submerged
#define MAX_WATER_LVL 25

// ECPH Defines
#define EC_LOWER_BOUND 1
#define EC_UPPER_BOUND 1.5
#define EC_SETPOINT 1.3
#define PH_SETPOINT 5.5

// WATER GROW TANK LEVEL
#define ROOT_LEVEL 195
#define MIN_GT_WATER_LEVEL 100


// STATE MACHINE
typedef enum{WATER_LOAD_PROCEDURE,
			WATER_STATUS_CHECK,
			WATER_UNLOAD_PROCEDURE,
			ECPH_PROCEDURE,
			MIX_PROCEDURE,
			DHT11_PROCEDURE,
			DATASEND_PROCEDURE,
			LIGHTS_PROCEDURE,
			WATER_TANK_CHECK,
			HOME,
			INIT,
			TEST,
			WATER_GROW_TANK_CHECK} machine;


// FUNCTIONS
void virtual_main();

// VARIABLES
extern float water_level;
extern uint32_t water_level_gt;

extern _Bool nutrs_deployed;

#endif /* INC_VIRTUAL_MAIN_H_ */
