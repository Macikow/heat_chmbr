/*
 * ds18b20.c
 *
 *  Created on: 7 maj 2020
 *      Author: kowma
 */
#include "main.h"
#include "ds18b20.h"
#include "delay.h"


#define ONE_WIRE_BUS_INPUT	1
#define ONE_WIRE_BUS_OUTPUT 2


// clk = 72 Mhz each nop = 6 cycle 72 / 6 = 12Mhz


volatile uint8_t ds18b20_sensors_amount = 0;
//uint8_t ROM_FAMILY_NO[8] ;
//uint8_t ROM_CRC[8] ;uint8_t ds18b20_sensors_amount = 0;
//uint32_t ROM_NO_FIRST_HALF[8]; //  first 32 bits
//uint16_t ROM_NO_SECOUND_HALF[8]; // last 16 bits
volatile struct __attribute__((__packed__)) ds18b20_address{
	uint8_t ROM_FAMILY_NO ;
	uint32_t ROM_NO_FIRST_HALF; //  first 32 bits
	uint16_t ROM_NO_SECOUND_HALF; // last 16 bits
	uint8_t ROM_CRC ;
}rom_address[MAX_DS18B20_SENSORS_AMOUNT];

volatile struct __attribute__((__packed__)) ds18b20_Scratchpad{
	uint16_t temperature ;
	uint16_t user_byte_or_temperature_register;
	uint8_t configuration_register;
	uint8_t reserverd[3];
	uint8_t crc;
}scratchpad[MAX_DS18B20_SENSORS_AMOUNT];



void oneWire_direction_set(uint8_t direction)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	if(direction == ONE_WIRE_BUS_INPUT)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Set as input
		GPIO_InitStruct.Pull = GPIO_NOPULL; // No pullup - the pullup resistor is external
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; // Medium GPIO frequency
		GPIO_InitStruct.Pin = ONE_WIRE_DATA1_Pin ;// Pin for 1-Wire bus
		HAL_GPIO_Init(ONE_WIRE_DATA1_GPIO_Port, &GPIO_InitStruct); // Reinitialize
	}
	else if(direction ==  ONE_WIRE_BUS_OUTPUT)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // Set as open-drain output
		GPIO_InitStruct.Pull = GPIO_NOPULL; // No pullup - the pullup resistor is external
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; // Medium GPIO frequency
		GPIO_InitStruct.Pin = ONE_WIRE_DATA1_Pin; // Pin for 1-Wire bus
		HAL_GPIO_Init(ONE_WIRE_DATA1_GPIO_Port, &GPIO_InitStruct); // Reinitialize
	}
	else
	{
		// error
	}

}

uint8_t oneWire_init(void)
{
	uint8_t i;
	//TIM2->CR1 |= (1<<3); //OPM
	oneWire_direction_set(ONE_WIRE_BUS_OUTPUT);
	ONE_WIRE_DATA1_GPIO_Port->ODR &= ~ONE_WIRE_DATA1_Pin;
	simple_delay_us(500);

	oneWire_direction_set(ONE_WIRE_BUS_INPUT);
	simple_delay_us(70);

	if(ONE_WIRE_DATA1_GPIO_Port->IDR & ONE_WIRE_DATA1_Pin) // if 0 - then there is devices on bus
	{
		i = ONE_WIRE_PRESENCE_FAIL;
	}
	else i = ONE_WIRE_PRESENCE_SUCCESS;
	simple_delay_us(410);
	return i;
}

void oneWire_writeBit(uint8_t value)
{
	ONE_WIRE_DATA1_GPIO_Port->ODR &= ~ONE_WIRE_DATA1_Pin; // todo check
	oneWire_direction_set(ONE_WIRE_BUS_OUTPUT);
	if(value)
	{
		simple_delay_us(6);
		oneWire_direction_set(ONE_WIRE_BUS_INPUT);
		simple_delay_us(64);
	}
	else
	{
		simple_delay_us(60);
		oneWire_direction_set(ONE_WIRE_BUS_INPUT);
		simple_delay_us(10);
	}
}
void oneWire_writeByte(uint8_t byte)
{
	oneWire_direction_set(ONE_WIRE_BUS_OUTPUT);
	ONE_WIRE_DATA1_GPIO_Port->ODR &= ~ONE_WIRE_DATA1_Pin;
	for(uint8_t i=0; i<8; i++){
		oneWire_writeBit(byte & 1<<i);
	}
}

uint8_t oneWire_readbit(void)
{
	oneWire_direction_set(ONE_WIRE_BUS_OUTPUT);
	ONE_WIRE_DATA1_GPIO_Port->ODR &= ~ONE_WIRE_DATA1_Pin;
	simple_delay_us(2);
	oneWire_direction_set(ONE_WIRE_BUS_INPUT);
	simple_delay_us(10);
	if(ONE_WIRE_DATA1_GPIO_Port->IDR & ONE_WIRE_DATA1_Pin)
	{
		simple_delay_us(60);
		return 1;
	}
	else{
		simple_delay_us(60);
		return 0;
	}
}

uint8_t oneWire_readbyte(void)
{
	uint8_t data = 0;
	for(uint8_t i = 0; i< 8 ; i++)
	{
		if(oneWire_readbit()) data |= (1 << i);
	}
	return data;
}


uint8_t oneWire_deviceCounter(void)
{
	uint8_t true_bit, complement_bit, devices_counter = 0;
	if(oneWire_init())
	{
		devices_counter++;
	}
	oneWire_writeByte(0xF0);
	for(uint8_t i=0; i<64; i++)
	{
		true_bit = oneWire_readbit();
		complement_bit = oneWire_readbit();
		if(true_bit == 0 && complement_bit ==0 ) devices_counter++;
		oneWire_writeBit(true_bit);
	}
	return devices_counter;
}

void write_data_to_struct(uint8_t bit_cnt, uint8_t bit_data, uint8_t deviceNo)
{
	if(bit_cnt < 8) // write to family no
	{
		 if(bit_data) rom_address[deviceNo].ROM_FAMILY_NO |= (1 << bit_cnt);
	}
	else if( (bit_cnt >= 8) && (bit_cnt < 40)) // Serial no first half
	{
		if(bit_data) rom_address[deviceNo].ROM_NO_FIRST_HALF |= (1 << (bit_cnt - 8));
	}
	else if( (bit_cnt >= 40)  && (bit_cnt < 56) )  // Serial no first half
	{
		if(bit_data) rom_address[deviceNo].ROM_NO_SECOUND_HALF |= (1 << (bit_cnt - 40 ));
	}
	else if( bit_cnt >= 56) // CRC
	{
		if(bit_data) rom_address[deviceNo].ROM_CRC |= (1 << (bit_cnt - 56));
	}
}

//void write_data_to_tables(uint8_t bit_cnt, uint8_t bit_data, uint8_t deviceNo)
//{
//	if(bit_cnt < 8) // write to family no
//	{
//		 if(bit_data)  ROM_FAMILY_NO[deviceNo] |= (1 << bit_cnt);
//	}
//	else if( (bit_cnt >= 8) && (bit_cnt < 40)) // Serial no first half
//	{
//		if(bit_data)  ROM_NO_FIRST_HALF[deviceNo] |= (1 << (bit_cnt - 8));
//	}
//	else if( (bit_cnt >= 40)  && (bit_cnt < 56) )  // Serial no first half
//	{
//		if(bit_data)  ROM_NO_SECOUND_HALF[deviceNo] |= (1 << (bit_cnt - 40 ));
//	}
//	else if( bit_cnt >= 56) // CRC
//	{
//		if(bit_data) ROM_CRC[deviceNo] |= (1 << (bit_cnt - 56));
//	}
//}

uint8_t read_address_bit_form_struct(uint8_t bit_cnt, uint8_t deviceNo)
{
	if(bit_cnt < 8) // write to family no
	{
		 return ((rom_address[deviceNo].ROM_FAMILY_NO & (1 << bit_cnt))> 0);
	}
	else if( (bit_cnt >= 8) && (bit_cnt < 40)) // Serial no first half
	{
		return ((rom_address[deviceNo].ROM_NO_FIRST_HALF & ( 1 << (bit_cnt - 8) )) > 0);
	}
	else if( (bit_cnt >= 40)  && (bit_cnt < 56) ) // Serial no first half
	{
		return ((rom_address[deviceNo].ROM_NO_SECOUND_HALF & (1 << (bit_cnt - 40) )) > 0);
	}
	else if( bit_cnt >= 56) // CRC
	{
		return ((rom_address[deviceNo].ROM_CRC & (1 << (bit_cnt - 56) )) > 0) ;
	}
	else // not used // TODO add asset
	{
		return 0;
	}

}


//uint8_t read_previous_address_bit(uint8_t bit_cnt, uint8_t deviceNo)
//{
//	if(bit_cnt < 8) // write to family no
//	{
//		 return ((ROM_FAMILY_NO[deviceNo] & (1 << bit_cnt))> 0);
//	}
//	else if( (bit_cnt >= 8) && (bit_cnt < 40)) // Serial no first half
//	{
//		return ((ROM_NO_FIRST_HALF[deviceNo] & ( 1 << (bit_cnt - 8) )) > 0);
//	}
//	else if( (bit_cnt >= 40)  && (bit_cnt < 56) ) // Serial no first half
//	{
//		return ((ROM_NO_SECOUND_HALF[deviceNo] & (1 << (bit_cnt - 40) )) > 0);
//	}
//	else if( bit_cnt >= 56) // CRC
//	{
//		return ((ROM_CRC[deviceNo] & (1 << (bit_cnt - 56) )) > 0) ;
//	}
//}


uint8_t ds18b20_check_crc(uint8_t DS18B20_sensor_no)
{
	uint8_t sensor_crc;
	sensor_crc = crc8(&rom_address[DS18B20_sensor_no], 7);
	if(sensor_crc == rom_address[DS18B20_sensor_no].ROM_CRC) return DS18B20_CRC_ACK_OK;
	else return DS18B20_CRC_ACK_FAIL;

}


/***
 * searching algorithm based on :
 * https://www.maximintegrated.com/en/design/technical-documents/app-notes/1/187.html
 */
uint8_t ds18b20_search_rom(void)
{
	uint8_t bit_counter = 0, true_bit = 0, complement_bit = 0, devices_counter = 0 ;
	uint8_t lastDiscrepancy = 0, searchDirection = 0, lastZero = 0; // może byc 0 bo na pierwszych 8 pozycjach zapisana jest rodzina urządzeń
	do{
		if(oneWire_init() == ONE_WIRE_PRESENCE_FAIL) return ROM_SARCHING_ERROR;

		// there is at least one sensor - someone send presensce ack
		oneWire_writeByte(0xF0);
		for(bit_counter = 0 ; bit_counter < 64; bit_counter++)
		{
			true_bit = oneWire_readbit();
			complement_bit = oneWire_readbit();
			if( ( true_bit ==  0 ) && (complement_bit == 0) ) {

				if((bit_counter + 1) ==  lastDiscrepancy)
				{
					searchDirection = 1;
				}
				else if((bit_counter + 1) > lastDiscrepancy )
				{
					searchDirection = 0;

				}
				else // (bit_counter + 1) < lastDiscrepancy
				{
					searchDirection = read_address_bit_form_struct(bit_counter, devices_counter-1);
				}

				oneWire_writeBit(searchDirection);
				write_data_to_struct(bit_counter, searchDirection, devices_counter);
				//write_data_to_tables(bit_counter, searchDirection, devices_counter); // ROM tables


				searchDirection ? (lastZero = lastZero) : (lastZero = bit_counter+1);
			}
			else if( true_bit != complement_bit)
			{
				write_data_to_struct(bit_counter, true_bit, devices_counter);
				//write_data_to_tables(bit_counter, true_bit, devices_counter); // ROM tables
				oneWire_writeBit(true_bit);
			}
			else{
				return ROM_SARCHING_ERROR;
			}
		}
		lastDiscrepancy = lastZero;
		lastZero = 0;
		if(lastDiscrepancy != 0)
		{
			devices_counter++  ;
		}

	}while(lastDiscrepancy != 0);

	ds18b20_sensors_amount = devices_counter + 1; //counter from 0 to device amount - 1


	// check CRC codes ..

	for(uint8_t founded_sensor_no = 0; founded_sensor_no < ds18b20_sensors_amount ; founded_sensor_no++)
	{
		if(ds18b20_check_crc(founded_sensor_no) == DS18B20_CRC_ACK_FAIL)
		{
			return DS18B20_CRC_ACK_FAIL;
		}
	}
	return ROM_SARCHING_OK;
}


uint8_t ds18b20_convertT(void) // for all sensor // there is no reason to convert only one sensor
{
	uint8_t convertion_time_counter = 300 / 5; // TODO jeśli zmienimy daomyślną rozdzielczość licznik można zmiejszyć
	if(!oneWire_init()) return 0;

	oneWire_writeByte(DS18B20_COMMAND_SKIP_ROM);
	oneWire_writeByte(DS18B20_COMMAND_CONVERT_T);
	oneWire_direction_set(ONE_WIRE_BUS_INPUT);
	simple_delay_ms(80);
	while(!(ONE_WIRE_DATA1_GPIO_Port->IDR & ONE_WIRE_DATA1_Pin))
	{
		simple_delay_ms(5);
		if(convertion_time_counter-- == 0) return CONVERTION_ERROR;
	}
	return CONVERTION_OK;
}

uint8_t ds18b20_read_scratchpad(uint8_t sensorNumber)
{
	if(!oneWire_init()) return ONE_WIRE_PRESENCE_FAIL;

	uint8_t *address  = (uint8_t*) &rom_address[sensorNumber];
	uint8_t *data  = (uint8_t*) &scratchpad[sensorNumber];


	if(ds18b20_sensors_amount == 1) // TODO do przetesotwania z jednym sensorem
	{
		oneWire_writeByte(DS18B20_COMMAND_SKIP_ROM);
	}
	else if(ds18b20_sensors_amount > 1)
	{
		oneWire_writeByte(DS18B20_COMMAND_MATCH_ROM);
		for(uint8_t i=0; i< 8 ; i++)
		{
			oneWire_writeByte(*(address++));
		}
	}
	else // nie dozwolony stan
	{
		return READ_ERROR;
	}

	oneWire_writeByte(DS18B20_COMMAND_READ_SCRATCHPAD);
	for(uint8_t i=0; i<9 ; i++)
	{
		*(data + i)= oneWire_readbyte();
	}
	// TODO  check CRC
	return READ_OK;
}

uint8_t ds18b20_write_scratchpad(uint8_t sensorNumber, uint16_t temp_alarm, uint8_t resolution)
{
	if(!oneWire_init()) return ONE_WIRE_PRESENCE_FAIL;
	if(resolution > 3) return WRITE_ERROR;
	if(sensorNumber > ds18b20_sensors_amount-1) return WRITE_ERROR;

	uint8_t *address  = (uint8_t*) &rom_address[sensorNumber];
	uint8_t *data  = (uint8_t*) ( &scratchpad[sensorNumber]);
	scratchpad[sensorNumber].user_byte_or_temperature_register = temp_alarm;
	scratchpad[sensorNumber].configuration_register = (0x1f | (resolution << 5));

	if(ds18b20_sensors_amount == 1) // TODO do przetesotwania z jednym sensorem
	{
		oneWire_writeByte(DS18B20_COMMAND_SKIP_ROM);
	}
	else if(ds18b20_sensors_amount > 1)
	{
		oneWire_writeByte(DS18B20_COMMAND_MATCH_ROM);
		for(uint8_t i=0; i< 8 ; i++)
		{
			oneWire_writeByte(*(address++));
		}
	}
	else // nie dozwolony stan
	{
		return WRITE_ERROR;
	}

	oneWire_writeByte(DS18B20_COMMAND_WRITE_SCRATCHPAD);
	for(uint8_t i=2; i<5 ; i++)
	{
		oneWire_writeByte(*(data+i));
	}
	// TODO  check CRC
	return WRITE_OK;
}

uint8_t ds18b20_copy_scratchpad(uint8_t sensorNumber)
{
	uint8_t convertion_time_counter = 5;
	if(!oneWire_init()) return ONE_WIRE_PRESENCE_FAIL;

	uint8_t *address  = (uint8_t*) &rom_address[sensorNumber];
	uint8_t *data  = (uint8_t*) &scratchpad[sensorNumber];


	if(ds18b20_sensors_amount == 1) // TODO do przetesotwania z jednym sensorem
	{
		oneWire_writeByte(DS18B20_COMMAND_SKIP_ROM);
	}
	else if(ds18b20_sensors_amount > 1)
	{
		oneWire_writeByte(DS18B20_COMMAND_MATCH_ROM);
		for(uint8_t i=0; i< 8 ; i++)
		{
			oneWire_writeByte(*(address++));
		}
	}
	else // nie dozwolony stan
	{
		return WRITE_ERROR;
	}

	oneWire_writeByte(DS18B20_COMMAND_COPY_SCRATCHPAD);
	while(!(ONE_WIRE_DATA1_GPIO_Port->IDR & ONE_WIRE_DATA1_Pin))
	{
		simple_delay_ms(5);
		if(convertion_time_counter-- == 0) return CONVERTION_ERROR;
	}
	return READ_OK;
}

uint8_t ds18b20_set_default_resolution(uint8_t sensorNumber)
{
	if(ds18b20_write_scratchpad(sensorNumber, 0xf0f0, DS18B20_RESOLUTION_11BIT) == READ_ERROR) return READ_ERROR;
	if(ds18b20_copy_scratchpad(sensorNumber) == READ_ERROR) return READ_ERROR;
	return READ_OK;
}


uint16_t ds18b20_get_scratchpad_temperature(uint8_t sensor_number)
{
	uint16_t temperature;
	if(ds18b20_convertT() == CONVERTION_ERROR) return CONVERTION_ERROR;
	if(ds18b20_read_scratchpad(sensor_number) != READ_OK ) return GET_TEMPERATURE_ERROR;
	temperature = scratchpad[sensor_number].temperature;
	return temperature;
}
uint16_t ds18b20_get_scratchpad_userBytes(uint8_t sensor_number)
{
	uint16_t user_bytes;
	if(ds18b20_read_scratchpad(sensor_number) != READ_OK ) return GET_TEMPERATURE_ERROR;
	user_bytes =  scratchpad[sensor_number].user_byte_or_temperature_register;
	return user_bytes;
}
uint8_t ds18b20_get_scratchpad_config(uint8_t sensor_number)
{
	uint8_t config;
	if(ds18b20_read_scratchpad(sensor_number) != READ_OK ) return GET_CONFIG_ERROR;
	config = scratchpad[sensor_number].configuration_register;
	return config;
}

uint8_t ds18b20_get_scratchpad_crc(uint8_t sensor_number)
{
	uint8_t crc;
	if(ds18b20_read_scratchpad(sensor_number) != READ_OK ) return GET_CONFIG_ERROR;
	crc = scratchpad[sensor_number].crc;
	return crc;
}

void delay_check(void)
{
	for(uint16_t i =0; i< 1000; i++)
	{
		LED_RED_GPIO_Port->BSRR = LED_RED_Pin;
		simple_delay_us(i);
		LED_RED_GPIO_Port->BRR = LED_RED_Pin;
		simple_delay_us(i);
	}
}

void convert_T_to_string_hex(char* temperature_str_hex, uint8_t sensorNumber)
{
	uint16_t i=10000;
	uint16_t temperature;
	uint8_t digit_counter = 0;
	const float mux = 0.0625 * 100 ;
	temperature = ds18b20_get_scratchpad_temperature(sensorNumber);
	if(temperature > 0xf000) // negative
	{
		temperature = 0x10000 - temperature;
		*(temperature_str_hex++) = '-';
	}
	temperature = (uint16_t)(temperature * mux);
	if(temperature == 0)
	{
		*(temperature_str_hex++) = '0';
		*(temperature_str_hex++) = '.';
		*(temperature_str_hex++) = '0';
		*(temperature_str_hex++) = '0';
		*(temperature_str_hex++) = ' ';
		*(temperature_str_hex++) = ' ';
		*(temperature_str_hex++) = ' ';
		*(temperature_str_hex++) = '\0';
		return ;
	}
	while((temperature / i) == 0)
	{
		i = i/10;
		digit_counter++;
	}
	if(digit_counter >= 3) // ułamek zero przed kropką
	{
		*(temperature_str_hex++) = '0';
		*(temperature_str_hex++) = '.';
	}
	while(i != 0 )
	{
		*(temperature_str_hex++) = 0x30 + temperature/i;
		temperature = (uint16_t)temperature % i;
		if(i == 100) *(temperature_str_hex++) = '.';
		i = i/10;
	}
	while(digit_counter--)
	{
		*(temperature_str_hex++) = ' ';
	}
	*(temperature_str_hex++) = '\0';
}

void ds18b20_initalize(void)
{
	ds18b20_search_rom();
	HC_status.ds18b20_amount = ds18b20_sensors_amount;
}

//uint8_t ds18b20_search_rom_only_one_sensor_in_bus(void)
//{
//	uint8_t  true_bit = 0, complement_bit = 0;
//	if(oneWire_init() == ONE_WIRE_PRESENCE_FAIL) return 0;
//	oneWire_writeByte(0xF0);
//	for(uint8_t bit_counter =0; bit_counter<64;bit_counter++)
//	{
//		true_bit = oneWire_readbit();
//		complement_bit = oneWire_readbit();
//		if(true_bit == complement_bit) return 0;
//		else{
//			oneWire_writeBit(true_bit);
//			true_bit ? (ROM_NO[0] |= (1 << bit_counter)) : (ROM_NO[0] &= ~(1 << bit_counter));
//		}
//	}
//	return 1;
//}

