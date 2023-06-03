/*
 * ecph.h
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#ifndef INC_ECPH_H_
#define INC_ECPH_H_

#include "stdio.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include <string.h>
#include "utils.h"

#define K1 1 //Coeff. calibrazione sonda EC
#define R1 500 // Valore resistenza sonda EC
#define Vin 3.3
#define ph_interval 0.2357 // prima era 0.35 N.B il pin legge max 3.3v non 5v

_Bool is_ec_initialized ();
void reset_ec_initialized ();

//_Bool read_ec ();
//void set_adEC (float adEC_val);
_Bool is_ec_value_readed ();
void reset_is_ec_value_readed ();


//_Bool read_ph ();
//void set_adPH (float adPH_val);
_Bool is_ph_value_readed ();
void reset_is_ph_value_readed ();

float get_EC ();
float get_PH ();

void ph_read (ADC_HandleTypeDef* hadc);
void ec_read (ADC_HandleTypeDef* hadc);
void ec_init ();

void set_ec_channel ();
void set_ph_channel ();

#endif /* INC_ECPH_H_ */
