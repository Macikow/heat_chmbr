/*
 * time_counter.h
 *
 *  Created on: 6 lip 2020
 *      Author: kowma
 */

#ifndef TIME_COUNTER_TIME_COUNTER_H_
#define TIME_COUNTER_TIME_COUNTER_H_

#define COUNTDOWN_FLAG_ENABLE 1 	// 1s event occure
#define COUNTDOWN_FLAG_DISABLE 0	// waiting for 1s event

#define TC_STATUS_ENABLE 1	// TIMER ENABLE
#define TC_STATUS_DISABLE 0 // TIMER DISABLE

#define TC_MODE_NONE 0
#define TC_MODE_NORMAL_PROGRAM 1
#define TC_MODE_MEMORY 2
#define TC_MODE_SEQENCE_1 3
#define TC_MODE_SEQENCE_2 4
#define TC_MODE_SEQENCE_3 5


typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t secounds;
	uint8_t active_status;
	uint8_t mode;
}Timer_TypeDef;

Timer_TypeDef time;

volatile uint8_t down_counter_flag;

void tc_set_counter(uint8_t hours, uint8_t minutes);
void tc_run_downcounter(void);
void tc_stop_downcounter(void);
void tc_down_counter(void);
void tc_convert_time_to_string(char * string_table);
void tc_get_time(uint8_t *hours, uint8_t *minutes, uint8_t *secound);
void tc_get_countdown_string(char * str_dc_time);
void tc_init(void);
void tc_interrupted_end(void);

// -------------- status / flagas --------
uint8_t tc_get_status(void);
uint8_t tc_set_status(uint8_t status);
uint8_t tc_get_dc_flag(void);
void tc_set_dc_flag(uint8_t flag);
void tc_set_mode(uint8_t program);


#endif /* TIME_COUNTER_TIME_COUNTER_H_ */
