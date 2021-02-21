/*
 * time_counter.c
 *
 *  Created on: 6 lip 2020
 *      Author: kowma
 */



#include "main.h"
#include "time_counter.h"


void tc_init()
{
	time.active_status =0;
	time.hours =0;
	time.minutes =0;
	time.secounds =0;
}




void tc_set_counter(uint8_t hours, uint8_t minutes)
{
	time.hours = hours;
	time.minutes = minutes;
	time.secounds = 0;
}

void tc_run_downcounter()
{
	time.active_status = 1;
	down_counter_flag = 0;
}

void tc_stop_downcounter()
{
	time.active_status = 0;
	down_counter_flag = 0;
}


void end_tc_down_counter_event()
{
	switch(time.mode)
	{

		case TC_MODE_NORMAL_PROGRAM:
			time.active_status =0;
			program_mode_handler(pm_event_countdown_end);
			break;

		case TC_MODE_MEMORY:
			time.active_status =0;
			program_mode_handler(pm_event_countdown_end);
			break;

		case TC_MODE_SEQENCE_1:
			break;

		case TC_MODE_SEQENCE_2:
			break;

		case TC_MODE_SEQENCE_3:
			time.active_status =0;
			break;

	}
}


void tc_down_counter()
{
	if(time.active_status == 1 && down_counter_flag == 1)
	{
		if(time.secounds == 0)
		{
			if(time.minutes == 0)
			{
				if( time.hours == 0)
				{
					end_tc_down_counter_event();
				}
				else
				{
					time.hours--;
					time.minutes = 59;
					time.secounds = 59;
				}
			}
			else
			{
				time.minutes--;
				time.secounds = 59;
			}
		}
		else
		{
			time.secounds--;
		}
	}
	down_counter_flag = 0;
}

void tc_convert_time_to_string(char * string_table)
{
	*string_table++ = 0x30 + time.hours / 10;
	*string_table++ = 0x30 + time.hours % 10;
	*string_table++ = ':';
	*string_table++ = 0x30 + time.minutes / 10;
	*string_table++ = 0x30 + time.minutes % 10;
	*string_table++ = ':';
	*string_table++ = 0x30 + time.secounds / 10;
	*string_table++ = 0x30 + time.secounds % 10;
	*string_table++ = '\n';
	*string_table++ = '\r';
}

void tc_get_time(uint8_t *hours, uint8_t *minutes, uint8_t *secound)
{
	*hours = time.hours;
	*minutes = time.minutes;
	*secound = time.secounds;
}

void tc_get_countdown_string(char * str_dc_time)
{
	*str_dc_time++ = 0x30 + time.hours / 10;
	*str_dc_time++ = 0x30 + time.hours % 10;
	*str_dc_time++ = ':';
	*str_dc_time++ = 0x30 + time.minutes / 10;
	*str_dc_time++ = 0x30 + time.minutes % 10;
	*str_dc_time++ = ':';
	*str_dc_time++ = 0x30 + time.secounds / 10;
	*str_dc_time++ = 0x30 + time.secounds % 10;
	*str_dc_time = '\0';
}

void tc_set_dc_flag(uint8_t flag)
{
	down_counter_flag = flag;
}


uint8_t tc_get_status(void)
{
	return time.active_status;
}

void tc_set_mode(uint8_t program)
{
	time.mode = program;
}

void tc_interrupted_end(void)
{
	tc_init();
	down_counter_flag = 0;
}




