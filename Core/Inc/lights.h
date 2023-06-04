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
#define voltageSource 3.3  //Voltage divider VCC ref
#define luxFactor = 500000
#define LEDUsed 10
#define USE_BRIGHTNESS 1 //1 if you want to control brightness, 0 if not
#define PI 3.14

typedef enum{Low, Med, High} lightThreeshold; // <30%, da 30 a 70, >70%

void readNaturalLight(void);
void ws2812SendData (void);
void setBrightness (int value);
void setLed(int LEDnum, int color[3]); //da sostituire con write led
void setLight(int check);

// VARIABLES
extern int violetColor400[3];
extern int redColor680[3];
extern int whiteBlueColor460[3];
extern int nullColor[3];
extern int dataSentFlag;

extern float B;
extern float m;
extern volatile uint32_t voltage_value;

#endif // INC_LIGHTLOWLEVELPRCEDURE_H_
