/*
 * time_counter.c
 *
 *  Created on: 6 lip 2020
 *      Author: kowma
 */



#include "main.h"
#include "time_counter.h"

typedef struct {
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint8_t secounds;
	uint8_t active_status;
}Timer_TypeDef;

Timer_TypeDef time;

void tc_set_counter(uint8_t days,uint8_t hours, uint8_t minutes)
{
	time.days = days;
	time.hours = hours;
	time.minutes = minutes;
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
					if( time.days == 0)
					{
						time.active_status = 0;
						// todo set_alarm;
					}
					else
					{
						time.days--;
						time.hours = 23;
						time.minutes = 59;
						time.secounds = 59;
					}
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
	*string_table++ = 0x30 + time.days / 10 ;
	*string_table++ = 0x30 + time.days % 10 ;
	*string_table++ = ':';
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

