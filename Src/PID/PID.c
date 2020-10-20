/*
 * PID.c
 *
 *  Created on: 11 paź 2020
 *      Author: kowma
 */

struct pid_s
{
	double setpoint;
	double error;
	double FB;
	double P;
	double I;
	double D;
}sPID;
double setpoint;

void pid_set_setpoint_temp(double temp)
{
	setpoint = temp;
}

double get_setpoint_temperature(void)
{
	return setpoint;
}
