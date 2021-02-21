/*
 * program_and_memory.c
 *
 *  Created on: 5 lut 2021
 *      Author: kowma
 */
#include "main.h"


extern char SET_TEMP[];
extern char CELCIUS_CHAR;
extern char END_OF_TEMP[];
extern char COUNTDOWN[];
extern char END_TIME[];
extern char EXIT[];
/**
 *	[1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 *	[P][R][O][G][R][A][M][ ][R][U][N][.][.][.][ ][ ][ ][ ][ ][ ]
 *	[R][T][[][2][5][.][6][o][C][]][ ][W][W][W][W][W][W][W][W][ ]
 */
e_pm_state program_run_display()
{
	 static uint8_t callback_counter_mod4=0;
	 if( (progTemp.temp - 2.0f) <= ui_get_primary_temperature())
	 {
		 // if real temperature >= set temp -2
		 program_countdown_timer_display();
		 return pm_state_countdown_display;
	 }

	 lcd_buf_clear();
	 lcd_buf_go_to(0,0);
	 lcd_buf_write_text("PROGRAM MODE");
	 for(uint8_t i=0;i<callback_counter_mod4;i++)
	 {
		 lcd_char('.');
	 }
	 if(callback_counter_mod4++ >= 3) callback_counter_mod4 =0;
	 progress_bar_display();
	 return pm_state_program_run_display;
}
/**
 *	[1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 *	[S][E][T][P][O][I][N][T][:][ ][[][4][6][.][7][o][C][]][ ][ ]
 *	[R][T][[][2][5][.][6][o][C][]][ ][W][W][W][W][W][W][W][W][ ]
 */
e_pm_state program_set_point_display()
{
	lcd_buf_clear();
	lcd_buf_go_to(0,0);
	lcd_buf_write_text(&SET_TEMP[0]);
	ntc_convert_float_to_string(&str_table[0], (double)progTemp.temp );
	lcd_buf_write_text((char*)&str_table[0]);
	lcd_char(CELCIUS_CHAR);
	lcd_buf_write_text(&END_OF_TEMP[0]);
	progress_bar_display();
	return pm_state_setpoit_display;
}
/**
 *	[1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 *	[C][O][U][N][T][D][O][W][N][ ][T][-][0][1][:][2][5][:][1][2]
 *	[R][T][[][2][5][.][6][o][C][]][ ][W][W][W][W][W][W][W][W][ ]
 */
e_pm_state program_countdown_timer_display()
{
	char str_countdown[10];
	lcd_buf_clear();
	lcd_buf_go_to(6,0);
	tc_get_countdown_string(&str_countdown[0]);
	lcd_buf_write_text(&str_countdown[0]);
	progress_bar_display();
	return pm_state_countdown_display;
}
/**
 *	[1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 *	[E][X][I][T][,][A][R][E][ ][Y][O][U][ ][S][U][R][E][?][ ][ ]
 */
e_pm_state program_exit_confirmation_display()
{
	lcd_buf_clear();
	lcd_buf_go_to(0,0);
	lcd_buf_write_text(&EXIT[0]);
	progress_bar_display();
	return pm_state_exit_confirmation_display;
}
/**
 *	[1][2][3][4][5][6][7][8][9][0][1][2][3][4][5][6][7][8][9][0]
 *	[E][N][D][ ][T][I][M][E][,][ ][P][R][E][S][S][ ][O][K][ ][ ]
 */
e_pm_state program_countdown_timer_end()
{
	lcd_buf_clear();
	lcd_buf_go_to(0,0);
	lcd_buf_write_text(&END_TIME[0]);
	progress_bar_display();
	return pm_state_countdown_end;
}
/**
 *
 */
e_pm_state exit_pm()
{
	tc_interrupted_end();
	ui_return_to_menu();
	return pm_state_program_run_display;
}


void populte_pm(uint8_t i, e_pm_state state , e_pm_event event, pm_event_handler pf)
{
	as_pm_state_machine[i].state_machine = state;
	as_pm_state_machine[i].state_machine_event = event;
	as_pm_state_machine[i].state_machine_event_handler = pf;
}

void program_mode_populate_table()
{
	populte_pm(0, pm_state_program_run_display , pm_event_button_ok, program_run_display); 			// press ok to refresh
	populte_pm(1, pm_state_program_run_display , pm_event_time_1s, program_run_display); 				// 1s to refresh
	populte_pm(2, pm_state_program_run_display , pm_event_time_7s, program_set_point_display);		// go to setpoint
	populte_pm(3, pm_state_program_run_display , pm_event_countdown_end, program_countdown_timer_end);// end of time
	populte_pm(4, pm_state_program_run_display , pm_event_button_exit, program_exit_confirmation_display);//

	populte_pm(5, pm_state_setpoit_display , pm_event_button_ok, program_set_point_display); 			// press ok to refresh
	populte_pm(6, pm_state_setpoit_display , pm_event_time_1s, program_set_point_display); 			// 1s to refresh
	populte_pm(7, pm_state_setpoit_display , pm_event_time_7s, program_run_display);				//
	populte_pm(8, pm_state_setpoit_display , pm_event_countdown_end, program_countdown_timer_end);			//
	populte_pm(9, pm_state_setpoit_display , pm_event_button_exit, program_exit_confirmation_display);			//

	populte_pm(10, pm_state_countdown_display , pm_event_button_ok, program_countdown_timer_display); 			// press ok to refresh
	populte_pm(11, pm_state_countdown_display , pm_event_time_1s, program_countdown_timer_display); 			// 1s to refresh
	populte_pm(12, pm_state_countdown_display , pm_event_time_7s, program_set_point_display);				//
	populte_pm(13, pm_state_countdown_display , pm_event_countdown_end, program_countdown_timer_end);			//
	populte_pm(14, pm_state_countdown_display , pm_event_button_exit, program_exit_confirmation_display);			//

	populte_pm(15, pm_state_exit_confirmation_display , pm_event_button_ok, exit_pm ); 			// press ok to refresh
	populte_pm(16, pm_state_exit_confirmation_display , pm_event_time_1s, program_exit_confirmation_display); 			// 1s to refresh
	populte_pm(17, pm_state_exit_confirmation_display , pm_event_time_7s, program_set_point_display);				//
	populte_pm(18, pm_state_exit_confirmation_display , pm_event_countdown_end, program_countdown_timer_end);			//
	populte_pm(19, pm_state_exit_confirmation_display , pm_event_button_exit, program_set_point_display);

	populte_pm(20, pm_state_countdown_end , pm_event_button_ok, exit_pm ); 							// press ok to refresh
	//
}

void program_mode_handler(e_pm_event event)
{
	static e_pm_state actual_state = pm_state_program_run_display;
	if( (actual_state < pm_state_last) && (event < pm_event_last))
	{
		for(uint8_t state_counter = 0; state_counter < PM_FSM_STATES ; state_counter++)
		{
			if((as_pm_state_machine[state_counter].state_machine == actual_state ) && ( as_pm_state_machine[state_counter].state_machine_event == event)  )
			{
				actual_state = (*as_pm_state_machine[state_counter].state_machine_event_handler)();
				break;
			}
		}
	}
}
