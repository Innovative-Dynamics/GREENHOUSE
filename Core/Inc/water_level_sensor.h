/*
 * water_level_sensor.h
 *
 *  Created on: Jun 3, 2023
 *      Author: envy0
 */

#ifndef INC_WATER_LEVEL_SENSOR_H_
#define INC_WATER_LEVEL_SENSOR_H_

_Bool get_water_level ();
void readLevelPolling();
void convertLevel(float);
void readLevelInterrupt();

#endif /* INC_WATER_LEVEL_SENSOR_H_ */
