/*
 * ntc.c
 *
 *  Created on: Sep 2, 2020
 *      Author: kowma
 *      name should be changed to analog temp sensor
 *			AHC have 3 slot of analog sensor
 *			NTC1 has been established as main sensor for control heating mat
 * 			NTC2 and NTC3 are additional analog sensor slots
 */


#include "main.h"
#include "ntc.h"
#include "ntc_beata3984.h"




ADC_HandleTypeDef hadc1;

#define V25  1.43  // [ V ]
#define AVGSLOPE  4.3  //[ mV / C ]
#define NTC_NUBMER_OF_SLOTS 3
#define NUMBER_OF_PROBE 5

#define NTC1_HEATING_MAT_SENSOR 0
#define NTC2_ADDITIONAL_ANALOG_SENSOR 1
#define NTC3_ADDITIONAL_ANALOG_SENSOR 2

struct ADC_struct{
	float NTC_resistance[NTC_NUBMER_OF_SLOTS];

	float NTC_voltage[NTC_NUBMER_OF_SLOTS];

	float NTC_temperature[NTC_NUBMER_OF_SLOTS];

	float TEMP_SENS_temperature;
}ntc_analog_temp[NUMBER_OF_PROBE];

/**
 * Start continue converting
 */
void ntc_init(void){
	HAL_ADC_Start(&hadc1);
	for(uint8_t i = 0; i <NUMBER_OF_PROBE ; i++ )
	{
		for(uint8_t j = 0; j <NTC_NUBMER_OF_SLOTS ; j++ )
		{
			ntc_analog_sensors_calc(j);
		}
	}
	for(uint8_t i = 0; i <NTC_NUBMER_OF_SLOTS ; i++ )
	{
		if(ntc_analog_temp[NUMBER_OF_PROBE-1].NTC_voltage[i] != 0)
		{
			if(i == 0) HC_status.ntc_heating_mat_sensor = STATUS_NTC_HEATING_MAT_SENSOR_OK;
			if(i == 1) HC_status.ntc2_additional_sensor = STATUS_NTC_HEATING_MAT_SENSOR_OK;
			if(i == 2) HC_status.ntc3_additional_sensor = STATUS_NTC_HEATING_MAT_SENSOR_OK;
		}
	}
}


void ntc_handler(uint16_t ms_time_counter)
{
	if(ntc_handler_flag == NTC_HANDLER_FLAG_BUSY) return ;
	if(ms_time_counter != ms_counter) return;

	for(uint8_t i=0; i < NTC_NUBMER_OF_SLOTS ; i++ ) ntc_analog_sensors_calc(i); // read all of slots
	for(uint8_t i=0; i < NUMBER_OF_PROBE ; i++)
	{
		if( ntc_analog_temp[i].NTC_voltage[NTC1_HEATING_MAT_SENSOR] == 0)
		{
			HC_status.ntc_heating_mat_sensor = STATUS_NTC_HEATING_MAT_SENSOR_ERROR;
			break;
		}
		else HC_status.ntc_heating_mat_sensor = STATUS_NTC_HEATING_MAT_SENSOR_OK; //  change status
	}


	ntc_handler_flag = NTC_HANDLER_FLAG_BUSY;
}

double ntc_get_temp_sens_value(void)
{
	double temp_sens_avg =0;
	for(uint8_t temp_pr_cnt =0 ; temp_pr_cnt< 5 ; temp_pr_cnt++ )
	{
		temp_sens_avg += ntc_analog_temp[temp_pr_cnt].TEMP_SENS_temperature;
	}
	return (temp_sens_avg/5);
}

void ntc_TEMP_SENS_calc(void)
{
	static uint8_t probe_counter;
	if(probe_counter++ > 5 ) probe_counter=0;
	ntc_analog_temp[probe_counter].TEMP_SENS_temperature = (((V25 * 1000.0 - (double)adc_value[3] * 0.8) / AVGSLOPE) + 25.0);
}

float search_temperature_from_pattern(float ntc_res_value)
{
	uint16_t i=0;
	while(ntc_res_value < NTC_BEATA3984[i++]);
	return ((float)--i)/10;
}

/**
 * Calculate ntc parametars and write to ntc main structure ADC_struct
 * @val no_sensor choose ntc input to read from adc_value[no_sensor]
 * measured procedure duration time is about 15us
 */
void ntc_analog_sensors_calc(uint8_t no_sensor)
{
	//for time measure //LED_RED_GPIO_Port->BSRR = LED_RED_Pin;
	static uint8_t asc_probe[3];
	if(no_sensor >= (ADC_CHANNELS-1)) return;
	if(asc_probe[no_sensor] > 5 ) asc_probe[no_sensor]=0;
	ntc_analog_temp[asc_probe[no_sensor]].NTC_voltage[no_sensor] = (float)((adc_value[no_sensor] * NTC_VOLTAGE_SUPPLY )/ 4095.00f);
	ntc_analog_temp[asc_probe[no_sensor]].NTC_resistance[no_sensor] = (NTC_VOLTAGE_SUPPLY * NTC_DIVIDER / ntc_analog_temp[asc_probe[no_sensor]].NTC_voltage[no_sensor]) - (float)NTC_DIVIDER;
	ntc_analog_temp[asc_probe[no_sensor]].NTC_temperature[no_sensor] = search_temperature_from_pattern(ntc_analog_temp[asc_probe[no_sensor]].NTC_resistance[no_sensor]);
	asc_probe[no_sensor]++;
	//for time measure  //LED_RED_GPIO_Port->BRR = LED_RED_Pin;
}

uint8_t ntc_check_analog_sensors_status(void)
{

	return 1 ; // TODO poprawić funkcje sprawdzającą czy czujnik jest podłączony do układu
}

double ntc_get_analog_sensors_value(uint8_t no_of_sensor)
{
	double temp_avg=0;
	for(uint8_t probe_cnt=0 ; probe_cnt < 5; probe_cnt++ )
	{
		temp_avg += ntc_analog_temp[probe_cnt].NTC_temperature[no_of_sensor];
	}
	temp_avg = temp_avg / 5;
	return temp_avg;
}


double ntc_convert_celsius_to_kelvin(double celsius_temperature)
{
	return celsius_temperature + 237.15;
}

double ntc_convert_kelvin_to_celsius(double kelvin_temperature)
{
	return kelvin_temperature - 273.15;
}

uint8_t ntc_convert_float_to_string(uint8_t * string_table, double temperature_ntc)
{
	uint8_t digit_counter = 0 ;
	uint16_t i=10000;
	uint16_t temperature_int_multiplied100;

	if(temperature_ntc < 0)
	{
		*(string_table++) = '-';
		temperature_int_multiplied100 = (uint16_t) (-temperature_ntc * 100);
	}
	else
	{
		temperature_int_multiplied100 = (uint16_t) (temperature_ntc * 100);
	}


	if(temperature_int_multiplied100 == 0)
		{
			*(string_table++) = '0';
			*(string_table++) = '.';
			*(string_table++) = '0';
			*(string_table++) = '0';
			*(string_table++) = ' ';
			*(string_table++) = ' ';
			*(string_table++) = '\0';
			return 1;
		}
		while((temperature_int_multiplied100 / i) == 0)
		{
			i = i/10;
			digit_counter++;
		}
		if(digit_counter >= 3) // ułamek zero przed kropką
		{
			*(string_table++) = '0';
			*(string_table++) = '.';
		}
		while(i != 0 )
		{
			*(string_table++) = 0x30 + temperature_int_multiplied100/i;
			temperature_int_multiplied100 = (uint16_t)temperature_int_multiplied100 % i;
			if(i == 100) *(string_table++) = '.';
			i = i/10;
		}
		while(digit_counter--)
		{
			*(string_table++) = ' ';
		}
		*(string_table++) = '\0';

	return 1;
}
