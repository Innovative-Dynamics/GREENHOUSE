#ifndef INC_LIGHTLOWLEVELPRCEDURE_H_
#define INC_LIGHTLOWLEVELPRCEDURE_H_

#include "math.h"
#include "usart.h"
#include <stdio.h>
#include "utils.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

#define HIGH_LIGHT_THREESHOLD 200
#define LOW_LIGHT_THREESHOLD 100
#define voltageSource 3.3  //Voltage divider VCC ref
#define luxFactor = 500000

typedef enum{Low, Med, High} lightThreeshold; // <30%, da 30 a 70, >70%

void readNaturalLight(void);
void ws2812SendData (void);
void setBrightness (int value);
void setLed(int LEDnum, int color[3]); //da sostituire con write led
void setLight(int check);

#endif // INC_LIGHTLOWLEVELPRCEDURE_H_
