/*
 * time_counter.h
 *
 *  Created on: 6 lip 2020
 *      Author: kowma
 */

#ifndef TIME_COUNTER_TIME_COUNTER_H_
#define TIME_COUNTER_TIME_COUNTER_H_


volatile uint8_t down_counter_flag;

void tc_set_counter(uint8_t hours, uint8_t minutes);
void tc_run_downcounter(void);
void tc_stop_downcounter(void);
void tc_down_counter(void);
void tc_convert_time_to_string(char * string_table);


#endif /* TIME_COUNTER_TIME_COUNTER_H_ */
