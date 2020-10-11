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
	char * name;
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

menu_t ns_set;
menu_t ns_time;
menu_t ns_temp;
menu_t ns_start;

menu_t mem1_set;
menu_t mem1_time; //= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem1_temp; // = {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem1_start; //= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};

menu_t mem2_set;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem2_time;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem2_temp;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem2_start;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};

menu_t mem3_set;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem3_time;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem3_temp;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};
menu_t mem3_start;//= {"", NULL, NULL, NULL, NULL , NULL,0,0,0,0};





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




#endif /* UI_UI_C_ */
