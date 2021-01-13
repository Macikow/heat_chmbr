/*
 * program&sequance.c
 *
 *  Created on: 11 sty 2021
 *      Author: kowma
 */
#include "main.h"

// -- program functions --
e_ps_state return_to_menu(void)
{
	return ps_neutral_state;
}

e_ps_state program_heating_display(void)
{
	return ps_program_heating_state;
}
e_ps_state program_sets_display(void)
{
	return ps_program_sets_display_state;
}
e_ps_state program_exit(void)
{
	return ps_program_exit_ask_state;
}
e_ps_state program_stabilation_display(void)
{
	return ps_program_stabilation_state;
}
e_ps_state program_time_end(void)
{
	return ps_program_end_time_state;
}
// --- sequance1 functions
e_ps_state sequance1_heating_display(void)
{
	return ps_sequance_1_heating_state;
}
e_ps_state sequance1_sets_display(void)
{
	return ps_sequance_1_sets_display_state;
}
e_ps_state sequance1_exit(void)
{
	return ps_sequance_1_exit_ask_state;
}
e_ps_state sequance1_stabliziation_display(void)
{
	return ps_sequance_1_stablilization_state;
}

// --- sequance2 functions
e_ps_state sequance2_heating_display(void)
{
	return ps_sequance_2_heating_state;
}
e_ps_state sequance2_sets_display(void)
{
	return ps_sequance_2_sets_display_state;
}
e_ps_state sequance2_exit(void)
{
	return ps_sequance_2_exit_ask_state;
}
e_ps_state sequance2_stabliziation_display(void)
{
	return ps_sequance_2_stablilization_state;
}

// --- sequance3 functions
e_ps_state sequance3_heating_display(void)
{
	return ps_sequance_3_heating_state;
}
e_ps_state sequance3_sets_display(void)
{
	return ps_sequance_3_sets_display_state;
}
e_ps_state sequance3_exit(void)
{
	return ps_sequance_3_exit_ask_state;
}
e_ps_state sequance3_stabliziation_display(void)
{
	return ps_sequance_3_stablilization_state;
}
e_ps_state sequance3_end(void)
{
	return ps_sequance_time_end_state;
}
//  mem1  functions ---
e_ps_state mem1_heating(void)
{
	return ps_mem1_heating_state;
}
e_ps_state mem1_sets_display(void)
{
	return ps_mem1_sets_display_state;
}
e_ps_state mem1_exit(void)
{
	return ps_mem1_exit_ask_state;
}
e_ps_state mem1_stabilation_display(void)
{
	return ps_mem1_stablilization_state;
}
e_ps_state mem1_time_end(void)
{
	return ps_mem1_end_time_state;
}
//  mem2  functions ---
e_ps_state mem2_heating(void)
{
	return ps_mem2_heating_state;
}
e_ps_state mem2_sets_display(void)
{
	return ps_mem2_sets_display_state;
}
e_ps_state mem2_exit(void)
{
	return ps_mem2_exit_ask_state;
}
e_ps_state mem2_stabilation_display(void)
{
	return ps_mem2_stablilization_state;
}
e_ps_state mem2_time_end(void)
{
	return ps_mem2_end_time_state;
}
//  mem3  functions ---
e_ps_state mem3_heating(void)
{
	return ps_mem3_heating_state;
}
e_ps_state mem3_sets_display(void)
{
	return ps_mem3_sets_display_state;
}
e_ps_state mem3_exit(void)
{
	return ps_mem3_exit_ask_state;
}
e_ps_state mem3_stabilation_display(void)
{
	return ps_mem3_stablilization_state;
}
e_ps_state mem3_time_end(void)
{
	return ps_exit_state;
}


static void populte_exec(uint8_t i, e_ps_state state , e_ps_event event, pf_ps_event_handler pf)
{
	as_ih_state_machine[i].state_machine = state;
	as_ih_state_machine[i].state_machine_event = event;
	as_ih_state_machine[i].state_machine_event_handler = pf;
}

void program_and_sequance_populate_table()
{
	populte_exec(0, ps_neutral_state , ps_event_program_start , program_heating_display );
	populte_exec(1, ps_neutral_state , ps_event_sequance_start , sequance1_heating_display );
	populte_exec(2, ps_neutral_state , ps_event_mem1_start , mem1_heating);
	populte_exec(3, ps_neutral_state , ps_event_mem2_start , mem2_heating );
	populte_exec(4, ps_neutral_state , ps_event_mem3_start , mem3_heating );

	// program states :
	populte_exec(5, ps_program_heating_state , ps_event_time_02s, program_heating_display );
	populte_exec(6, ps_program_heating_state , ps_event_time_5s , program_sets_display );
	populte_exec(7, ps_program_heating_state , ps_event_near_time_5s , program_sets_display );
	populte_exec(8, ps_program_heating_state , ps_event_button_exit ,program_exit );

	populte_exec(9, ps_program_sets_display_state , ps_event_time_02s , program_sets_display );
	populte_exec(10, ps_program_sets_display_state , ps_event_time_5s , program_heating_display );
	populte_exec(11, ps_program_sets_display_state , ps_event_button_exit ,program_exit );
	populte_exec(12, ps_program_sets_display_state , ps_event_near_time_5s ,program_stabilation_display );
	populte_exec(13, ps_program_sets_display_state , ps_event_program_time_end ,program_time_end );

	populte_exec(14,  ps_program_stabilation_state , ps_event_time_02s , program_stabilation_display );
	populte_exec(15,  ps_program_stabilation_state , ps_event_time_5s , program_sets_display );
	populte_exec(16, ps_program_stabilation_state , ps_event_near_time_5s ,program_sets_display );
	populte_exec(17, ps_program_stabilation_state , ps_event_button_exit ,program_exit );
	populte_exec(18,  ps_program_stabilation_state , ps_event_program_time_end , program_time_end );

	populte_exec(19,  ps_program_exit_ask_state , ps_event_time_5s  ,  program_sets_display );
	populte_exec(20,  ps_program_exit_ask_state , ps_event_near_time_5s  ,program_sets_display );
	populte_exec(21,  ps_program_exit_ask_state , ps_event_button_exit  ,program_sets_display );
	populte_exec(22,  ps_program_exit_ask_state , ps_event_button_ok  ,return_to_menu );

	populte_exec(23,  ps_program_end_time_state , ps_event_button_ok  ,  return_to_menu );
	populte_exec(24,  ps_program_end_time_state , ps_event_button_exit ,return_to_menu );

	// sequance 1 state  - heating
	populte_exec(25, ps_sequance_1_heating_state , ps_event_time_02s , sequance1_heating_display );
	populte_exec(26, ps_sequance_1_heating_state , ps_event_time_5s , sequance1_sets_display );
	populte_exec(27, ps_sequance_1_heating_state , ps_event_near_time_5s ,sequance1_sets_display );
	populte_exec(28, ps_sequance_1_heating_state , ps_event_button_exit , sequance1_exit );
	// sequance 1 state - set_display
	populte_exec(29, ps_sequance_1_sets_display_state , ps_event_time_02s ,      sequance1_sets_display );
	populte_exec(30, ps_sequance_1_sets_display_state , ps_event_time_5s ,       sequance1_heating_display );
	populte_exec(31, ps_sequance_1_sets_display_state , ps_event_near_time_5s ,  sequance1_stabliziation_display );
	populte_exec(32, ps_sequance_1_sets_display_state , ps_event_button_exit ,   sequance1_exit );
	populte_exec(33, ps_sequance_1_sets_display_state , ps_event_seq1_time_end , sequance2_heating_display );
	// sequance 1 state - stabilization
	populte_exec(34, ps_sequance_1_stablilization_state , ps_event_time_02s ,      sequance1_stabliziation_display );
	populte_exec(35, ps_sequance_1_stablilization_state , ps_event_time_5s ,       sequance1_sets_display );
	populte_exec(36, ps_sequance_1_stablilization_state , ps_event_near_time_5s ,  sequance1_sets_display );
	populte_exec(37, ps_sequance_1_stablilization_state , ps_event_button_exit ,   sequance1_exit );
	populte_exec(38, ps_sequance_1_stablilization_state , ps_event_seq1_time_end , sequance2_heating_display );
	// sequance 1 state - ase_end:
	populte_exec(39, ps_sequance_1_exit_ask_state , ps_event_time_5s , sequance1_sets_display );
	populte_exec(40, ps_sequance_1_exit_ask_state , ps_event_near_time_5s , sequance1_sets_display );
	populte_exec(41, ps_sequance_1_exit_ask_state , ps_event_button_exit , sequance1_sets_display );
	populte_exec(42, ps_sequance_1_exit_ask_state , ps_event_button_ok , return_to_menu );

	// sequance 1 state  - heating
	populte_exec(43, ps_sequance_2_heating_state , ps_event_time_02s , sequance2_heating_display );
	populte_exec(44, ps_sequance_2_heating_state , ps_event_time_5s , sequance2_sets_display );
	populte_exec(45, ps_sequance_2_heating_state , ps_event_near_time_5s ,sequance2_sets_display );
	populte_exec(46, ps_sequance_2_heating_state , ps_event_button_exit , sequance2_exit );
	// sequance 1 state - set_display
	populte_exec(47, ps_sequance_2_sets_display_state , ps_event_time_02s ,      sequance2_sets_display );
	populte_exec(48, ps_sequance_2_sets_display_state , ps_event_time_5s ,       sequance2_heating_display );
	populte_exec(49, ps_sequance_2_sets_display_state , ps_event_near_time_5s ,  sequance2_stabliziation_display );
	populte_exec(50, ps_sequance_2_sets_display_state , ps_event_button_exit ,   sequance2_exit );
	populte_exec(51, ps_sequance_2_sets_display_state , ps_event_seq2_time_end , sequance2_heating_display );
	// sequance 1 state - stabilization
	populte_exec(52, ps_sequance_2_stablilization_state , ps_event_time_02s ,      sequance2_stabliziation_display );
	populte_exec(53, ps_sequance_2_stablilization_state , ps_event_time_5s ,       sequance2_sets_display );
	populte_exec(54, ps_sequance_2_stablilization_state , ps_event_near_time_5s ,  sequance2_sets_display );
	populte_exec(55, ps_sequance_2_stablilization_state , ps_event_button_exit ,   sequance2_exit );
	populte_exec(56, ps_sequance_2_stablilization_state , ps_event_seq1_time_end , sequance3_heating_display );
	// sequance 1 state - ase_end:
	populte_exec(57, ps_sequance_2_exit_ask_state , ps_event_time_5s , sequance2_sets_display );
	populte_exec(58, ps_sequance_2_exit_ask_state , ps_event_near_time_5s, sequance2_sets_display );
	populte_exec(59, ps_sequance_2_exit_ask_state , ps_event_button_exit , sequance2_sets_display );
	populte_exec(60, ps_sequance_2_exit_ask_state , ps_event_button_ok , return_to_menu );

	// sequance 3 state  - heating
	populte_exec(61, ps_sequance_3_heating_state , ps_event_time_02s , sequance3_heating_display );
	populte_exec(62, ps_sequance_3_heating_state , ps_event_time_5s , sequance3_sets_display );
	populte_exec(63, ps_sequance_3_heating_state , ps_event_near_time_5s ,sequance3_sets_display );
	populte_exec(64, ps_sequance_3_heating_state , ps_event_button_exit , sequance3_exit );
	// sequance 3 state - set_display
	populte_exec(65, ps_sequance_3_sets_display_state , ps_event_time_02s ,      sequance3_sets_display );
	populte_exec(66, ps_sequance_3_sets_display_state , ps_event_time_5s ,       sequance3_heating_display );
	populte_exec(67, ps_sequance_3_sets_display_state , ps_event_near_time_5s ,  sequance3_stabliziation_display );
	populte_exec(68, ps_sequance_3_sets_display_state , ps_event_button_exit ,   sequance3_exit );
	populte_exec(69, ps_sequance_3_sets_display_state , ps_event_seq3_time_end , sequance3_heating_display );
	// sequance 3 state - stabilization
	populte_exec(70, ps_sequance_3_stablilization_state , ps_event_time_02s ,      sequance3_stabliziation_display );
	populte_exec(71, ps_sequance_3_stablilization_state , ps_event_time_5s ,       sequance3_sets_display );
	populte_exec(72, ps_sequance_3_stablilization_state , ps_event_near_time_5s ,  sequance3_sets_display );
	populte_exec(73, ps_sequance_3_stablilization_state , ps_event_button_exit ,   sequance3_exit );
	populte_exec(74, ps_sequance_3_stablilization_state , ps_event_seq3_time_end , sequance3_end );
	// sequance 3 state - ase_end:
	populte_exec(75, ps_sequance_3_exit_ask_state , ps_event_time_5s , sequance3_sets_display );
  	populte_exec(76, ps_sequance_3_exit_ask_state , ps_event_button_exit , sequance3_sets_display );
  	populte_exec(77, ps_sequance_3_exit_ask_state , ps_event_button_ok , return_to_menu );
  	// sequance 3 state - sequance time end
  	populte_exec(78, ps_sequance_time_end_state , ps_event_button_exit , return_to_menu );
  	populte_exec(79, ps_sequance_time_end_state , ps_event_button_ok , return_to_menu );

  	// mem1 states :
	populte_exec(80, ps_mem1_heating_state , ps_event_time_02s, mem1_heating );
	populte_exec(81, ps_mem1_heating_state , ps_event_time_5s , mem1_sets_display );
	populte_exec(82, ps_mem1_heating_state , ps_event_near_time_5s , mem1_sets_display );
	populte_exec(83, ps_mem1_heating_state , ps_event_button_exit , mem1_exit );

	populte_exec(84, ps_mem1_sets_display_state , ps_event_time_02s , mem1_sets_display );
	populte_exec(85, ps_mem1_sets_display_state , ps_event_time_5s , mem1_heating);
	populte_exec(86, ps_mem1_sets_display_state , ps_event_button_exit ,mem1_exit );
	populte_exec(87, ps_mem1_sets_display_state , ps_event_near_time_5s ,mem1_stabilation_display );
	populte_exec(88, ps_mem1_sets_display_state , ps_event_mem1_time_end ,mem1_time_end );

	populte_exec(89,  ps_mem1_stablilization_state , ps_event_time_02s , mem1_stabilation_display );
	populte_exec(90,  ps_mem1_stablilization_state , ps_event_time_5s , mem1_sets_display );
	populte_exec(91, ps_mem1_stablilization_state , ps_event_near_time_5s ,mem1_sets_display );
	populte_exec(92, ps_mem1_stablilization_state , ps_event_button_exit ,mem1_exit );
	populte_exec(93,  ps_mem1_stablilization_state , ps_event_mem1_time_end , mem1_time_end );

	populte_exec(94,  ps_mem1_exit_ask_state , ps_event_time_5s  ,  mem1_sets_display );
	populte_exec(95,  ps_mem1_exit_ask_state , ps_event_near_time_5s  ,mem1_sets_display );
	populte_exec(96,  ps_mem1_exit_ask_state , ps_event_button_exit  ,mem1_sets_display );
	populte_exec(97,  ps_mem1_exit_ask_state , ps_event_button_ok  ,return_to_menu );

	populte_exec(98,  ps_mem1_end_time_state , ps_event_button_ok  ,  return_to_menu );
	populte_exec(99,  ps_mem1_end_time_state , ps_event_button_exit , return_to_menu );

	// mem2 states :
	populte_exec(100, ps_mem2_heating_state , ps_event_time_02s, mem2_heating );
	populte_exec(101, ps_mem2_heating_state , ps_event_time_5s , mem2_sets_display );
	populte_exec(102, ps_mem2_heating_state , ps_event_near_time_5s , mem2_sets_display );
	populte_exec(103, ps_mem2_heating_state , ps_event_button_exit , mem2_exit );

	populte_exec(104, ps_mem2_sets_display_state , ps_event_time_02s , mem2_sets_display );
	populte_exec(105, ps_mem2_sets_display_state , ps_event_time_5s , mem2_heating );
	populte_exec(106, ps_mem2_sets_display_state , ps_event_button_exit ,mem2_exit );
	populte_exec(107, ps_mem2_sets_display_state , ps_event_near_time_5s ,mem2_stabilation_display );
	populte_exec(108, ps_mem2_sets_display_state , ps_event_mem2_time_end ,mem2_time_end );

	populte_exec(109,  ps_mem2_stablilization_state , ps_event_time_02s , mem2_stabilation_display );
	populte_exec(110,  ps_mem2_stablilization_state , ps_event_time_5s , mem2_sets_display );
	populte_exec(111, ps_mem2_stablilization_state , ps_event_near_time_5s ,mem2_sets_display );
	populte_exec(112, ps_mem2_stablilization_state , ps_event_button_exit ,mem2_exit );
	populte_exec(113,  ps_mem2_stablilization_state , ps_event_mem2_time_end , mem2_time_end );

	populte_exec(114,  ps_mem2_exit_ask_state , ps_event_time_5s  ,  mem2_sets_display );
	populte_exec(115,  ps_mem2_exit_ask_state , ps_event_near_time_5s  ,mem2_sets_display );
	populte_exec(116,  ps_mem2_exit_ask_state , ps_event_button_exit  ,mem2_sets_display );
	populte_exec(117,  ps_mem2_exit_ask_state , ps_event_button_ok  ,return_to_menu );

	populte_exec(118,  ps_mem2_end_time_state , ps_event_button_ok  ,  return_to_menu );
  	populte_exec(119,  ps_mem2_end_time_state , ps_event_button_exit ,return_to_menu );

  	// mem3 states :
	populte_exec(120, ps_mem3_heating_state , ps_event_time_02s, mem3_heating );
	populte_exec(121, ps_mem3_heating_state , ps_event_time_5s , mem3_sets_display );
	populte_exec(122, ps_mem3_heating_state , ps_event_near_time_5s , mem3_sets_display );
	populte_exec(123, ps_mem3_heating_state , ps_event_button_exit , mem3_exit );

	populte_exec(124, ps_mem3_sets_display_state , ps_event_time_02s , mem3_sets_display );
	populte_exec(125, ps_mem3_sets_display_state , ps_event_time_5s , mem3_heating );
	populte_exec(126, ps_mem3_sets_display_state , ps_event_button_exit ,mem3_exit );
	populte_exec(127, ps_mem3_sets_display_state , ps_event_near_time_5s ,mem3_stabilation_display );
	populte_exec(128, ps_mem3_sets_display_state , ps_event_mem3_time_end ,mem3_time_end );

	populte_exec(129,  ps_mem3_stablilization_state , ps_event_time_02s , mem3_stabilation_display );
	populte_exec(130,  ps_mem3_stablilization_state , ps_event_time_5s , mem3_sets_display );
	populte_exec(131, ps_mem3_stablilization_state , ps_event_near_time_5s ,mem3_sets_display );
	populte_exec(132, ps_mem3_stablilization_state , ps_event_button_exit ,mem3_exit );
	populte_exec(133,  ps_mem3_stablilization_state , ps_event_mem3_time_end , mem3_time_end );

	populte_exec(134,  ps_mem3_exit_ask_state , ps_event_time_5s  ,  mem3_sets_display );
	populte_exec(135,  ps_mem3_exit_ask_state , ps_event_near_time_5s  ,mem3_sets_display );
	populte_exec(136,  ps_mem3_exit_ask_state , ps_event_button_exit  ,mem3_sets_display );
	populte_exec(137,  ps_mem3_exit_ask_state , ps_event_button_ok  ,return_to_menu );

	populte_exec(138,  ps_mem3_end_time_state , ps_event_button_ok  ,  return_to_menu );
	populte_exec(139,  ps_mem3_end_time_state , ps_event_button_exit , return_to_menu );


}

void program_and_sequance_handler( e_ps_event event)
{

}
