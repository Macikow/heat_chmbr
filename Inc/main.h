/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "EEPROM.h"
#include "HD44780.h"
#include "ds18b20.h"
#include "delay.h"
#include "UI.h"
#include "ntc.h"
#include "time_counter.h"
#include "PWM_CTRL.h"
#include "PID.h"
#include "servis.h"
#include <stdlib.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
TIM_HandleTypeDef htim1;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma1;
UART_HandleTypeDef huart3;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define ADC_CHANNELS 4
 
uint16_t adc_value[ADC_CHANNELS];
void increment_ms_counter(void);
/* counts ms from 0 to 999  - its 1s overflow*/
volatile uint16_t ms_counter;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUTTON_DOWN_Pin GPIO_PIN_13
#define BUTTON_DOWN_GPIO_Port GPIOC
#define BUTTON_DOWN_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_EXIT_Pin GPIO_PIN_14
#define BUTTON_EXIT_GPIO_Port GPIOC
#define BUTTON_EXIT_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_OK_Pin GPIO_PIN_15
#define BUTTON_OK_GPIO_Port GPIOC
#define BUTTON_OK_EXTI_IRQn EXTI15_10_IRQn
#define ONE_WIRE_DATA1_Pin GPIO_PIN_3
#define ONE_WIRE_DATA1_GPIO_Port GPIOA
#define ONE_WIRE_DATA2_Pin GPIO_PIN_4
#define ONE_WIRE_DATA2_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_6
#define LED_GREEN_GPIO_Port GPIOA
#define ZCD_Pin GPIO_PIN_7
#define ZCD_GPIO_Port GPIOA
#define ZCD_EXTI_IRQn EXTI9_5_IRQn
#define LCD_D0_Pin GPIO_PIN_0
#define LCD_D0_GPIO_Port GPIOB
#define LCD_D1_Pin GPIO_PIN_1
#define LCD_D1_GPIO_Port GPIOB
#define LCD_D1B2_Pin GPIO_PIN_2
#define LCD_D1B2_GPIO_Port GPIOB
#define BUTTON_UP_Pin GPIO_PIN_12
#define BUTTON_UP_GPIO_Port GPIOB
#define BUTTON_UP_EXTI_IRQn EXTI15_10_IRQn
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define FAN_CTRL_Pin GPIO_PIN_9
#define FAN_CTRL_GPIO_Port GPIOA
#define HEATER_CTRL_Pin GPIO_PIN_10
#define HEATER_CTRL_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_15
#define LCD_RS_GPIO_Port GPIOA
#define LCD_D3_Pin GPIO_PIN_3
#define LCD_D3_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOB
#define LCD_E_Pin GPIO_PIN_8
#define LCD_E_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_9
#define LCD_RW_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
