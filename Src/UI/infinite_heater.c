/*
 * infinite_heater.c
 *
 *  Created on: Jan 6, 2021
 *      Author: kowma
 */
#include "main.h"
#define PROGRES_BAR_RESOLUTION 8
#define PROGRES_BAR_X 11
#define PROGRES_BAR_Y 1

char HEATING[] = "Heating";
char ACTUAL_TEMP[] = "RT[";
char CELCIUS_CHAR = 0xdf;
char END_OF_TEMP[] = "C]";
char SET_TEMP[] = " Set temp.  [";


typedef  e_ih_state (*pf_event_handler)(void);

typedef struct
{
	e_ih_state state_machine;
	e_ih_event state_machine_event;
	pf_event_handler state_machine_event_handler;
} s_state_machine;


void progress_bar_display()
{
  uint8_t str_table[20];
  float current_temp = ui_get_primary_temperature();
  float setpoint = ui_get_set_point_temperature();
  float differance = setpoint - current_temp;
  float accuracy = differance/PROGRES_BAR_RESOLUTION ;
  lcd_buf_go_to(0,1);
  lcd_buf_write_text(&ACTUAL_TEMP[0]);
  ntc_convert_float_to_string(&str_table[0], (double)current_temp ); // TODO zmienić na primary sensor flaot
  lcd_buf_write_text((char*)&str_table[0]);
  lcd_char(CELCIUS_CHAR);
  lcd_buf_write_text(&END_OF_TEMP[0]);
  for(uint8_t i =0 ; i < PROGRES_BAR_RESOLUTION ; i++)
  {
    if((accuracy * i) >= current_temp)
    {
      lcd_char(0xff);
    }
    else
    {
      lcd_char(0x00);
    }
  }
  lcd_char(' ');
}

/**
 * [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 * [H][E][A][T][I][N][G][.][.][.][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
 * [R][T][[][2][5][.][6][ ][o][C][]][W][W][W][W][W][W][W][W][ ]
 */
e_ih_state heating_display()
{
  static uint8_t callback_counter_mod4=0;
  lcd_buf_clear();
  lcd_buf_go_to(0,0);
  lcd_buf_write_text(&HEATING[0]);
  for(uint8_t i=0;i<callback_counter_mod4;i++)
  {
	lcd_char('.');
  }
  if(callback_counter_mod4++ >= 3) callback_counter_mod4 =0;
  progress_bar_display();
  return ih_state_heating_display;
}

/**
 * [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 * [S][E][T][ ][T][E][M][P][.][ ][S][T][[][4][6][.][7][o][C][]]
 * [R][T][[][2][5][.][6][o][C][]][ ][W][W][W][W][W][W][W][W][ ]
 */
e_ih_state set_point_display()
{

	return ih_state_heating_setpoit_display;
}
/**
 * [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 * [E][X][I][T][,][A][R][E][ ][Y][O][U][ ][S][U][R][E][?][ ][ ]
 * [R][T][[][2][5][.][6][o][C][]][ ][W][W][W][W][W][W][W][W][ ]
 */
e_ih_state exit_confirmation_display()
{
	return ih_state_exit_confirmation_display;
}

e_ih_state exit_ih_display()
{
	return ih_state_exit;
}

/**
 * [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 * [S][T][[][4][6][.][7][o][C][]][ ][ ][ ][ ][ ][V][ ][ ][ ][ ]
 * [d][T][[][+][1][.][0][5][o][C][]][ ][|][|][|][|][|][|][|][ ]
 */
e_ih_state near_temp_display()
{
	return ih_state_near_temp_display;
}

s_state_machine as_ih_state_machine[]=
{
		{ih_state_heating_display,ih_event_button_ok,heating_display},
		{ih_state_heating_display,ih_event_time_02s,heating_display},
		{ih_state_heating_display,ih_event_button_up,set_point_display},
		{ih_state_heating_display,ih_event_button_down,set_point_display},
		{ih_state_heating_display,ih_event_time_2s,set_point_display},
		{ih_state_heating_display,ih_event_temp_near,near_temp_display},
		{ih_state_heating_display,ih_event_button_exit,exit_confirmation_display},
		{ih_state_heating_display,ih_event_button_ok,heating_display},

		{ih_state_heating_setpoit_display,ih_event_time_02s,set_point_display},
		{ih_state_heating_setpoit_display,ih_event_button_up,set_point_display},
		{ih_state_heating_setpoit_display,ih_event_button_down,set_point_display},
		{ih_state_heating_setpoit_display,ih_event_time_2s,heating_display},
		{ih_state_heating_setpoit_display,ih_event_temp_near,near_temp_display},
		{ih_state_heating_setpoit_display,ih_event_button_exit,exit_confirmation_display},

		{ih_state_exit_confirmation_display,ih_event_time_6s,heating_display},
		{ih_state_exit_confirmation_display,ih_event_button_exit,heating_display},
		{ih_state_exit_confirmation_display,ih_event_button_ok,exit_ih_display},

		{ih_state_near_temp_display,ih_event_time_02s,near_temp_display},
		{ih_state_near_temp_display,ih_event_button_up,near_temp_display},
		{ih_state_near_temp_display,ih_event_button_down,near_temp_display},
		{ih_state_near_temp_display,ih_event_button_exit,exit_confirmation_display},
		{ih_state_near_temp_display,ih_event_temp_far,heating_display},
};


void infinite_heater_handler(e_ih_event event)
{
	static e_ih_state actual_state = ih_state_heating_display;
	if( (actual_state < ih_state_last) && (event < ih_event_last) && (as_ih_state_machine[actual_state].state_machine_event == event) && (as_ih_state_machine[actual_state].state_machine_event_handler != NULL))
	{
		actual_state = (*as_ih_state_machine[actual_state].state_machine_event_handler)();
	}
}
