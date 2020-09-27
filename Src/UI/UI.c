/*
 * UI.c
 *
 *  Created on: 9 sie 2020
 *      Author: kowma
 */

#include "main.h"

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


uint8_t button_hold_on_counter =0;



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

typedef struct menu_struct {
	const char * name;
	volatile struct menu_struct * next;
	volatile struct menu_struct * prev;
	volatile struct menu_struct * child;
	volatile struct menu_struct * parent;
	void (*menu_function)(void);
	uint8_t set_option;
	double temp;
	uint8_t hours;
	uint8_t minutes;
}menu_t;


volatile menu_t memory2_settings;
volatile menu_t memory2_settings_time;
volatile menu_t memory2_settings_temp;
volatile menu_t memory2_settings_start;

volatile menu_t memory3_settings;
volatile menu_t memory3_settings_time;
volatile menu_t memory3_settings_temp;
volatile menu_t memory3_settings_start;


volatile menu_t new_settings;
volatile menu_t new_settings_time;
volatile menu_t new_settings_temp;
volatile menu_t new_settings_start;

volatile menu_t memory1_settings;
volatile menu_t memory1_settings_time;
volatile menu_t memory1_settings_temp;
volatile menu_t memory1_settings_start;

volatile menu_t dev_propertise;


void ui_set_time_callback(void);
void ui_set_temp_callback(void);
void menu_refresh(void);


volatile menu_t new_settings = {"NEW SET.", &memory1_settings, NULL, &new_settings_time, NULL , NULL,0,0,0,0};
volatile menu_t new_settings_time= {"SET TIME", &new_settings_temp, NULL, NULL, &new_settings , ui_set_time_callback,SET_OPTION_TIME,0,12,45};
volatile menu_t new_settings_temp= {"SET TEMP.", &new_settings_start, &new_settings_time, NULL, &new_settings , ui_set_temp_callback,SET_OPTION_TEMP,56.8,0,0};
volatile menu_t new_settings_start= {"START", NULL, &new_settings_temp, NULL, &new_settings , NULL,0,0,0,0};

volatile menu_t memory1_settings= {"MEMORY 1", &memory2_settings, &new_settings, &memory1_settings_time, NULL , NULL,0,0,0,0};
volatile menu_t memory1_settings_time= {"MEM1 TIME", &memory1_settings_temp, NULL, NULL, &memory1_settings , ui_set_time_callback,SET_OPTION_TIME,0,0,0};
volatile menu_t memory1_settings_temp= {"MEM1 TEMP", &memory1_settings_start, &memory1_settings_time, NULL, &memory1_settings , ui_set_temp_callback,SET_OPTION_TEMP,0,0,0};
volatile menu_t memory1_settings_start= {"MEM1 START", NULL, &memory1_settings_temp, NULL, &memory1_settings , NULL,0,0,0,0};

volatile menu_t memory2_settings= {"MEMORY 2", &memory3_settings, &memory1_settings, &memory2_settings_time, NULL , NULL,0,0,0,0};
volatile menu_t memory2_settings_time= {"MEM2 TIME", &memory2_settings_temp, NULL, NULL, &memory2_settings , ui_set_time_callback,SET_OPTION_TIME,0,0,0};
volatile menu_t memory2_settings_temp= {"MEM2 TEMP", &memory2_settings_start, &memory2_settings_time, NULL, &memory2_settings , ui_set_temp_callback,SET_OPTION_TEMP,0,0,0};
volatile menu_t memory2_settings_start= {"MEM2 START", NULL, &memory2_settings_temp, NULL, &memory2_settings , NULL,0,0,0,0};

 volatile menu_t memory3_settings= {"MEMORY 3", NULL, &memory2_settings, &memory3_settings_time, NULL , NULL,0,0,0,0};
 volatile menu_t memory3_settings_time= {"MEM3 TIME", &memory3_settings_temp, NULL, NULL, &memory3_settings , ui_set_time_callback,SET_OPTION_TIME,0,0,0};
 volatile menu_t memory3_settings_temp= {"MEM3 TEMP", &memory3_settings_start, &memory3_settings_time, NULL, &memory3_settings , ui_set_temp_callback,SET_OPTION_TEMP,0,0,0};
 volatile menu_t memory3_settings_start= {"MEM3 START", NULL, &memory3_settings_temp, NULL, &memory3_settings , NULL,0,0,0,0};

uint8_t lcd_row_pos_level_1,lcd_row_pos_level_2;


menu_t *currentPointer = &new_settings;
uint8_t menu_index = 0;
uint8_t lcd_row_pos = 0;
uint8_t button_handler_func = BUTTON_HANDLER_FUNC_MENU;
char time_string[8];
char temp_string[8];


void ui_set_time_callback(){
	button_handler_func = BUTTON_HANDLER_FUNC_SET_TIME;
}

void ui_set_temp_callback(){
	button_handler_func = BUTTON_HANDLER_FUNC_SET_TEMP;
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
}
void ui_time_exit_witout_save()
{
	button_handler_func = BUTTON_HANDLER_FUNC_MENU;
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
}
void ui_temp_exit_witout_save()
{
	button_handler_func = BUTTON_HANDLER_FUNC_MENU;
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
	char string_table[7];
	if (currentPointer->parent) temp = (currentPointer->parent)->child;
	else temp = &new_settings;
	for (i = 0; i != menu_index - lcd_row_pos; i++) {
		temp = temp->next;
	}

	lcd_buf_clear();
	for (i = 0; i < LCD_ROWS; i++) {

		lcd_buf_go_to(0,i);
		if (temp == currentPointer) lcd_char(62);
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
	else temp = &new_settings;

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
	if(button_hold_on_counter < 2) return;
	if(button_hold_on_counter > 2)
	{
		button_hold_on_timer_divider = 2;
		if(button_hold_on_counter  > 4) button_hold_on_timer_divider = 3;
		if(button_hold_on_counter  > 6) button_hold_on_timer_divider = 8;
		if(button_hold_on_counter  > 10) button_hold_on_timer_divider = 14;
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


void ui_handler()
{

	if(ui_handler_flag == UI_HANDLER_FLAG_BUSY) return;

	iu_button_hold_handler();

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
