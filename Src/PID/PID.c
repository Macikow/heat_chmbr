/*
 * PID.c
 *
 *  Created on: 11 paź 2020
 *      Author: kowma
 */
#include "main.h"




void PID_manual_settings(void)
{
	sPID.setpoint = 45.0;
	sPID.K_P = 550;
	sPID.K_I = 0.3;
	sPID.K_D = 450;
}

void PID_set_PWM(double PID_value)
{
	if(PID_value > 9728 ) PID_value = 9472;
	sPID.PWM_value = 0x2600 - (uint16_t)PID_value;
	if(sPID.PWM_value < 10) sPID.PWM_value = 10;
	if(sPID.PWM_value > 0x2500) sPID.PWM_value = 0x2500;
	TIM1->CCR1 = sPID.PWM_value;
	// system pracuje dla nastaw 10 - maksymalna moc
	// 0x2600  - minimalna moc
}


void PID_control_handler(uint16_t ms_delay)
{

	if(ms_counter != ms_delay) return;
	PID_manual_settings();
	sPID.output_fb = ntc_get_analog_sensors_value(0);
	sPID.error[1] = sPID.error[0];
	sPID.error[0] = sPID.setpoint - sPID.output_fb;
	// obliczenie wyjścia proporcjonalnego
	sPID.P_out = sPID.K_P*sPID.error[0];
	// obliczenie wyjścia całkującego
	sPID.I_error = sPID.I_error + (sPID.error[0] + sPID.error[1])/2;
	sPID.I_out = sPID.K_I * sPID.I_error;
	// obliczenie wyjścia róźniczkującego
	sPID.D_error = sPID.error[0] - sPID.error[1]; //  / t
	sPID.D_out = sPID.K_D * sPID.D_error;
	PID_set_PWM(sPID.P_out  + sPID.I_out + sPID.D_out);
}


void pid_set_setpoint_temp(double temp)
{
	sPID.setpoint = temp;
}

double get_setpoint_temperature(void)
{
	return sPID.setpoint;
}
