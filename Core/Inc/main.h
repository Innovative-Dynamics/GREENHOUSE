/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define WATER_LEVEL_Pin GPIO_PIN_0
#define WATER_LEVEL_GPIO_Port GPIOC
#define EC_READ_Pin GPIO_PIN_1
#define EC_READ_GPIO_Port GPIOC
#define PH_READ_Pin GPIO_PIN_2
#define PH_READ_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define EC_WRITE_Pin GPIO_PIN_7
#define EC_WRITE_GPIO_Port GPIOA
#define PHOTORESISTOR_Pin GPIO_PIN_0
#define PHOTORESISTOR_GPIO_Port GPIOB
#define STEP_IN1_Pin GPIO_PIN_1
#define STEP_IN1_GPIO_Port GPIOB
#define MIX_PUMP_Pin GPIO_PIN_2
#define MIX_PUMP_GPIO_Port GPIOB
#define STEP_IN4_Pin GPIO_PIN_13
#define STEP_IN4_GPIO_Port GPIOB
#define STEP_IN3_Pin GPIO_PIN_14
#define STEP_IN3_GPIO_Port GPIOB
#define STEP_IN2_Pin GPIO_PIN_15
#define STEP_IN2_GPIO_Port GPIOB
#define LIGHTS_PWM_Pin GPIO_PIN_8
#define LIGHTS_PWM_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define ULTRAS__TRIG_Pin GPIO_PIN_4
#define ULTRAS__TRIG_GPIO_Port GPIOB
#define ULTRAS__ECHO_Pin GPIO_PIN_5
#define ULTRAS__ECHO_GPIO_Port GPIOB
#define ELECTROVALVE_Pin GPIO_PIN_6
#define ELECTROVALVE_GPIO_Port GPIOB
#define WATER_PUMP_Pin GPIO_PIN_7
#define WATER_PUMP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
