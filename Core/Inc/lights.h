#ifndef INC_LIGHTLOWLEVELPRCEDURE_H_
#define INC_LIGHTLOWLEVELPRCEDURE_H_

#include "math.h"
#include "usart.h"
#include <stdio.h>
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

#define HIGH_LIGHT_THREESHOLD 200
#define LOW_LIGHT_THREESHOLD 100
#define voltageSource 3.3  // VCC
#define luxFactor = 500000
#define LEDUsed 10
#define USE_BRIGHTNESS 1 // 1 BRIGHTNESS ON; 0 BRIGHTNESS OFF
#define PI 3.14

void readNaturalLight(void);
void ws2812SendData (void);
void setBrightness (int value);
void setLed(int LEDnum, int color[3]);
void setLight(_Bool check);

typedef enum{Low, Med, High} lightThreeshold;

extern float voltage_value;

#endif // INC_LIGHTLOWLEVELPRCEDURE_H_
