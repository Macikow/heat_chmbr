/*
 * UI.c
 *
 *  Created on: 9 sie 2020
 *      Author: kowma
 */

#include "main.h"
#include <stdlib.h>
#include <string.h>

#define HEATING_STATUS_STOP 0
#define HEATING_STATUS_RUN 1
#define HEATING_STATUS_ENDING 2
#define HEATING_STATUS_END 3
#define LCD_ROWS 2
#define SET_OPTION_TIME 1
#define SET_OPTION_TEMP 2
#define BUTTON_HANDLER_FUNC_MENU 0
#define BUTTON_HANDLER_FUNC_SET_TIME 1
#define BUTTON_HANDLER_FUNC_SET_TEMP 2


#define UI_HEAT_FLAG_STABILIZING 2
#define UI_HEAT_FLAG_HEATING 1
#define UI_HEAT_FLAG_OFF 0


uint8_t button_hold_on_counter =0;
uint8_t ui_heating_flag =0;


void ui_set_time_callback(void);
void ui_set_temp_callback(void);
void menu_refresh(void);
void menu_button_event(uint8_t button);
void set_time_button_event(uint8_t button);
void set_temp_button_event(uint8_t button);
void heat_event(uint8_t button);

uint8_t lcd_row_pos_level_1=0,lcd_row_pos_level_2=0;
menu_t *currentPointer = (menu_t *)&htemp;
uint8_t menu_index = 0;
uint8_t lcd_row_pos = 0;
enum button_e{menu_event,set_time_event,set_temp_event,heating_event};
enum button_e button_event_handler = menu_event;
void (*button_event[])(uint8_t button) = {menu_button_event, set_time_button_event,set_temp_button_event,heat_event,program_and_sequance_event};
char time_string[8];
char temp_string[8];

void ui_set_time_callback(){
	button_event_handler = set_time_event;
	menu_refresh();
}

void ui_set_temp_callback(){
	button_event_handler = set_temp_event;
	menu_refresh();
}
void ui_time_save_and_exit()
{
	button_event_handler = menu_event;
	menu_refresh();
}
void ui_time_exit_witout_save()
{
	button_event_handler = menu_event;
	menu_refresh();
}
void ui_temp_save_and_exit()
{
	button_event_handler = menu_event;
	menu_refresh();
}
void ui_temp_exit_witout_save()
{
	button_event_handler = menu_event;
	menu_refresh();
}

void infinite_heating_callback()
{
	button_event_handler = heating_event;
	infinite_heater_handler(ih_event_button_ok);
}
/*
void ui_nc_start_heating(){
	tc_set_counter(ns_time.hours, ns_time.minutes);
	pid_set_setpoint_temp(ns_temp.temp);
}
void ui_mem1_start_heating(){
	tc_set_counter(mem1_time.hours, mem1_time.minutes);
	pid_set_setpoint_temp(mem1_temp.temp);
}
void ui_mem2_start_heating(){
	tc_set_counter(mem2_time.hours, mem2_time.minutes);
	pid_set_setpoint_temp(mem2_temp.temp);
}
void ui_mem3_start_heating(){
	tc_set_counter(mem3_time.hours, mem3_time.minutes);
	pid_set_setpoint_temp(mem3_temp.temp);
}*/


void ui_time_plus()
{
	currentPointer->minutes++;
	if(currentPointer->minutes == 60)
	{
		currentPointer->minutes = 0;
		currentPointer->hours++;
		if(currentPointer->hours == 100)
		{
			currentPointer->hours = 0;
		}
	}
	menu_refresh();
}
void ui_time_minus()
{
	currentPointer->minutes--;
	if(currentPointer->minutes == 0xff)
	{
		currentPointer->minutes = 59;
		currentPointer->hours--;
		if(currentPointer->hours == 0xff)
		{
			currentPointer->hours = 99;
		}
	}
	menu_refresh();
}


void ui_temp_plus()
{
	currentPointer->temp = currentPointer->temp + 0.1;
	if(currentPointer->temp > 70)
	{
		currentPointer->temp = 70.0;
	}
	menu_refresh();
}
void ui_temp_minus() // TODO sprawdzić temperature otoczenie i do niej sie odnieść jako do najmniejszej możliwie ustawianej temperatury
{
	currentPointer->temp = currentPointer->temp - 0.1;
	if(currentPointer->temp < 0 )
	{
		currentPointer->temp = 0;
	}
	menu_refresh();
}

void ui_infinite_heating_temp_plus()
{
	htemp.temp = htemp.temp + 0.1;
	if(htemp.temp > 70)
	{
		htemp.temp = 70.0;
	}
}
void ui_infinite_heating_temp_minus() // TODO sprawdzić temperature otoczenie i do niej sie odnieść jako do najmniejszej możliwie ustawianej temperatury
{
	htemp.temp = htemp.temp - 0.1;
	if(htemp.temp < 0 )
	{
		htemp.temp = 0;
	}
}


void menu_button_event(uint8_t button)
{
	switch(button)
	{
	case UI_BUTTON_UP:
		ui_menu_prev();
		break;
	case UI_BUTTON_DOWN:
		ui_menu_next();
		break;
	case UI_BUTTON_OK:
		ui_menu_enter();
		break;
	case UI_BUTTON_BACK:
		ui_menu_back();
		break;
	}
}

void set_time_button_event(uint8_t button)
{
	switch(button)
	{
	case UI_BUTTON_UP:
		ui_time_plus();
		break;
	case UI_BUTTON_DOWN:
		ui_time_minus();
		break;
	case UI_BUTTON_OK:
		ui_time_save_and_exit();
		break;
	case UI_BUTTON_BACK:
		ui_time_exit_witout_save();
		break;
	}
}
void set_temp_button_event(uint8_t button)
{
	switch(button)
	{
	case UI_BUTTON_UP:
		ui_temp_plus();
		break;
	case UI_BUTTON_DOWN:
		ui_temp_minus();
		break;
	case UI_BUTTON_OK:
		ui_temp_save_and_exit();
		break;
	case UI_BUTTON_BACK:
		ui_temp_exit_witout_save();
		break;
	}
}

void heat_event(uint8_t button)
{
	switch(button)
	{
	case UI_BUTTON_UP:
		htemp.temp +=0.1;
		infinite_heater_handler(ih_event_button_up);
		break;
	case UI_BUTTON_DOWN:
		htemp.temp -=0.1;
		infinite_heater_handler(ih_event_button_down);
		break;
	case UI_BUTTON_OK:
		infinite_heater_handler(ih_event_button_ok);
		break;
	case UI_BUTTON_BACK:
		infinite_heater_handler(ih_event_button_exit);
		break;
	}
}


void ui_button_handler(uint8_t button)
{
	(*button_event[(uint8_t)button_event_handler])(button);
}

void convert_time_to_str(uint8_t time_h, uint8_t time_m)
{
	char zero_str = 48;
	time_string[0] = '[';
	time_string[1] = time_h/10 + zero_str;
	time_string[2] = time_h%10 + zero_str;
	time_string[3] = ':';
	time_string[4] = time_m/10 + zero_str;
	time_string[5] = time_m%10 + zero_str;
	time_string[6] = ']';
	time_string[7] = '\0';
}
void convert_temp_to_str(double temp_double)
{
	uint16_t temp_int = (uint16_t)(temp_double*10);
	char zero_str = 48;
	temp_string[0] = '[';
	temp_string[1] = temp_int/100 + zero_str;
	temp_string[2] = (temp_int%100)/10 +zero_str;
	temp_string[3] = '.';
	temp_string[4] = (temp_int%100)%10+zero_str;
	temp_string[5] = 0xdf;
	temp_string[6] = ']';
	temp_string[7] = '\0';
}

const char * ui_test_string(void)
{
	char *p = "Test";
	return p;
}

void menu_refresh(void) {

	menu_t *temp;
	uint8_t i;
	if (currentPointer->parent) temp = (currentPointer->parent)->child;
	else temp = (menu_t *)&heat;
	for (i = 0; i != menu_index - lcd_row_pos; i++) {
		temp = temp->next;
	}

	lcd_buf_clear();
	for (i = 0; i < LCD_ROWS; i++) {

		lcd_buf_go_to(0,i);
		if (temp == currentPointer)
		{
			if(button_event_handler != menu_event)
			{
				lcd_char(0x01);
			}
			else
			{
				lcd_char(62);
			}
		}
		else lcd_char(' ');

		lcd_buf_go_to(2, i);
		lcd_buf_write_text(temp->name);

		if(temp->set_option == SET_OPTION_TIME)
		{
			lcd_buf_go_to(12, i);
			convert_time_to_str(temp->hours, temp->minutes);
			lcd_buf_write_text(time_string);
			//lcd_buf_write_text(ui_convert_inttime_to_str(temp->hours, temp->minutes));
		}
		else if(temp->set_option == SET_OPTION_TEMP)
		{
			lcd_buf_go_to(12, i);
			convert_temp_to_str(temp->temp);
			lcd_buf_write_text(temp_string);
		}

		temp = temp->next;
		if (!temp) break;

	}
}


uint8_t menu_get_index(menu_t *q) {

	menu_t *temp;
	uint8_t i = 0;

	if (q->parent) temp = (q->parent)->child;
	else temp = (menu_t *)&heat;

	while (temp != q) {
		temp = temp->next;
		i++;
	}

	return i;
}

void ui_menu_next(void)
{
	if (currentPointer->next)
	{
		currentPointer = currentPointer->next;
		menu_index++;
		if (++lcd_row_pos > LCD_ROWS - 1) lcd_row_pos = LCD_ROWS - 1;
	}
	else
	{
		//menu_index = 0;
		//lcd_row_pos = 0;

		//if (currentPointer->parent) currentPointer = (currentPointer->parent)->child;
		//else currentPointer = &new_settings;
	}
	menu_refresh();
}

void ui_menu_prev(void) {


	if(currentPointer->prev)
	{
		currentPointer = currentPointer->prev;
		if (menu_index)
		{
			menu_index--;
			if (lcd_row_pos > 0) lcd_row_pos--;
		}
		else
		{
			menu_index = menu_get_index(currentPointer);

			if (menu_index >= LCD_ROWS - 1) lcd_row_pos = LCD_ROWS - 1;
			else lcd_row_pos = menu_index;
		}
	}
	menu_refresh();
}

uint8_t menu_get_level(menu_t *q) {

	menu_t *temp = q;
	uint8_t i = 0;

	if (!q->parent) return 0;

	while (temp->parent != NULL) {
		temp = temp->parent;
		i++;
	}

	return i;
}


void ui_menu_enter(void) {

	if (currentPointer->menu_function) currentPointer->menu_function();

	if (currentPointer->child)
	{

		switch (menu_get_level(currentPointer)) {
			case 0:
				lcd_row_pos_level_1 = lcd_row_pos;
				break;

			case 1:
				lcd_row_pos_level_2 = lcd_row_pos;
				break;
		}

		// switch...case can be replaced by:
		// lcd_row_pos_level[ menu_get_level(currentPointer) ] = lcd_row_pos;

		menu_index = 0;
		lcd_row_pos = 0;

		currentPointer = currentPointer->child;

		menu_refresh();
	}
}



void ui_menu_back(void) {

	if (currentPointer->parent) {

		switch (menu_get_level(currentPointer)) {
			case 1:
				lcd_row_pos = lcd_row_pos_level_1;
				break;

			case 2:
				lcd_row_pos = lcd_row_pos_level_2;
				break;
			}

		currentPointer = currentPointer->parent;
		menu_index = menu_get_index(currentPointer);

		menu_refresh();

	}
}
/* ui_handler_flag its busy flag its prevent
 * entering to ui_handler() few times with this same ms_counter value */


uint8_t iu_button_hold_handler()
{
	static uint8_t button_hold_on_timer_divider;


	if((GPIOB->IDR & GPIO_PIN_12) == 0)
	{
		if(ms_counter % 999 == 0)
		{
			button_hold_on_counter++;
		}
	}
	else if((GPIOC->IDR & GPIO_PIN_13) == 0)
	{
		if(ms_counter % 999 == 0)
		{
			button_hold_on_counter++;
		}
	}
	else
	{
		button_hold_on_counter = 0;
		return 0;
	}
	if(button_hold_on_counter < 3) return 1 ;
	else
	{
		button_hold_on_timer_divider = 2;
		if(button_hold_on_counter  > 4) button_hold_on_timer_divider = 3;
		if(button_hold_on_counter  > 6) button_hold_on_timer_divider = 8;
		if(button_hold_on_counter  > 10) button_hold_on_timer_divider = 14;
		if(button_hold_on_counter  > 15) button_hold_on_timer_divider = 25;
		if(ms_counter %(1000/button_hold_on_timer_divider) == 0)
		{
			if((GPIOB->IDR & GPIO_PIN_12) == 0)
			{
				ui_button_handler(UI_BUTTON_UP);
				return 2;
			}
			else if((GPIOC->IDR & GPIO_PIN_13) == 0)
			{
				ui_button_handler(UI_BUTTON_DOWN);
				return 3;
			}
		}
	}
	return 4;
}



uint8_t temp_progress_bar(double external_temp, double actual_temp, double setpoint)
{
	double sub;
	if(actual_temp >= setpoint - 0.5 )
	{
		return 10;
	}
	else
	{
		sub = (setpoint - external_temp) / 10;
		return  (actual_temp - external_temp)  / sub;
	}
}


void iu_time_handler(uint8_t button_hold)
{
  static uint16_t ms_time_stamp, ms_handler_counter, s_handler_counter ;
  if(button_hold == 1)
  {
	  s_handler_counter=0;
	  ms_handler_counter=0;
  }
  if(ms_time_stamp == ms_counter) return;
  ms_time_stamp = ms_counter;
  ms_handler_counter++;

  if(s_handler_counter == 60) s_handler_counter = 0;
  if(ms_handler_counter == 999)
  {
    ms_handler_counter = 0;
    s_handler_counter++;
  }

  if(button_event_handler == heating_event)
  {
    if((ms_handler_counter % 200) == 0)
    {
      infinite_heater_handler(ih_event_time_02s);
    }
    if(((s_handler_counter % 5) == 0) && (ms_handler_counter == 0))
    {
    	if(abs(htemp.temp - ui_get_primary_temperature()) < 2.0f )	infinite_heater_handler(ih_event_near_time_5s);
    	else infinite_heater_handler(ih_event_time_5s);
    }
  }
}




void ui_handler()
{

	if(ui_handler_flag == UI_HANDLER_FLAG_BUSY) return;

	if(iu_button_hold_handler() == 0) iu_time_handler(2); // call time_hander if dont hold button
	else iu_time_handler(1);

	//else if(button_event_handler == heating_event)

	//if(button_event_handler == heating_event) iu_heating_process_handler();



	ui_handler_flag = UI_HANDLER_FLAG_BUSY;
}

void build_menu(menu_t *currentNode, char *name, struct menu_struct *next, struct menu_struct *prev, struct menu_struct *child,
				struct menu_struct *parent, void (*f)(void), uint8_t set_option, double temp, uint8_t hours, uint8_t minutes )
{
	currentNode->name = name;
	currentNode->next = next;
	currentNode->prev = prev;
	currentNode->child = child;
	currentNode->parent = parent;
	currentNode->menu_function = f;
	currentNode->set_option = set_option;
	currentNode->temp = temp;
	currentNode->hours = hours;
	currentNode->minutes = minutes;

}


void ui_populate_with_rom_data(void)
{
	build_menu(&heat,"HEATING",				&prog,	NULL,	&htemp,		NULL,	NULL,0,25.0f,0,0);
		build_menu(&htemp,"SET TEMP",			&hstart,NULL,	NULL,	&heat,	ui_set_temp_callback,SET_OPTION_TEMP,25.0f,0,0);
		build_menu(&hstart,"START HEAT",		NULL,	&htemp,	NULL,	&heat,	&infinite_heating_callback,0,25.0f,0,0);
	build_menu(&prog,"SET PROGRAM",    &seq,	&heat,	&progTime,	NULL,	NULL,0,25.0f,0,0);
		build_menu(&progTime,"SET TIME",			&progTemp,	NULL,			NULL,	&prog,	ui_set_time_callback,SET_OPTION_TIME,25.0f,0,0);
		build_menu(&progTemp,"SET TEMP",			&progStart,	&progTime,NULL,	&prog,	ui_set_temp_callback,SET_OPTION_TEMP,25.0f,0,0);
		build_menu(&progStart,"START PROGRAM",NULL,				&progTemp,NULL,	&prog,	NULL,0,25.0f,0,0);
	build_menu(&seq,"SET SEQUANCE",			&mem,&prog,&seqTime1,NULL,NULL,0,25.0f,0,0);
		build_menu(&seqTime1,"SET TIME1",&seqTemp1,NULL,		NULL,&seq,ui_set_time_callback,SET_OPTION_TIME,25.0f,0,0);
		build_menu(&seqTemp1,"SET TEMP1",&seqTime2,&seqTime1,	NULL,&seq,ui_set_temp_callback,SET_OPTION_TEMP,25.0f,0,0);
		build_menu(&seqTime2,"SET TIME2",&seqTemp2,&seqTemp1,	NULL,&seq,ui_set_time_callback,SET_OPTION_TIME,25.0f,0,0);
		build_menu(&seqTemp2,"SET TEMP2",&seqTime3,&seqTime2,	NULL,&seq,ui_set_temp_callback,SET_OPTION_TEMP,25.0f,0,0);
		build_menu(&seqTime3,"SET TIME3",&seqTemp3,&seqTemp2,	NULL,&seq,ui_set_time_callback,SET_OPTION_TIME,25.0f,0,0);
		build_menu(&seqTemp3,"SET TEMP3",&seqStart,&seqTime3,	NULL,&seq,ui_set_temp_callback,SET_OPTION_TEMP,25.0f,0,0);
		build_menu(&seqStart,"START SEQUANCE",NULL,&seqTemp3,	NULL,&seq,NULL,0,25.0f,0,0);
	build_menu(&mem,"PROGRAM MEM",	&params,&seq,&mem1,NULL,	NULL,0,25.0f,0,0);
		build_menu(&mem1,"PROGRAM MEM1",&mem2,NULL,&mem1Time,	&mem,NULL,0,25.0f,0,0);
			build_menu(&mem1Time,"TIME MEM1",		&mem1Temp, NULL,		 NULL,&mem1,NULL,SET_OPTION_TIME,25.0f,0,0);
			build_menu(&mem1Temp,"TEMP MEM1",		&mem1Start,&mem1Time,NULL,&mem1,NULL,SET_OPTION_TEMP,25.0f,0,0);
			build_menu(&mem1Start,"START MEM1",	NULL,			 &mem1Temp,NULL,&mem1,NULL,0,25.0f,0,0);
		build_menu(&mem2,"PROGRAM MEM2",&mem3,&mem1,&mem2Time,&mem,NULL,0,25.0f,0,0);
			build_menu(&mem2Time,"TIME MEM2",  &mem2Temp, NULL,			NULL,&mem2,NULL,SET_OPTION_TIME,25.0f,0,0);
			build_menu(&mem2Temp,"TEMP MEM2",	 &mem2Start,&mem2Time,NULL,&mem2,NULL,SET_OPTION_TEMP,25.0f,0,0);
			build_menu(&mem2Start,"START MEM2",NULL,			&mem2Temp,NULL,&mem2,NULL,0,25.0f,0,0);
		build_menu(&mem3,"PROGRAM MEM3",NULL,&mem2,&mem3Time,&mem,NULL,0,25.0f,0,0);
			build_menu(&mem3Time,"TIME MEM3",		&mem3Temp,	NULL,			NULL,	&mem3,NULL,SET_OPTION_TIME,25.0f,0,0);
			build_menu(&mem3Temp,"TEMP MEM3",		&mem3Start,	&mem3Time,NULL,	&mem3,NULL,SET_OPTION_TEMP,25.0f,0,0);
			build_menu(&mem3Start,"START MEM3",	NULL,				&mem3Temp,NULL,	&mem3,NULL,0,25.0f,0,0);
	build_menu(&params,"PARAMETERS",	NULL,&mem,&paramsAnalog,NULL,NULL,0,25.0f,0,0);
		build_menu(&paramsAnalog,"ANALOG SENSOR",			&paramsDigital,	NULL,						NULL,	&params,NULL,0,25.0f,0,0);
		build_menu(&paramsDigital,"DIGITAL SENSOR",		&paramsPS,			&paramsAnalog,	NULL,	&params,NULL,0,25.0f,0,0);
		build_menu(&paramsPS,"PRIMARY SENSOR",				&paramsHumidity,&paramsDigital,	NULL,	&params,NULL,0,25.0f,0,0);
		build_menu(&paramsHumidity,"HUMIDITY SENSOR",	&paramsPressure,&paramsPS,			NULL,	&params,NULL,0,25.0f,0,0);
		build_menu(&paramsPressure,"PRESSURE SENSOR",	&paramsMat,			&paramsHumidity,NULL,	&params,NULL,0,25.0f,0,0);
		build_menu(&paramsMat,"MAT POWER",						&paramsFan,			&paramsPressure,NULL,	&params,NULL,0,25.0f,0,0);
		build_menu(&paramsFan,"FAN",									NULL,						&paramsMat,			NULL,	&params,NULL,0,25.0f,0,0);

	infinite_heater_populate_table();
/*	mem1_time.hours = rom_get_time_hours(1);
	mem1_time.minutes = rom_get_time_minutes(1);
	mem2_time.hours = rom_get_time_hours(2);
	mem2_time.minutes = rom_get_time_minutes(2);
	mem3_time.hours = rom_get_time_hours(3);
	mem3_time.minutes = rom_get_time_minutes(3);
	mem1_temp.temp = rom_get_temp(1);
	mem2_temp.temp = rom_get_temp(2);
	mem3_temp.temp = rom_get_temp(3);

*/

}

float ui_get_primary_temperature(void)
{
	return (float)(ds18b20_get_scratchpad_temperature(0,0)*0.0625f);
}

void ui_return_to_menu(void)
{
	button_event_handler = menu_event;
	menu_refresh();
}

/***
 * 	function is calling in SysTick IRQ Handler
 */


void ui_initalize(void)
{
	SysTick_Config(72000);
}






inline void UI_led_red_on()
{
	LED_RED_GPIO_Port->ODR |= LED_RED_Pin;
}
inline void UI_led_red_off()
{
	LED_RED_GPIO_Port->ODR &= ~LED_RED_Pin;
}
inline void UI_green_led_on()
{
	LED_GREEN_GPIO_Port->ODR |= LED_GREEN_Pin;
}
inline void UI_green_led_off()
{
	LED_GREEN_GPIO_Port->ODR &= ~LED_GREEN_Pin;
}
inline void UI_buzz_on()
{
	BUZZER_GPIO_Port->ODR |= BUZZER_Pin;
}
inline void UI_buzz_off()
{
	BUZZER_GPIO_Port->ODR &= ~BUZZER_Pin;
}

void UI_leds_check()
{
	UI_led_red_on();
	simple_delay_ms(500);
	UI_green_led_on();
	simple_delay_ms(500);
	UI_led_red_off();
	simple_delay_ms(500);
	UI_green_led_off();
	simple_delay_ms(500);
	UI_green_led_on();
	simple_delay_ms(500);
	UI_led_red_on();
	simple_delay_ms(500);
	UI_green_led_off();
	simple_delay_ms(500);
	UI_led_red_off();
}

void UI_buzz_check()
{
	UI_buzz_on();
	simple_delay_ms(500);
	UI_buzz_off();
	simple_delay_ms(500);
	UI_buzz_on();
	simple_delay_ms(500);
	UI_buzz_off();
}
