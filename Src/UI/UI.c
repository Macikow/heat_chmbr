/*
 * UI.c
 *
 *  Created on: 9 sie 2020
 *      Author: kowma
 */

#include "main.h"
#include <stdlib.h>

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




struct statusStruct
{
	uint8_t ds18b20_amount;
	uint8_t ntc_amount;
	uint8_t heating_status;
	uint16_t current_temperature;
	uint16_t set_temperature;
	uint16_t set_time;
	uint16_t currnt_time;
	uint8_t pc_connection_status;
}HC_status;









void ui_set_time_callback(void);
void ui_set_temp_callback(void);
void menu_refresh(void);




uint8_t lcd_row_pos_level_1,lcd_row_pos_level_2;


menu_t *currentPointer = (menu_t *)&ns_set;
uint8_t menu_index = 0;
uint8_t lcd_row_pos = 0;
uint8_t button_handler_func = BUTTON_HANDLER_FUNC_MENU;
char time_string[8];
char temp_string[8];





void ui_set_time_callback(){
	button_handler_func = BUTTON_HANDLER_FUNC_SET_TIME;
	menu_refresh();
}

void ui_set_temp_callback(){
	button_handler_func = BUTTON_HANDLER_FUNC_SET_TEMP;
	menu_refresh();
}

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
}


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
void ui_time_save_and_exit()
{
	button_handler_func = BUTTON_HANDLER_FUNC_MENU;
	menu_refresh();
}
void ui_time_exit_witout_save()
{
	button_handler_func = BUTTON_HANDLER_FUNC_MENU;
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
void ui_temp_save_and_exit()
{
	button_handler_func = BUTTON_HANDLER_FUNC_MENU;
	menu_refresh();
}
void ui_temp_exit_witout_save()
{
	button_handler_func = BUTTON_HANDLER_FUNC_MENU;
	menu_refresh();
}



void ui_button_handler(uint8_t button)
{
	if(button_handler_func == BUTTON_HANDLER_FUNC_MENU)
	{
		if(button == UI_BUTTON_UP)
		{
			ui_menu_prev();
		}
		else if(button == UI_BUTTON_DOWN)
		{
			ui_menu_next();
		}
		else if(button == UI_BUTTON_OK)
		{
			ui_menu_enter();
		}
		else if(button == UI_BUTTON_BACK)
		{
			ui_menu_back();
		}
	}
	else if(button_handler_func == BUTTON_HANDLER_FUNC_SET_TIME)
	{
		if(button == UI_BUTTON_UP)
		{
			ui_time_plus();
		}
		else if(button == UI_BUTTON_DOWN)
		{
			ui_time_minus();
		}
		else if(button == UI_BUTTON_OK)
		{
			ui_time_save_and_exit();
		}
		else if(button == UI_BUTTON_BACK)
		{
			ui_time_exit_witout_save();
		}
	}
	else if(button_handler_func == BUTTON_HANDLER_FUNC_SET_TEMP)
	{
		if(button == UI_BUTTON_UP)
		{
			ui_temp_plus();
		}
		else if(button == UI_BUTTON_DOWN)
		{
			ui_temp_minus();
		}
		else if(button == UI_BUTTON_OK)
		{
			ui_temp_save_and_exit();
		}
		else if(button == UI_BUTTON_BACK)
		{
			ui_temp_exit_witout_save();
		}
	}

}



void ui_change_button_func(uint8_t func)
{
	/*if(func == UI_FUNC_MENU)
	{

	}
	else if(func == UI_FUNC_SET_TEMP)
	{

	}
	else if(func == UI_FUNC_SET_TIME)
	{

	}
	else
	{

	}*/
}



void ui_list_init(void){
//  read data from eeprom and put into scructs
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
	else temp = (menu_t *)&ns_set;
	for (i = 0; i != menu_index - lcd_row_pos; i++) {
		temp = temp->next;
	}

	lcd_buf_clear();
	for (i = 0; i < LCD_ROWS; i++) {

		lcd_buf_go_to(0,i);
		if (temp == currentPointer)
		{
			if(button_handler_func != BUTTON_HANDLER_FUNC_MENU)
			{
				lcd_char(0x7e);
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
	else temp = (menu_t *)&ns_set;

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


void iu_button_hold_handler()
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
		return ;
	}
	if(button_hold_on_counter < 3) return;
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
			}
			else if((GPIOC->IDR & GPIO_PIN_13) == 0)
			{
				ui_button_handler(UI_BUTTON_DOWN);
			}
		}
	}
}

 char HEATING[] = "     Heating...     ";
 char PROGRESS_BAR_1[] = "    [";
 char PROGRESS_BAR_2[] = "]    ";
 char ACTUAL_TEMP[] = "Actual temp.[";
 char CELCIUS_CHAR = 0xdf;
 char END_OF_TEMP[] = "C]";
 char SET_TEMP[] = " Set temp.  [";

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


void iu_heating_process_handler(){

	static uint16_t display_counter;
	uint8_t str_table[20];
	uint8_t progress_bar_value;
	// external temperature
	// get external temperature
	// set temperature - external

	progress_bar_value = temp_progress_bar(25.0, ntc_get_analog_sensors_value(0) , 45.0);
	for(uint8_t i = 0 ; i< 10 ; i++)
	{
		lcd_buf_go_to(5+i,1);
		if(progress_bar_value >= i )
		{
			lcd_char(0xff);
		}
		else
		{
			lcd_char(' ');
		}
	}
	temp_progress_bar(25.0, ntc_get_analog_sensors_value(0) , 45.0);

	if(ui_heating_flag == 0 /*UI_HEAT_FLAG_HEATING*/)
	{
		if(display_counter++ < 1000)
		{
			lcd_buf_go_to(0,0);
			lcd_buf_write_text(&HEATING[0]);
			lcd_buf_go_to(0,1);
			lcd_buf_write_text(&PROGRESS_BAR_1[0]);
			lcd_buf_go_to(15,1);
			lcd_buf_write_text(&PROGRESS_BAR_2[0]);
		}
		else if(display_counter < 8000)
		{
			lcd_buf_go_to(0,0);
			lcd_buf_write_text(&ACTUAL_TEMP[0]);
			lcd_buf_go_to(13,0);
			ntc_convert_float_to_string(&str_table[0],  ntc_get_analog_sensors_value(0));
			lcd_buf_write_text((char*)&str_table[0]);
			lcd_buf_go_to(17,0);
			lcd_char(CELCIUS_CHAR);
			lcd_buf_go_to(18,0);
			lcd_buf_write_text(&END_OF_TEMP[0]);
			lcd_buf_go_to(0,1);
			lcd_buf_write_text(&PROGRESS_BAR_1[0]);
			lcd_buf_go_to(15,1);
			lcd_buf_write_text(&PROGRESS_BAR_2[0]);
		}
		else if(display_counter < 9000)
		{
			lcd_buf_go_to(0,0);
			lcd_buf_write_text(&SET_TEMP[0]);
			lcd_buf_go_to(13,0);
			ntc_convert_float_to_string(&str_table[0], 45.0);
			lcd_buf_write_text((char*)&str_table[0]);
			lcd_buf_go_to(17,0);
			lcd_char(CELCIUS_CHAR);
			lcd_buf_go_to(18,0);
			lcd_buf_write_text(&END_OF_TEMP[0]);
			lcd_buf_go_to(0,1);
			lcd_buf_write_text(&PROGRESS_BAR_1[0]);
			lcd_buf_go_to(15,1);
			lcd_buf_write_text(&PROGRESS_BAR_2[0]);
		}
		else
		{
			display_counter = 0;
		}
	}
	else if(ui_heating_flag == UI_HEAT_FLAG_STABILIZING)
	{

	}
}


void ui_handler()
{

	if(ui_handler_flag == UI_HANDLER_FLAG_BUSY) return;

	iu_button_hold_handler();

	iu_heating_process_handler();

	//red led control

	if( (HC_status.ds18b20_amount == 0) && (HC_status.ntc_amount == 0))
	{
		if(ms_counter % 200 == 0)	LED_RED_GPIO_Port -> ODR ^= LED_RED_Pin;
	}
	else if(HC_status.ds18b20_amount == 0 || (HC_status.ntc_amount == 0))
	{
		if(ms_counter % 500 == 0) LED_RED_GPIO_Port -> ODR ^= LED_RED_Pin;
	}

	// green led control

	if(HC_status.heating_status == HEATING_STATUS_RUN)
	{
		if((ms_counter/10)%20 == 0)
		{
			LED_GREEN_GPIO_Port->ODR ^=  LED_GREEN_Pin;
		}
	}
	else if(HC_status.heating_status == HEATING_STATUS_STOP)
	{
		if(ms_counter % 499 == 0)
		{
			LED_GREEN_GPIO_Port->BRR = LED_GREEN_Pin;
		}
	}
	else if(HC_status.heating_status == HEATING_STATUS_ENDING)
	{
		if( (ms_counter % 200) == 0 )
		{
			LED_GREEN_GPIO_Port->ODR ^= LED_GREEN_Pin;
		}
	}
	else if(HC_status.heating_status == HEATING_STATUS_END)
	{
		if( ( ms_counter % 90 ) == 0)
		{
			LED_GREEN_GPIO_Port->BSRR = LED_GREEN_Pin;
		}
	}

	// buzzer contorl

	ui_handler_flag = UI_HANDLER_FLAG_BUSY;
}





void ui_change_ds18B20_status(uint8_t status)
{
	HC_status.ds18b20_amount = status;
}

void ui_change_ntc_status(uint8_t status)
{
	HC_status.ds18b20_amount = status;
}

void ui_change_heating_status(uint8_t status)
{
	HC_status.ds18b20_amount = status;
}


void ui_populate_with_rom_data(void)
{
	ns_set.name = "NEW SET.";
	ns_set.next = &mem1_set;
	ns_set.prev = NULL;
	ns_set.child = &ns_time;
	ns_set.parent = NULL;
	ns_set.menu_function = NULL;

	ns_time.name = "SET TIME";
	ns_time.next = &ns_temp;
	ns_time.prev = NULL;
	ns_time.child = NULL;
	ns_time.parent = &ns_set;
	ns_time.menu_function = ui_set_time_callback;
	ns_time.set_option = SET_OPTION_TIME;

	ns_temp.name = "SET TEMP";
	ns_temp.next = &ns_start;
	ns_temp.prev = &ns_time;
	ns_temp.child = NULL;
	ns_temp.parent = &ns_set;
	ns_temp.menu_function = ui_set_temp_callback;
	ns_temp.set_option = SET_OPTION_TEMP;

	ns_start.name = "START";
	ns_start.next = NULL;
	ns_start.prev = &ns_temp;
	ns_start.child = NULL;
	ns_start.parent = &ns_set;
	ns_start.menu_function = NULL;
	ns_start.set_option = 0;

	mem1_set.name = "MEM1 SET";
	mem1_set.next = &mem2_set;
	mem1_set.prev = &ns_set;
	mem1_set.child = &mem1_time;
	mem1_set.parent = NULL;

	mem1_time.name = "MEM1 TIME";
	mem1_time.next = &mem1_temp;
	mem1_time.prev = NULL;
	mem1_time.child = NULL;
	mem1_time.parent = &mem1_set;
	mem1_time.menu_function = NULL;
	mem1_time.set_option = SET_OPTION_TIME;

	mem1_temp.name = "MEM1 TEMP";
	mem1_temp.next = &mem1_start;
	mem1_temp.prev = &mem1_time;
	mem1_temp.child = NULL;
	mem1_temp.parent = &mem1_set;
	mem1_temp.menu_function = NULL;
	mem1_temp.set_option = SET_OPTION_TEMP;

	mem1_start.name = "MEM1 START";
	mem1_start.next = NULL;
	mem1_start.prev = &mem1_temp;
	mem1_start.child = NULL;
	mem1_start.parent = &mem1_set;
	mem1_start.menu_function = NULL;
	mem1_start.set_option = 0;

	mem2_set.name = "MEM2 SET";
	mem2_set.next = &mem3_set;
	mem2_set.prev = &mem1_set;
	mem2_set.child = &mem2_time;
	mem2_set.parent = NULL;

	mem2_time.name = "MEM2 TIME";
	mem2_time.next = &mem2_temp;
	mem2_time.prev = NULL;
	mem2_time.child = NULL;
	mem2_time.parent = &mem2_set;
	mem2_time.menu_function = NULL;
	mem2_time.set_option = SET_OPTION_TIME;

	mem2_temp.name = "MEM2 TEMP";
	mem2_temp.next = &mem2_start;
	mem2_temp.prev = &mem2_time;
	mem2_temp.child = NULL;
	mem2_temp.parent = &mem2_set;
	mem2_temp.menu_function = NULL;
	mem2_temp.set_option = SET_OPTION_TEMP;

	mem2_start.name = "MEM2 START";
	mem2_start.next = NULL;
	mem2_start.prev = &mem2_temp;
	mem2_start.child = NULL;
	mem2_start.parent = &mem2_set;
	mem2_start.menu_function = NULL;
	mem2_start.set_option = 0;

	mem3_set.name = "MEM3 SET";
	mem3_set.next = NULL;
	mem3_set.prev = &mem2_set;
	mem3_set.child = &mem3_time;
	mem3_set.parent = NULL;
	mem3_set.menu_function = NULL;

	mem3_time.name = "MEM3 TIME";
	mem3_time.next = &mem3_temp;
	mem3_time.prev = NULL;
	mem3_time.child = NULL;
	mem3_time.parent = &mem3_set;
	mem3_time.menu_function = NULL;
	mem3_time.set_option = SET_OPTION_TIME;

	mem3_temp.name = "MEM3 TEMP";
	mem3_temp.next = &mem3_start;
	mem3_temp.prev = &mem3_time;
	mem3_temp.child = NULL;
	mem3_temp.parent = &mem3_set;
	mem3_temp.menu_function = NULL;
	mem3_temp.set_option = SET_OPTION_TEMP;

	mem3_start.name = "MEM3 START";
	mem3_start.next = NULL;
	mem3_start.prev = &mem3_temp;
	mem3_start.child = NULL;
	mem3_start.parent = &mem3_set;
	mem3_start.menu_function = NULL;
	mem3_start.set_option = 0;



	//{, &memory1_settings, NULL, &new_settings_time, NULL , NULL,0,0,0,0};
//	menu_t ns_time= {"SET TIME", &new_settings_temp, NULL, NULL, NULL , NULL,0,0,0,0};
//	menu_t ns_temp= {"SET TEMP.", &new_settings_start, &new_settings_time, NULL, &new_settings , ui_set_temp_callback,SET_OPTION_TEMP,56.8,0,0};
//	menu_t ns_start= {"START", NULL, &new_settings_temp, NULL, &new_settings , NULL,0,0,0,0};
//
//	menu_t m1s= {"MEMORY 1", &memory2_settings, &new_settings, &memory1_settings_time, NULL , NULL,0,0,0,0};
//	menu_t m1s_time= {"MEM1 TIME", &memory1_settings_temp, NULL, NULL, &memory1_settings , ui_set_time_callback,SET_OPTION_TIME,0,0,0};
//	menu_t m1s_temp= {"MEM1 TEMP", &memory1_settings_start, &memory1_settings_time, NULL, &memory1_settings , ui_set_temp_callback,SET_OPTION_TEMP,0,0,0};
//	menu_t m1s_start= {"MEM1 START", NULL, &memory1_settings_temp, NULL, &memory1_settings , NULL,0,0,0,0};
//
//	menu_t memory2_settings= {"MEMORY 2", &memory3_settings, &memory1_settings, &memory2_settings_time, NULL , NULL,0,0,0,0};
//	menu_t memory2_settings_time= {"MEM2 TIME", &memory2_settings_temp, NULL, NULL, &memory2_settings , ui_set_time_callback,SET_OPTION_TIME,0,0,0};
//	menu_t memory2_settings_temp= {"MEM2 TEMP", &memory2_settings_start, &memory2_settings_time, NULL, &memory2_settings , ui_set_temp_callback,SET_OPTION_TEMP,0,0,0};
//	menu_t memory2_settings_start= {"MEM2 START", NULL, &memory2_settings_temp, NULL, &memory2_settings , NULL,0,0,0,0};
//
//	menu_t memory3_settings= {"MEMORY 3", NULL, &memory2_settings, &memory3_settings_time, NULL , NULL,0,0,0,0};
//	menu_t memory3_settings_time= {"MEM3 TIME", &memory3_settings_temp, NULL, NULL, &memory3_settings , ui_set_time_callback,SET_OPTION_TIME,0,0,0};
//	menu_t memory3_settings_temp= {"MEM3 TEMP", &memory3_settings_start, &memory3_settings_time, NULL, &memory3_settings , ui_set_temp_callback,SET_OPTION_TEMP,0,0,0};
//	menu_t memory3_settings_start= {"MEM3 START", NULL, &memory3_settings_temp, NULL, &memory3_settings , NULL,0,0,0,0};


	mem1_time.hours = rom_get_time_hours(1);
	mem1_time.minutes = rom_get_time_minutes(1);
	mem2_time.hours = rom_get_time_hours(2);
	mem2_time.minutes = rom_get_time_minutes(2);
	mem3_time.hours = rom_get_time_hours(3);
	mem3_time.minutes = rom_get_time_minutes(3);
	mem1_temp.temp = rom_get_temp(1);
	mem2_temp.temp = rom_get_temp(2);
	mem3_temp.temp = rom_get_temp(3);



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
