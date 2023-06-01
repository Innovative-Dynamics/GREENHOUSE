///*
// * lights.h
// *
// *  Created on: May 31, 2023
// *      Author: envy0
// */
//
//#ifndef INC_LIGHTS_H_
//#define INC_LIGHTS_H_
//
//#include "gpio.h"
//#include "dma.h"
//#include "math.h"
//#include "tim.h"
//#include "adc.h"
//#include "stdio.h"
//#include "utils.h"
//
//#define HIGH_LIGHT_THREESHOLD 200
//#define LOW_LIGHT_THREESHOLD 100
//#define voltageSource 3.3  //Voltage divider VCC ref
//#define luxFactor = 500000
//#define LEDUsed 10
//#define USE_BRIGHTNESS 1 //1 if you want to control brightness, 0 if not
//#define PI 3.14
//
//void read_natural_light(ADC_HandleTypeDef* hadc);
//void setLed (int LEDnum, int color[3]);
//void setBrightness (int brightness);
//void ws2812SendData (TIM_HandleTypeDef* htim);
//void set_lights (TIM_HandleTypeDef* htim, ADC_HandleTypeDef* hadc, _Bool status);
//
//#endif /* INC_LIGHTS_H_ */

/*
 * lights.h
 *
 *  Created on: May 22, 2023
 *      Author: envy0
 */

//#ifndef INC_LIGHTS_H_
//#define INC_LIGHTS_H_
//
//#include "gpio.h"
//#include "dma.h"
//#include "math.h"
//#include "tim.h"
//#include "adc.h"
//#include "stdio.h"
//
//typedef enum{Low, Med, High} lightThreeshold; // <30%, da 30 a 70, >70%
//
//#define HIGH_LIGHT_THREESHOLD 200
//#define LOW_LIGHT_THREESHOLD 100
//#define voltageSource 3.3  //Voltage divider VCC ref
//#define luxFactor = 500000
//#define LEDUsed 10
//#define USE_BRIGHTNESS 1 //1 if you want to control brightness, 0 if not
//#define PI 3.14
//
//// Violet 400nm light #8300b5 ---> (131, 0, 181)
//// Red 680nm light ---> (255, 0, 0)
//// whiteBlue 460nm light #007bff --> (0, 123, 255)
//
//void readNaturalLight(ADC_HandleTypeDef* hadc);
//void ws2812SendData(TIM_HandleTypeDef* htim);
//void setBrightness(int value);
//void setLed(int LEDnum, int color[3]); //da sostituire con write led
//void setLight(TIM_HandleTypeDef* htim, int check);
//
//#endif /* INC_LIGHTS_H_ */



#ifndef INC_LIGHTLOWLEVELPRCEDURE_H_
#define INC_LIGHTLOWLEVELPRCEDURE_H_

//Photoresistor-----------------------------------------------------------------

typedef enum{Low, Med, High} lightThreeshold; // <30%, da 30 a 70, >70%

void readNaturalLight(void);
void ws2812SendData (void);
void setBrightness (int value);
void setLed(int LEDnum, int color[3]); //da sostituire con write led
void setLight(int check);

#endif // INC_LIGHTLOWLEVELPRCEDURE_H_
