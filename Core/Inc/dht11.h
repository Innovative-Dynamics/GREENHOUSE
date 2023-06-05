/*
 * dht11.h
 *
 *  Created on: Jun 5, 2023
 *      Author: envy0
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "gpio.h"
#include "stdio.h"
#include "tim.h"

void dht11_start();

extern _Bool dht11_init;

#endif /* INC_DHT11_H_ */
