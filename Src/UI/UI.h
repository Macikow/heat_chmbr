/*
 * UI.c
 *
 *  Created on: 9 sie 2020
 *      Author: kowma
 */

#ifndef UI_UI_C_
#define UI_UI_C_


/* counts ms from 0 to 999  - its 1s overflow*/
volatile uint16_t ms_counter;


void UI_led_red_on();
void UI_led_red_off();
void UI_green_led_on();
void UI_green_led_off();
void UI_buzz_on();
void UI_buzz_off();
void UI_leds_check();
void UI_buzz_check();

void ui_increment_ms_counter(void);
void ui_change_ds18B20_status(uint8_t status);
void ui_change_ntc_status(uint8_t status);
void ui_change_heating_status(uint8_t status);

void ui_handler();


#endif /* UI_UI_C_ */
