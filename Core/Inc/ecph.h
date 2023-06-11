/*
 * ecph.h
 *
 *  Created on: May 29, 2023
 *      Author: Innovative Dymanics
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

#define K1 0.8 // EC Calibration parameter
#define R1 500 // EC Resistance
#define Vin 3.3
#define ph_interval 0.2357
#define EC_CHANNEL ADC_CHANNEL_2
#define PH_CHANNEL ADC_CHANNEL_3

void ec_init ();
void ec_read (ADC_HandleTypeDef*);
void ph_read (ADC_HandleTypeDef*);
void set_ec_channel ();
void set_ph_channel ();

#endif /* INC_ECPH_H_ */
