/*
 * ntc.h
 *
 *  Created on: Sep 2, 2020
 *      Author: kowma
 */

#ifndef SRC_NTC_NTC_H_
#define SRC_NTC_NTC_H_

#define ADC_CHANNELS 4

#define NTC_HANDLER_FLAG_FREE 0
#define NTC_HANDLER_FLAG_BUSY 1


uint16_t adc_value[ADC_CHANNELS];



volatile uint8_t ntc_handler_flag;

void ntc_handler(uint16_t ms_time_counter, uint8_t no_sensor);


void ntc_init(void);
double ntc_get_temp_sens_value(void);
void ntc_TEMP_SENS_calc(void);
void ntc_analog_sensors_calc(uint8_t no_sensor);
uint8_t ntc_check_analog_sensors_status(void);
double ntc_get_analog_sensors_value(uint8_t no_of_sensor);
uint8_t ntc_convert_float_to_string(uint8_t * string_table, double temperature_ntc);


#endif /* SRC_NTC_NTC_H_ */
