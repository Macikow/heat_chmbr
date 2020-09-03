/*
 * delay.c
 *
 *  Created on: 5 maj 2020
 *      Author: kowma
 */

#include "delay.h"
#include "main.h"

extern volatile uint8_t delay_handler;
extern TIM_HandleTypeDef htim4;




void simple_delay_ms(const uint16_t ms)
{
  uint32_t i = ms * 7200;
  while (i-- > 0) {
    asm("nop");
  }
}

void simple_delay_us(const uint16_t us)
{
  uint16_t i = us * 7;
  while (i-- > 0) {
    asm("nop");
  }
}

//void delay_us(uint16_t us_delay)
//{
//	delay_handler = 1;
//	TIM4->CNT = us_delay;
//	TIM4->ARR = 72-1;
//	TIM4->PSC = us_delay-1;
//	HAL_TIM_Base_Start_IT(&htim4);
//	while(delay_handler);
//}
//
//void delay_ms(uint16_t ms_delay)
//{
//	delay_handler = 1;
//	TIM4->CNT = ms_delay;
//	TIM4->ARR = 7200-1;
//	TIM4->PSC = ms_delay*10-1;
//	HAL_TIM_Base_Start_IT(&htim4);
//	while(delay_handler);
//	//HAL_TIM_Base_Stop_IT(&htim4);
//}

//void delay_us(uint16_t us_delay)
//{
//	HAL_TIM_Base_Start_IT(&htim4);
//	while(us_delay--)
//	{
//		while(delay_handler)
//		delay_handler = 1;
//	}
//
//	HAL_TIM_Base_Stop_IT(&htim4);
//}
