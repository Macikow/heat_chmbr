/*
 * ntc.c
 *
 *  Created on: Sep 2, 2020
 *      Author: kowma
 *      name should be changed to analog temp sensor
 */


#include "main.h"
#include "math.h"
#include "ntc.h"


ADC_HandleTypeDef hadc1;

#define V25  1.43  // [ V ]
#define AVGSLOPE  4.3  //[ mV / C ]



float a1[] = {-2.804e-07,	0.00062214,	-0.50532,	260.02}; //100 , 1000,
float a2[] = {-8.0662e-10,	1.0114e-05,	-0.049375,	138.34};  //1000 , 5000,
float a3[] = {-3.0381e-11,	9.8636e-07,	-0.013294,	89.666}; // 5000, 10000,
float a4[] = {-8.2157e-12,	3.9495e-7,	-0.0080207,	73.928}; //10000 , 12000
float a5[] = {-2.596e-12,	1.8088e-07,	-0.005287,	62.231}; // 12000 , 20000,
float a6[] = {1.473e-08,	-0.0016249,	34.525}; // 20000 , 40000
float a7[] = {4.6875e-09,	-0.00087143,	20.307}; //40000 , 60000
float a8[] = {-0.00023598,	-1.4709}; // 60000 , 100000
float a9[] = {-0.0001165,	-13.88}; // 100000 , 200000



struct ADC_struct{
	double NTC_resistance[3];

	double NTC_voltage[3];

	double NTC_temperature[3];

	double TEMP_SENS_temperature;
}ntc_analog_values;

void ntc_init()
{
	ui_change_ntc_status(ntc_check_analog_sensors_status());
}



void ntc_handler(uint16_t ms_time_counter, uint8_t no_sensor)
{
	if(ntc_handler_flag == NTC_HANDLER_FLAG_BUSY) return ;
	ntc_handler_flag = NTC_HANDLER_FLAG_BUSY;
	if(ms_time_counter != ms_counter) return;

	LED_GREEN_GPIO_Port->BSRR = LED_GREEN_Pin;
		  //ntc_handler(500);

	ntc_TEMP_SENS_calc();
	ntc_analog_sensors_calc(no_sensor);

	LED_GREEN_GPIO_Port->BRR = LED_GREEN_Pin;
}

double ntc_get_temp_sens_value(void)
{
	return ntc_analog_values.TEMP_SENS_temperature;
}

void ntc_TEMP_SENS_calc(void)
{
	ntc_analog_values.TEMP_SENS_temperature = (((V25 * 1000.0 - (double)adc_value[3] * 0.8) / AVGSLOPE) + 25.0);
}


// ze względu na szybkość obliczeń wykoanano aproksymacje wielomianową przedziałami
// dokumentacja znajduje sie w pliku ln_aprox.m
void ntc_analog_sensors_calc(uint8_t no_sensor)
{
	const double V_DD = 3.32;
	const uint16_t R_divide = 33000;
	uint8_t iterator = 0;

	if(adc_value[no_sensor] == 0) return ;
	ntc_analog_values.NTC_voltage[no_sensor] = adc_value[no_sensor] * V_DD / 4095;
	ntc_analog_values.NTC_resistance[no_sensor] = R_divide * V_DD / ntc_analog_values.NTC_voltage[no_sensor] - R_divide;

	if(ntc_analog_values.NTC_resistance[no_sensor] > 100 && ntc_analog_values.NTC_resistance[no_sensor] <= 1000)
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a1)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a1[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 1000 && ntc_analog_values.NTC_resistance[no_sensor] <= 5000)
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a2)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a2[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 5000 && ntc_analog_values.NTC_resistance[no_sensor] <= 10000)
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a3)/4;
		do {
			float power = 1;
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a3[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 10000 && ntc_analog_values.NTC_resistance[no_sensor] <= 12000)
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a4)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a4[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 12000 && ntc_analog_values.NTC_resistance[no_sensor] <= 20000)
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a5)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a5[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 20000 && ntc_analog_values.NTC_resistance[no_sensor] <= 40000)
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a6)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a6[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 40000 && ntc_analog_values.NTC_resistance[no_sensor] <= 60000 )
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a7)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a7[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 60000 && ntc_analog_values.NTC_resistance[no_sensor] <= 100000 )
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a8)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a8[iterator++]*power;
		} while(number_of_components--);
	}
	else if(ntc_analog_values.NTC_resistance[no_sensor] > 100000 && ntc_analog_values.NTC_resistance[no_sensor] <= 200000 )
	{
		ntc_analog_values.NTC_temperature[no_sensor] = 0;
		uint8_t number_of_components = sizeof(a9)/4;
		do {
			float power = 1;
			for(uint8_t i = 0; i < number_of_components - 1; i++)
			{
				power = power*ntc_analog_values.NTC_resistance[no_sensor];
			}
			ntc_analog_values.NTC_temperature[no_sensor] += a9[iterator++]*power;
		} while(number_of_components--);
	}
	//ntc_analog_values.NTC_temperature[no_sensor] = BETA * T_0 / (T_0 * log(ntc_analog_values.NTC_resistance[no_sensor]/10000) + BETA) - 273.15;
}

uint8_t ntc_check_analog_sensors_status(void)
{
	uint8_t ntc_counter;
	for(uint8_t analog_sens_count = 0; analog_sens_count < (ADC_CHANNELS - 1) ; analog_sens_count++ )
	{
		if(ntc_analog_values.NTC_temperature[analog_sens_count] != 0) ntc_counter++;
	}
	return ntc_counter;
}

double ntc_get_analog_sensors_value(uint8_t no_of_sensor)
{
	if(no_of_sensor == 0)		return ntc_analog_values.NTC_temperature[0];
	else if(no_of_sensor == 1)	return ntc_analog_values.NTC_temperature[1];
	else if(no_of_sensor == 2)	return ntc_analog_values.NTC_temperature[2];
}

void ntc_start_continous_converting(void)
{
	 HAL_ADC_Start(&hadc1);
}

double ntc_convert_celsius_to_kelvin(double celsius_temperature)
{
	return celsius_temperature + 237.15;
}

double ntc_convert_kelvin_to_celsius(double kelvin_temperature)
{
	return kelvin_temperature - 273.15;
}

double ntc_convert_ADC_to_double(void)
{
	uint16_t ADC_Data;
	const double V_DD = 3.32;
	const uint16_t R_divide = 33000;
	const uint16_t BETA = 3435;
	double U_ADC = 0, R_NTC, T_0 = 298.15;
	double T = 0;


	 if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
		 ADC_Data = HAL_ADC_GetValue(&hadc1);
		 ADC_Data = HAL_ADC_GetValue(&hadc1);
		 ADC_Data = HAL_ADC_GetValue(&hadc1);
		 HAL_ADC_Start(&hadc1);
	 }



	U_ADC = ADC_Data * V_DD / 4095;
	R_NTC = R_divide * V_DD / U_ADC - R_divide;

	T = BETA * T_0 / (T_0 * log(R_NTC/10000) + BETA);

	return ntc_convert_kelvin_to_celsius(T);
}

uint8_t ntc_convert_float_to_string(char * string_table)
{
	double temperature_ntc = ntc_convert_ADC_to_double();
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


void ntc_initalize()
{
	ntc_start_continous_converting();
}
