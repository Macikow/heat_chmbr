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


typedef struct menu_struct menu_t;

struct menu_struct {

 	char * name;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(void);

};

menu_t run_heating,set_temp_heating;
menu_t stop_confirmation;
menu_t set_time_heating;
menu_t start_heating;
menu_t run_mem1;
menu_t set_time_mem1;
menu_t set_temp_mem1 ;
menu_t set_temp_mem1;
menu_t start_mem1;
menu_t stop_confirmation_mem1;
menu_t run_mem2;
menu_t set_time_mem2;
menu_t set_temp_mem2 ;
menu_t set_temp_mem2;
menu_t start_mem2;
menu_t stop_confirmation_mem2;
menu_t run_mem3;
menu_t set_time_mem3;
menu_t set_temp_mem3 ;
menu_t set_temp_mem3;
menu_t start_mem3;
menu_t stop_confirmation_mem3;
menu_t run_param;
menu_t pid_param;


menu_t run_heating = {"RUN HEATING", &run_mem1, &run_param, &set_time_heating, NULL, NULL};
	menu_t set_time_heating = {"TIME", NULL, NULL, &run_heating, &set_temp_heating, NULL };
		menu_t set_temp_heating = {"TEMPERATURE", NULL, NULL, &set_time_heating, &start_heating, NULL };
			menu_t start_heating = {"HEATING ...",NULL, NULL, NULL, &stop_confirmation,	NULL };
				menu_t stop_confirmation = {"Are you sure?",NULL, NULL,&run_heating , NULL, NULL };
menu_t run_mem1 = {"RUN MEMORY 1 " ,&run_mem2, &run_heating, &set_time_mem1, NULL, NULL};
	menu_t set_time_mem1 = {"TIME MEM1", NULL, NULL, &set_temp_mem1, &run_mem1, NULL};
		menu_t set_temp_mem1 = {"TEMP. MEM1", NULL, NULL, &start_mem1, &set_time_mem1, NULL};
			menu_t start_mem1 = {"HEATING ...",NULL, NULL, NULL, &stop_confirmation_mem1,	NULL };
				menu_t stop_confirmation_mem1 = {"Are you sure?",NULL, NULL,&run_mem1 , NULL, NULL };
menu_t run_mem2 = {"RUN MEMORY 2",&run_mem3, &run_mem1, &set_time_mem2, NULL, NULL};
	menu_t set_time_mem2 = {"TIME MEM2", NULL, NULL, &set_temp_mem2, &run_mem2, NULL};
		menu_t set_temp_mem2 = {"TEMP. MEM2", NULL, NULL, &start_mem2, &set_time_mem2, NULL};
			menu_t start_mem2 = {"HEATING ...",NULL, NULL, NULL, &stop_confirmation_mem2,	NULL };
				menu_t stop_confirmation_mem2 = {"Are you sure?",NULL, NULL,&run_mem2 , NULL, NULL };
menu_t run_mem3 = {"RUN MEMORY 3",&run_param, &run_mem2, &set_time_mem3, NULL, NULL};
	menu_t set_time_mem3 = {"TIME MEM3", NULL, NULL, &set_temp_mem3, &run_mem3, NULL};
		menu_t set_temp_mem3 = {"000.0", NULL, NULL, &start_mem3, &set_time_mem3, NULL};
			menu_t start_mem3 = {"HEATING ...",NULL, NULL, NULL, &stop_confirmation_mem3,	NULL };
				menu_t stop_confirmation_mem3 = {"Are you sure?",NULL, NULL,&run_mem3 , NULL, NULL };
//menu_t run_servis = {"SERVIS SETTINGS", }
menu_t run_param = {"DEV PARAMETERS", &run_heating, &run_mem3, &pid_param, NULL, NULL};
	menu_t pid_param = {"PID PARAMS", NULL,NULL,NULL,&run_param,NULL};
	//menu_t temp_param = {"TEMP. PARAMS", }
	//menu_t memmory_param = {"MEMMORY ", }
	//menu_t pc_conn_param = {"PC CONNECTON", }
	//menu_t firm_version = {"FIRMWARE VER.", }
	//menu_t hadware_version = {"HARDWARE VER.", }
	//menu_t add_sensors = {"EXTRA SENSORS", }
	//menu_t pcb_temp = {"PCB TEMPERATURE", }
menu_t *currentPointer = &run_heating;
uint8_t menu_index = 0;
uint8_t lcd_row_pos = 0;



void menu_refresh(void) {

	menu_t *temp;
	uint8_t i;
	if (currentPointer->parent) temp = (currentPointer->parent)->child;
	else temp = &run_heating;
	for (i = 0; i != menu_index - lcd_row_pos; i++) {
		temp = temp->next;
	}

	lcd_buf_clear();
	for (i = 0; i < LCD_ROWS; i++) {

		lcd_buf_go_to(i,0);
		if (temp == currentPointer) lcd_char(62);
		else lcd_char(' ');

		lcd_buf_go_to(i, 2);
		lcd_buf_write_text(temp->name);

		temp = temp->next;
		if (!temp) break;

	}
}


uint8_t menu_get_index(menu_t *q) {

	menu_t *temp;
	uint8_t i = 0;

	if (q->parent) temp = (q->parent)->child;
	else temp = &run_heating;

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
		menu_index = 0;
		lcd_row_pos = 0;

		if (currentPointer->parent) currentPointer = (currentPointer->parent)->child;
		else currentPointer = &run_heating;
	}
	menu_refresh();
}

void ui_menu_prev(void) {

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

	menu_refresh();
}


/* ui_handler_flag its busy flag its prevent
 * entering to ui_handler() few times with this same ms_counter value */



void ui_handler()
{
	if(ui_handler_flag == UI_HANDLER_FLAG_BUSY) return;

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


uint8_t ui_initalize(void)
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
