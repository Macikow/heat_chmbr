/*
 * rom.c
 *
 *  Created on: 27 wrz 2020
 *      Author: kowma
 */
#include "main.h"

#define EE_VADDR_MEM1_TIME 1
#define EE_VADDR_MEM1_TEMP 2
#define EE_VADDR_MEM2_TIME 3
#define EE_VADDR_MEM2_TEMP 4
#define EE_VADDR_MEM3_TIME 5
#define EE_VADDR_MEM3_TEMP 6
#define EE_VADDR_PID_P 7
#define EE_VADDR_PID_I 8
#define EE_VADDR_PID_D 9

union rom_memory{
	__attribute__((packed))struct rom_s
	{
		uint16_t memory1_time;
		uint16_t memory1_temp;
		uint16_t memory2_time;
		uint16_t memory2_temp;
		uint16_t memory3_time;
		uint16_t memory3_temp;
		uint16_t PID_P;
		uint16_t PID_I;
		uint16_t PID_D;
	}rom_str;
	uint16_t rom_table[NB_OF_VAR];
}rom_mem;


void rom_refresh(void)
{
	for(uint8_t addr = 0; addr  < NB_OF_VAR ; addr++)
	{
		EE_ReadVariable(addr, &rom_mem.rom_table[addr-1]);
	}
}

void rom_value_init(void)
{
	void rom_refresh();

	if( (rom_mem.rom_str.memory1_temp == 0) && (rom_mem.rom_str.memory1_time == 0) )
	{
		rom_wite_mem1_data(11,11, 31.1);
	}
	if( (rom_mem.rom_str.memory2_temp == 0) && (rom_mem.rom_str.memory2_time == 0) )
	{
		rom_wite_mem2_data(22,22, 32.2);
	}
	if((rom_mem.rom_str.memory3_temp == 0) && (rom_mem.rom_str.memory3_time == 0))
	{
		rom_wite_mem3_data(33,33, 33.3);
	}
	if((rom_mem.rom_str.PID_P + rom_mem.rom_str.PID_I + rom_mem.rom_str.PID_D) == 0 )
	{
		rom_wite_PID_data(100,100,100);
	}

}

void rom_wite_mem1_data(uint8_t ui_time_hours,uint8_t ui_time_minutes, double ui_temp)
{
	rom_mem.rom_str.memory1_temp = (uint16_t) ui_temp*10;
	rom_mem.rom_str.memory1_time = ui_time_hours*100 + ui_time_minutes;
	EE_WriteVariable(EE_VADDR_MEM1_TEMP, rom_mem.rom_str.memory1_temp);
	EE_WriteVariable(EE_VADDR_MEM1_TIME, rom_mem.rom_str.memory1_time);
}
void rom_wite_mem2_data(uint8_t ui_time_hours,uint8_t ui_time_minutes, double ui_temp)
{
	rom_mem.rom_str.memory2_temp = (uint16_t) ui_temp*10;
	rom_mem.rom_str.memory2_time = ui_time_hours*100 + ui_time_minutes;
	EE_WriteVariable(EE_VADDR_MEM2_TEMP, rom_mem.rom_str.memory2_temp);
	EE_WriteVariable(EE_VADDR_MEM2_TIME, rom_mem.rom_str.memory2_time);
}
void rom_wite_mem3_data(uint8_t ui_time_hours,uint8_t ui_time_minutes, double ui_temp)
{
	rom_mem.rom_str.memory3_temp = (uint16_t) ui_temp*10;
	rom_mem.rom_str.memory3_time = ui_time_hours*100 + ui_time_minutes;
	EE_WriteVariable(EE_VADDR_MEM3_TEMP, rom_mem.rom_str.memory3_temp);
	EE_WriteVariable(EE_VADDR_MEM3_TIME, rom_mem.rom_str.memory3_time);
}
void rom_wite_PID_data(uint8_t P, uint8_t I, uint8_t D){
	rom_mem.rom_str.PID_P = P;
	rom_mem.rom_str.PID_I = I;
	rom_mem.rom_str.PID_D = D;
	EE_WriteVariable(EE_VADDR_PID_P, rom_mem.rom_str.PID_P);
	EE_WriteVariable(EE_VADDR_PID_I, rom_mem.rom_str.PID_I);
	EE_WriteVariable(EE_VADDR_PID_D, rom_mem.rom_str.PID_D);
}

void rom_wite_new_set_data(uint8_t ui_time_hours, uint8_t ui_time_minutes , double ui_temp)
{
	rom_mem.rom_str.memory3_temp = rom_mem.rom_str.memory2_temp;
	rom_mem.rom_str.memory3_time = rom_mem.rom_str.memory2_time;
	rom_mem.rom_str.memory2_temp = rom_mem.rom_str.memory1_temp;
	rom_mem.rom_str.memory2_time = rom_mem.rom_str.memory1_time;
	rom_mem.rom_str.memory1_temp = (uint16_t) ui_temp*10;
	rom_mem.rom_str.memory1_time = (uint16_t) (ui_time_hours*100 + ui_time_minutes);
	EE_WriteVariable(EE_VADDR_MEM1_TEMP, rom_mem.rom_str.memory1_temp);
	EE_WriteVariable(EE_VADDR_MEM1_TIME, rom_mem.rom_str.memory1_time);
	EE_WriteVariable(EE_VADDR_MEM2_TEMP, rom_mem.rom_str.memory2_temp);
	EE_WriteVariable(EE_VADDR_MEM2_TIME, rom_mem.rom_str.memory2_time);
	EE_WriteVariable(EE_VADDR_MEM3_TEMP, rom_mem.rom_str.memory3_temp);
	EE_WriteVariable(EE_VADDR_MEM3_TIME, rom_mem.rom_str.memory3_time);
}

double rom_get_temp(uint8_t mem_no)
{
	if(mem_no == 1)
	{
		return ((double)rom_mem.rom_str.memory1_temp)/10;
	}
	else if(mem_no == 2)
	{
		return ((double)rom_mem.rom_str.memory2_temp)/10;
	}
	else if(mem_no == 3)
	{
		return ((double)rom_mem.rom_str.memory3_temp)/10;
	}
	else
	{
		return -1;
	}
}
uint8_t rom_get_time_hours(uint8_t mem_no)
{
	if(mem_no == ROM_MEM1)
	{
		return (uint8_t)(rom_mem.rom_str.memory1_time/100);
	}
	else if(mem_no == ROM_MEM2)
	{
		return (uint8_t)(rom_mem.rom_str.memory2_time/100);
	}
	else if(mem_no == ROM_MEM3)
	{
		return (uint8_t)(rom_mem.rom_str.memory3_time/100);
	}
	else
	{
		return 254;
	}
}

uint8_t rom_get_time_minutes(uint8_t mem_no)
{
	if(mem_no == 1)
	{
		return (uint8_t)(rom_mem.rom_str.memory1_time%100);
	}
	else if(mem_no == 2)
	{
		return (uint8_t)(rom_mem.rom_str.memory2_time%100);
	}
	else if(mem_no == 3)
	{
		return (uint8_t)(rom_mem.rom_str.memory3_time%100);
	}
	else
	{
		return 254;
	}
}

void rom_mem2_exec()
{
	uint16_t temp_temperature,temp_time;
	temp_temperature = rom_mem.rom_str.memory2_temp;
	temp_time = rom_mem.rom_str.memory2_time;
	rom_mem.rom_str.memory2_temp = rom_mem.rom_str.memory1_temp;
	rom_mem.rom_str.memory2_time = rom_mem.rom_str.memory1_time;
	rom_mem.rom_str.memory1_temp = temp_temperature;
	rom_mem.rom_str.memory1_time = temp_time;
	EE_WriteVariable(EE_VADDR_MEM1_TEMP, rom_mem.rom_str.memory1_temp);
	EE_WriteVariable(EE_VADDR_MEM1_TIME, rom_mem.rom_str.memory1_time);
	EE_WriteVariable(EE_VADDR_MEM2_TEMP, rom_mem.rom_str.memory2_temp);
	EE_WriteVariable(EE_VADDR_MEM2_TIME, rom_mem.rom_str.memory2_time);
}
void rom_mem3_exec()
{
	uint16_t temp_temperature,temp_time;
	temp_temperature = rom_mem.rom_str.memory3_temp;
	temp_time = rom_mem.rom_str.memory3_time ;
	rom_mem.rom_str.memory3_temp = rom_mem.rom_str.memory1_temp;
	rom_mem.rom_str.memory3_time = rom_mem.rom_str.memory1_time;
	rom_mem.rom_str.memory2_temp = rom_mem.rom_str.memory1_temp;
	rom_mem.rom_str.memory2_time = rom_mem.rom_str.memory1_time;
	rom_mem.rom_str.memory1_temp = temp_temperature;
	rom_mem.rom_str.memory1_time = temp_time;
	EE_WriteVariable(EE_VADDR_MEM1_TEMP, rom_mem.rom_str.memory1_temp);
	EE_WriteVariable(EE_VADDR_MEM1_TIME, rom_mem.rom_str.memory1_time);
	EE_WriteVariable(EE_VADDR_MEM2_TEMP, rom_mem.rom_str.memory2_temp);
	EE_WriteVariable(EE_VADDR_MEM2_TIME, rom_mem.rom_str.memory2_time);
	EE_WriteVariable(EE_VADDR_MEM3_TEMP, rom_mem.rom_str.memory3_temp);
	EE_WriteVariable(EE_VADDR_MEM3_TIME, rom_mem.rom_str.memory3_time);
}




