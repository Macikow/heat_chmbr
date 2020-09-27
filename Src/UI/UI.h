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




#endif /* UI_UI_C_ */
