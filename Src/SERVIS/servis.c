/*
 * servis.c
 *
 *  Created on: 14 paź 2020
 *      Author: kowma
 */
#include "main.h"


// data packet
struct __attribute__((__packed__)) packet_s
{
	uint8_t dev_name[20];
	uint32_t uc_id;
	struct temp_data
	{
		uint16_t temp_sensor_ntc[3]; // format exmp- 275 -> 27.5 # 1523 -> 152.3
		uint16_t temp_ds_sensor[2];  // format exmp- 275 -> 27.5 # 1523 -> 152.3
		uint16_t temp_uc_sensor;	// format exmp- 275 -> 27.5 # 1523 -> 152.3
	}sTemp;
	struct pid_data
	{
		uint16_t error;
		uint16_t setpoint;
		uint16_t K_P;
		uint16_t K_I;
		uint16_t K_D;
	}sPID;
	uint8_t reserved[120];
}sData_packet;

void servis_static_data_refresh()
{
	// TODO read name from EEPROM
	sData_packet.dev_name[0] = 'H';
	sData_packet.dev_name[1] = 'C';
	sData_packet.dev_name[2] = '2';
	sData_packet.dev_name[3] = '0';
	sData_packet.dev_name[4] = 'v';
	sData_packet.dev_name[5] = '1';
	sData_packet.dev_name[5] = '\0';
	sData_packet.uc_id = HAL_GetDEVID();
}

void refresh_temp_struct(void)
{
	// copy ntc value
//	for(uint8_t ntc_cnt =0; ntc_cnt< 3 ; ntc_cnt++)
//	{
//		sData_packet.sTemp.temp_sensor_ntc[ntc_cnt] = ntc_get_analog_sensors_value(ntc_cnt);
//	}
//	for(uint8_t ds18b20_cnt =0; ds18b20_cnt< 2 ; ds18b20_cnt++)
//	{
//		sData_packet.sTemp.temp_ds_sensor[ds18b20_cnt] = ds18b20_get_scratchpad_temperature(ds18b20_cnt,);
//	}
	// copy dc value
}



void servis_uart_send_packet(uint16_t ms_run_time)
{
	refresh_temp_struct();
	//refresh_PID_struct();
}


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
