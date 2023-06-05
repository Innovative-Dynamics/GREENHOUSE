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


// DEFINES
// Water Level Defines
#define MIN_WATER_LVL 8 // Double of the height of the pump so it remains always submerged
#define MAX_WATER_LVL 18

// ECPH Defines
#define EC_SETPOINT 1

// WATER GROW TANK LEVEL
#define ROOT_LEVEL 124


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
extern _Bool enough_water;

extern volatile uint32_t water_level_gt;

extern _Bool ecph_initialization_called;
extern _Bool ecph_ready;

extern _Bool is_default_check; // TRUE == 1; FALSE == 0

extern _Bool loading;
extern _Bool unloading;

extern _Bool reading_value;
extern _Bool is_reading_adc;

//extern _Bool is_counting;

extern _Bool dht11_initialization;
extern _Bool water_status_initialized;

//extern uint32_t time_prev;
//extern long long int water_time_prev;

extern float temp;
extern float humidity;

extern float EC_Value;
extern float PH_Value;

extern char data[8];

#endif /* INC_VIRTUAL_MAIN_H_ */
