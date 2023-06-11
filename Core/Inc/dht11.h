/*
 * dht11.h
 *
 *  Created on: Jun 5, 2023
 *      Author: Innovative Dymanics
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "gpio.h"
#include "stdio.h"
#include "tim.h"
#include "utils.h"

uint8_t DHT11_Start (void);
uint8_t DHT11_Read (void);

#endif /* INC_DHT11_H_ */
