/*
 * UI.c
 *
 *  Created on: 9 sie 2020
 *      Author: kowma
 */


#include "main.h"

#define HEATING_STATUS_STOP 0
#define HEATING_STATUS_RUN 1
#define HEATING_STATUS_ENDING 2
#define HEATING_STATUS_END 3




struct statusStruct
{
	uint8_t ds18b20_amount;
	uint8_t ntc_amount;
	uint8_t heating_status;
	uint16_t current_temperature;
	uint16_t set_temperature;
	uint16_t set_time;
	uint16_t currnt_time;
	uint8_t pc_connection_status;
}HC_status;




/* ui_handler_flag its busy flag its prevent
 * entering to ui_handler() few times with this same ms_counter value */



void ui_handler()
{
	if(ui_handler_flag == UI_HANDLER_FLAG_BUSY) return;

	//red led control 

	if( (HC_status.ds18b20_amount == 0) && (HC_status.ntc_amount == 0))
	{
		if(ms_counter % 200 == 0)	LED_RED_GPIO_Port -> ODR ^= LED_RED_Pin;
	}
	else if(HC_status.ds18b20_amount == 0 || (HC_status.ntc_amount == 0))
	{
		if(ms_counter % 500 == 0) LED_RED_GPIO_Port -> ODR ^= LED_RED_Pin;
	}

	// green led control 

	if(HC_status.heating_status == HEATING_STATUS_RUN)
	{
		if((ms_counter/10)%20 == 0)
		{
			LED_GREEN_GPIO_Port->ODR ^=  LED_GREEN_Pin;
		}
	}
	else if(HC_status.heating_status == HEATING_STATUS_STOP)
	{
		if(ms_counter % 499 == 0)
		{
			LED_GREEN_GPIO_Port->BRR = LED_GREEN_Pin;
		}
	}
	else if(HC_status.heating_status == HEATING_STATUS_ENDING)
	{
		if( (ms_counter % 200) == 0 )
		{
			LED_GREEN_GPIO_Port->ODR ^= LED_GREEN_Pin;
		}
	}
	else if(HC_status.heating_status == HEATING_STATUS_END)
	{
		if( ( ms_counter % 90 ) == 0)
		{
			LED_GREEN_GPIO_Port->BSRR = LED_GREEN_Pin;
		}
	}

	// buzzer contorl

	ui_handler_flag = UI_HANDLER_FLAG_BUSY;
}

void ui_change_ds18B20_status(uint8_t status)
{
	HC_status.ds18b20_amount = status;
}

void ui_change_ntc_status(uint8_t status)
{
	HC_status.ds18b20_amount = status;
}

void ui_change_heating_status(uint8_t status)
{
	HC_status.ds18b20_amount = status;
}


/***
 * 	function is calling in SysTick IRQ Handler
 */


uint8_t ui_initalize(void)
{
	SysTick_Config(72000);
}






inline void UI_led_red_on()
{
	LED_RED_GPIO_Port->ODR |= LED_RED_Pin;
}
inline void UI_led_red_off()
{
	LED_RED_GPIO_Port->ODR &= ~LED_RED_Pin;
}
inline void UI_green_led_on()
{
	LED_GREEN_GPIO_Port->ODR |= LED_GREEN_Pin;
}
inline void UI_green_led_off()
{
	LED_GREEN_GPIO_Port->ODR &= ~LED_GREEN_Pin;
}
inline void UI_buzz_on()
{
	BUZZER_GPIO_Port->ODR |= BUZZER_Pin;
}
inline void UI_buzz_off()
{
	BUZZER_GPIO_Port->ODR &= ~BUZZER_Pin;
}

void UI_leds_check()
{
	UI_led_red_on();
	simple_delay_ms(500);
	UI_green_led_on();
	simple_delay_ms(500);
	UI_led_red_off();
	simple_delay_ms(500);
	UI_green_led_off();
	simple_delay_ms(500);
	UI_green_led_on();
	simple_delay_ms(500);
	UI_led_red_on();
	simple_delay_ms(500);
	UI_green_led_off();
	simple_delay_ms(500);
	UI_led_red_off();
}

void UI_buzz_check()
{
	UI_buzz_on();
	simple_delay_ms(500);
	UI_buzz_off();
	simple_delay_ms(500);
	UI_buzz_on();
	simple_delay_ms(500);
	UI_buzz_off();
}
