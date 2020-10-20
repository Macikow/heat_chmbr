/*
 * servis.c
 *
 *  Created on: 14 paź 2020
 *      Author: kowma
 */
#include "main.h"

void servis_uart_send_ntc(uint16_t ms_run_time, uint8_t sensor_no)
{
	static uint8_t ntc_string[]={'\0','\0','\0','\0','\0','\0','\0','\0','\0'};
	uint8_t char_counter=0;

	if(ms_run_time != ms_counter) return;

	ntc_convert_float_to_string(&ntc_string[0],  ntc_get_analog_sensors_value(sensor_no));
	while(ntc_string[char_counter] != '\0' && ntc_string[char_counter] != ' ')
	{
		char_counter++;
	}
	ntc_string[char_counter++] =  '\n';
	ntc_string[char_counter++] =  '\r';
	HAL_UART_Transmit_IT(&huart3, &ntc_string[0], char_counter );
}
