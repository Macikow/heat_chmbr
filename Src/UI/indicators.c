/*
 * indicators.c
 *
 *  Created on: 17 gru 2020
 *      Author: kowma
 */
#include "main.h"
// handle of leds and buzzer

enum states { IDLE_SENSOR_OK, IDLE_SENSOR_ERR, PRE_HEATING, OVER_HEAT, IDLE_END,  MAX_STATES } current_state;
enum events { EV_START_HEAT, EV_STOP_HEAT, EV_TIME_END, EV_SENSOR_BRAKE, EV_OVERHEAT, EV_HEATING, MAX_EVENTS } new_event;

void idle_state (void);
void idle_sensors_fault_state (void);
void idle_downcnt_end_state (void);
void heating_state (void);
void overheat_state (void);

enum events get_new_event (void);


void (*const state_table [MAX_STATES][MAX_EVENTS]) (void) = {
//		EV_START_HEAT	EV_STOP_HEAT	EV_TIME_END				EV_SENSOR_BRAKE				EV_OVERHEAT		EV_HEATING		MAX_EVENTS
	{ 	heating_state , NULL ,			NULL,					idle_sensors_fault_state,	NULL,			NULL			}, //IDLE_SENSOR_OK
    { 	NULL,			NULL,			NULL,					NULL,						NULL,			NULL    		}, //IDLE_SENSOR_ERR
    { 	NULL,			idle_state,		idle_downcnt_end_state, idle_sensors_fault_state,	overheat_state,	NULL			}, //PRE_HEATING
	{ 	NULL,			idle_state,		idle_downcnt_end_state, idle_sensors_fault_state,	NULL,			heating_state	}, //OVER_HEAT
	{ 	NULL,			idle_state,		NULL, 					idle_sensors_fault_state,	NULL,			NULL			}  //IDLE_END
	// MAX_STATES
};

void idle_state(void)
{
	LED_GREEN_GPIO_Port->BRR = LED_GREEN_Pin;
	LED_RED_GPIO_Port->BRR = LED_RED_Pin;
	BUZZER_GPIO_Port->BRR = BUZZER_Pin;

}
void idle_sensors_fault_state(void)
{

}
void idle_downcnt_end_state(void)
{

}
void heating_state(void)
{

}
void overheat_state(void)
{

}






