/*
 * ntc.c
 *
 *  Created on: Sep 2, 2020
 *      Author: kowma
 */


#include "main.h"
#include "math.h"
#include "ntc.h"



ADC_HandleTypeDef hadc1;

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
