/*
 * ecph.h
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#ifndef INC_ECPH_H_
#define INC_ECPH_H_

// INCLUDES
#include "stdio.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include <string.h>
#include "utils.h"

// DEFINES
#define K1 1 //Coeff. calibrazione sonda EC
#define R1 500 // Valore resistenza sonda EC
#define Vin 3.3
#define ph_interval 0.2357 // prima era 0.35 N.B il pin legge max 3.3v non 5v
#define EC_CHANNEL ADC_CHANNEL_2
#define PH_CHANNEL ADC_CHANNEL_3

// FUNCTIONS
//_Bool is_ec_initialized ();
//void reset_ec_initialized ();
//
//_Bool is_ec_value_readed ();
//void reset_is_ec_value_readed ();
//
//_Bool is_ph_value_readed ();
//void reset_is_ph_value_readed ();

//float get_EC ();
//float get_PH ();

void ph_read (ADC_HandleTypeDef*);
void ec_read (ADC_HandleTypeDef*);
void ec_init ();

void set_ec_channel ();
void set_ph_channel ();

// VARIABLES

// EC METER VARS.
extern float EC;

// PH METER VARS.
extern uint8_t PH;
//double somma = 0;
//double media = 0;

//_Bool reading_ec = 0;
//_Bool reading_ph = 0;

//_Bool is_reading = 0;

extern _Bool ec_initialized;

//_Bool ec_value_readed = 0;
//
//_Bool ph_value_readed = 0;

#endif /* INC_ECPH_H_ */
