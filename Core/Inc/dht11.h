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
#include "utils.h"

uint8_t DHT11_Start (void);
//_Bool DHT11_Check_Response ();
uint8_t DHT11_Read (void);

extern _Bool dht11_init;

#endif /* INC_DHT11_H_ */
