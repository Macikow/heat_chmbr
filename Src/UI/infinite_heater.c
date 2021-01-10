/*
 * infinite_heater.c
 *
 *  Created on: Jan 6, 2021
 *      Author: kowma
 */
#include "main.h"
#define PROGRES_BAR_RESOLUTION 7
#define PROGRES_BAR_X 11
#define PROGRES_BAR_Y 1
#define IH_FSM_STATES 23

char COOLING[] = "Cooling";
char HEATING[] = "Heating";
char STABILIZING[] = "STABILIZATION";
char ACTUAL_TEMP[] = "RT[";
char DELTA = 0x3;
char START_TEMP[] = "T[";
char CELCIUS_CHAR = 0xdf;
char END_OF_TEMP[] = "C]";
char SET_TEMP[] = "SET TEMP. ST[";


uint8_t str_table[20];


void progress_bar_display()
{

  float current_temp = ui_get_primary_temperature();
  float differance = htemp.temp- 25.0f;
  float accuracy = differance/PROGRES_BAR_RESOLUTION ;
  lcd_buf_go_to(0,1);
  lcd_buf_write_text(&ACTUAL_TEMP[0]);
  ntc_convert_float_to_string(&str_table[0], current_temp ); // TODO zmienić na primary sensor flaot
  lcd_buf_write_text((char*)&str_table[0]);
  lcd_char(CELCIUS_CHAR);
  lcd_buf_write_text(&END_OF_TEMP[0]);
  lcd_char(' ');
  for(uint8_t i = 0 ; i < PROGRES_BAR_RESOLUTION ; i++)
  {
	float pb_value = 25.0f + (float)(accuracy*i);
    if( pb_value  >= current_temp)
    {
      lcd_char(0x00);
    }
    else
    {
      lcd_char(0xff);
    }
  }
  lcd_char(' ');
}

void temp_ruler()
{
  float current_temp = ui_get_primary_temperature();
  lcd_buf_go_to(0,1);
  lcd_char(0x03);
  lcd_buf_write_text(&START_TEMP[0]);
  ntc_convert_float_to_string(&str_table[0], (current_temp - htemp.temp) ); // TODO zmienić na primary sensor flaot
  lcd_buf_write_text((char*)&str_table[0]);
  lcd_buf_go_to(9, 1);
  lcd_char(CELCIUS_CHAR);
  lcd_buf_write_text(&END_OF_TEMP[0]);
  lcd_char(' ');
  if((htemp.temp - current_temp)<0)
  {
	  lcd_buf_go_to(13,1);
	  for(uint8_t i = 0 ; i < 3 ; i++)
	  {
		  lcd_char(0x00);
	  }
	  lcd_char(0xff);
	  for(uint8_t i = 1 ; i < 4 ; i++)
	  {
		  if( (0.5f+i*0.5f) < (current_temp-htemp.temp))	lcd_char(0xff);
		  else lcd_char(0x00);
	  }
  }
  else{

	  lcd_buf_go_to(13,1);
	  for(int i = 3 ; i >= 0 ; --i)
	  {
		  if( (0.5f+(i)*0.5f) > (htemp.temp - current_temp))	lcd_char(0x00);
		  else lcd_char(0xff);
	  }
	  lcd_buf_go_to(16,1);
	  lcd_char(0xff);
	  for(uint8_t i = 0 ; i < 3 ; i++)
	  {
		  lcd_char(0x00);
	  }
  }
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
  if(htemp.temp > ui_get_primary_temperature())	lcd_buf_write_text(&HEATING[0]);
  else lcd_buf_write_text(&COOLING[0]);
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
	lcd_buf_clear();
	lcd_buf_go_to(0,0);
	lcd_buf_write_text(&SET_TEMP[0]);
	ntc_convert_float_to_string(&str_table[0], (double)htemp.temp );
	lcd_buf_write_text((char*)&str_table[0]);
	lcd_buf_go_to(17,0);
	lcd_char(CELCIUS_CHAR);
	lcd_buf_write_text(&END_OF_TEMP[0]);
	progress_bar_display();

	return ih_state_heating_setpoit_display;
}
/**
 * [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 * [E][X][I][T][,][A][R][E][ ][Y][O][U][ ][S][U][R][E][?][ ][ ]
 * [R][T][[][2][5][.][6][o][C][]][ ][W][W][W][W][W][W][W][W][ ]
 */
e_ih_state exit_confirmation_display()
{
	lcd_buf_clear();
	lcd_buf_go_to(0,0);
	lcd_buf_write_text("EXIT, ARE YOU SURE ?");
	progress_bar_display();
	return ih_state_exit_confirmation_display;
}

e_ih_state exit_ih_display()
{
	ui_return_to_menu();
	return ih_state_heating_display;
}

/**
 * [1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 * [S][T][A][B][I][L][I][Z][A][C][I][O][N][ ][ ][|][ ][ ][ ][ ]
 * [d][T][[][+][1][.][0][5][o][C][]][ ][|][|][|][|][|][|][|][ ]
 */
e_ih_state near_temp_display()
{
	static uint8_t callback_counter_mod4=0;
	lcd_buf_clear();
	lcd_buf_go_to(0,0);
	lcd_buf_write_text(&STABILIZING[0]);
	for(uint8_t i=0;i<callback_counter_mod4;i++)
	{
		lcd_char('.');
	}
	lcd_buf_go_to(16,0);
	lcd_char(0x04);
	temp_ruler();
	if(callback_counter_mod4++ >= 3) callback_counter_mod4 =0;
	return ih_state_near_temp_display;
}

void populte_exec(uint8_t i, e_ih_state state , e_ih_event event, pf_event_handler pf)
{
	as_ih_state_machine[i].state_machine = state;
	as_ih_state_machine[i].state_machine_event = event;
	as_ih_state_machine[i].state_machine_event_handler = pf;
}

void infinite_heater_populate_table()
{
	populte_exec(0, ih_state_heating_display , ih_event_button_ok, heating_display);
	/*2*/	populte_exec(1,ih_state_heating_display,ih_event_time_02s,heating_display);
	/*3*/	populte_exec(2,ih_state_heating_display,ih_event_button_up,set_point_display);
	/*4*/	populte_exec(3,ih_state_heating_display,ih_event_button_down,set_point_display);
	/*5*/	populte_exec(4,ih_state_heating_display,ih_event_time_5s,set_point_display);
	/*6*/	populte_exec(5,ih_state_heating_display,ih_event_near_time_5s,set_point_display);
	/*7*/	populte_exec(6,ih_state_heating_display,ih_event_button_exit,exit_confirmation_display);

	/*8*/	populte_exec(7,ih_state_heating_setpoit_display,ih_event_time_02s,set_point_display); // refresh display data
	/*9*/	populte_exec(8,ih_state_heating_setpoit_display,ih_event_time_5s,heating_display);
	/*10*/	populte_exec(9,ih_state_heating_setpoit_display,ih_event_button_up,set_point_display);
	/*11*/	populte_exec(10,ih_state_heating_setpoit_display,ih_event_button_down,set_point_display);
	/*12*/	populte_exec(11,ih_state_heating_setpoit_display,ih_event_near_time_5s,near_temp_display);
	/*13*/	populte_exec(12,ih_state_heating_setpoit_display,ih_event_button_exit,exit_confirmation_display);

	/*14*/	populte_exec(13,ih_state_exit_confirmation_display,ih_event_time_5s,heating_display);
	/*15*/	populte_exec(14,ih_state_exit_confirmation_display,ih_event_near_time_5s,set_point_display);
	/*16*/	populte_exec(15,ih_state_exit_confirmation_display,ih_event_button_exit,set_point_display);
	/*17*/	populte_exec(16,ih_state_exit_confirmation_display,ih_event_button_ok,exit_ih_display);

	/*18*/	populte_exec(17,ih_state_near_temp_display,ih_event_time_02s,near_temp_display);
	/*19*/	populte_exec(18,ih_state_near_temp_display,ih_event_button_up,set_point_display);
	/*20*/	populte_exec(19,ih_state_near_temp_display,ih_event_button_down,set_point_display);
	/*21*/	populte_exec(20,ih_state_near_temp_display,ih_event_button_exit,exit_confirmation_display);
	/*22*/	populte_exec(21,ih_state_near_temp_display,ih_event_time_5s,set_point_display);
	/*23*/	populte_exec(22,ih_state_near_temp_display,ih_event_near_time_5s,set_point_display);
}





void infinite_heater_handler(e_ih_event event)
{
	static e_ih_state actual_state = ih_state_heating_display;
	if( (actual_state < ih_state_last) && (event < ih_event_last))
	{
		for(uint8_t state_counter = 0; state_counter < IH_FSM_STATES ; state_counter++)
		{
			if((as_ih_state_machine[state_counter].state_machine == actual_state ) && ( as_ih_state_machine[state_counter].state_machine_event == event)  )
			{
				actual_state = (*as_ih_state_machine[state_counter].state_machine_event_handler)();
				break;
			}
		}
	}
}
