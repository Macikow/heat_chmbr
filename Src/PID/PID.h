/*
 * PID.h
 *
 *  Created on: 11 paź 2020
 *      Author: kowma
 */

#ifndef PID_PID_H_
#define PID_PID_H_

struct pid_s
{
	double setpoint;
	double error[2]; //error[0] - now error[1]  - t= t+1
	double output_fb;
	double K_P;
	double P_out;
	double K_I;
	double I_error;
	double I_out;
	double K_D;
	double D_error;
	double D_out;
	uint16_t PWM_value;
}sPID;

void pid_set_setpoint_temp(double temp);
double get_setpoint_temperature(void);
void PID_manual_settings(void);
void PID_control_handler(uint16_t ms_delay);
#endif /* PID_PID_H_ */
