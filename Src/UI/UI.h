/*
 * UI.c
 *
 *  Created on: 9 sie 2020
 *      Author: kowma
 */

#ifndef UI_UI_C_
#define UI_UI_C_


#define UI_HANDLER_FLAG_FREE 0
#define UI_HANDLER_FLAG_BUSY 1
#define UI_BUTTON_UP	0
#define UI_BUTTON_DOWN	1
#define UI_BUTTON_OK	2
#define UI_BUTTON_BACK	3
volatile uint8_t ui_handler_flag;


/*
menu_t new_settings;

*/

typedef struct menu_struct {
	char *name;
	struct menu_struct * next;
	struct menu_struct * prev;
	struct menu_struct * child;
	struct menu_struct * parent;
	void (*menu_function)(void);
	uint8_t set_option;
	double temp;
	uint8_t hours;
	uint8_t minutes;
}menu_t;

menu_t
heat,
	htemp,
	hstart,
prog,
	progTime,
	progTemp,
	progStart,
seq,
	seqTime1
	,seqTemp1
	,seqTime2
	,seqTemp2
	,seqTime3
	,seqTemp3
	,seqStart
,mem
	,mem1
		,mem1Time
		,mem1Temp
		,mem1Start
	,mem2
		,mem2Time
		,mem2Temp
		,mem2Start
	,mem3
		,mem3Time
		,mem3Temp
		,mem3Start
,params
	,paramsAnalog
	,paramsDigital
	,paramsPS
	,paramsHumidity
	,paramsPressure
	,paramsMat
	,paramsFan;





void UI_led_red_on();
void UI_led_red_off();
void UI_green_led_on();
void UI_green_led_off();
void UI_buzz_on();
void UI_buzz_off();
void UI_leds_check();
void UI_buzz_check();


void ui_change_ds18B20_status(uint8_t status);
void ui_change_ntc_status(uint8_t status);
void ui_change_heating_status(uint8_t status);

void ui_handler(void);

void ui_menu_refresh(void);

void ui_menu_next(void);
void ui_menu_prev(void);
void ui_menu_enter(void);
void ui_menu_back(void);

void ui_button_handler(uint8_t button);
void ui_populate_with_rom_data(void);
float ui_get_primary_temperature();
void ui_return_to_menu(void);

// infinite_heater
#define IH_FSM_STATES 23

typedef enum {
	ih_state_heating_display,
	ih_state_heating_setpoit_display,
	ih_state_exit_confirmation_display,
	ih_state_exit,
	ih_state_near_temp_display,
	ih_state_last
}e_ih_state;

typedef enum {
	ih_event_button_up,
	ih_event_button_down,
	ih_event_button_exit,
	ih_event_button_ok,
	ih_event_time_5s,
	ih_event_time_02s,
	ih_event_near_time_5s,
	ih_event_near_time_02s,
	ih_event_last
}e_ih_event;

typedef e_ih_state (*pf_event_handler)(void);
typedef struct
{
	e_ih_state state_machine;
	e_ih_event state_machine_event;
	pf_event_handler state_machine_event_handler;
} s_state_machine;

e_ih_state heating_display();
e_ih_state set_point_display();
e_ih_state exit_confirmation_display();
e_ih_state exit_ih_display();
e_ih_state near_temp_display();

s_state_machine as_ih_state_machine[IH_FSM_STATES];


void infinite_heater_populate_table(void);
void infinite_heater_handler(e_ih_event event);


#endif /* UI_UI_C_ */
