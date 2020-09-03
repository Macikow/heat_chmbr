/*
 * PWM_CTRL.c
 *
 *  Created on: 3 wrz 2020
 *      Author: kowma
 */


#include "main.h"



void pwmctrl_enable_timer_irq(void)
{
	__HAL_TIM_ENABLE_IT(&htim1,TIM_IT_CC1);
	__HAL_TIM_ENABLE_IT(&htim1,TIM_IT_UPDATE);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 9799);
	HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
	HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
	// ZCD start TIM1 find in *_it.c
}
