/*
 * ds18b20.h
 *
 *  Created on: 7 maj 2020
 *      Author: kowma
 */

#ifndef DS18B20_DS18B20_H_
#define DS18B20_DS18B20_H_

#define MAX_DS18B20_SENSORS_AMOUNT 8
#define ONE_WIRE_PRESENCE_SUCCESS 1
#define ONE_WIRE_PRESENCE_FAIL 0


#define DS18B20_SEARCHING_BRANCH 2


#define DS18B20_CRC_ACK_FAIL	0
#define DS18B20_CRC_ACK_OK		1

#define ROM_SARCHING_ERROR 0
#define ROM_SARCHING_OK 1

#define CONVERTION_ERROR 0
#define CONVERTION_OK 1

#define READ_ERROR 0
#define READ_OK 1

#define WRITE_ERROR 0
#define WRITE_OK 1

#define GET_TEMPERATURE_ERROR 0xffff
#define GET_CONFIG_ERROR 0xff


#define DS18B20_RESOLUTION_9BIT		0
#define DS18B20_RESOLUTION_10BIT	1
#define DS18B20_RESOLUTION_11BIT	2
#define DS18B20_RESOLUTION_12BIT	3



#define DS18B20_COMMAND_SEARCH_ROM 			0xf0
#define DS18B20_COMMAND_READ_ROM 			0x33
#define DS18B20_COMMAND_MATCH_ROM			0x55
#define DS18B20_COMMAND_SKIP_ROM			0xCC
#define DS18B20_COMMAND_ALARM_SEARCH	 	0xEC
#define DS18B20_COMMAND_CONVERT_T 			0x44
#define DS18B20_COMMAND_WRITE_SCRATCHPAD 	0x4e
#define DS18B20_COMMAND_READ_SCRATCHPAD 	0xbe
#define DS18B20_COMMAND_COPY_SCRATCHPAD 	0x48
#define DS18B20_COMMAND_RECALL_EEPROM	 	0xb8
#define DS18B20_COMMAND_READ_POWER	 		0xb4



void delay_check(void);
uint8_t oneWire_init(void);
void oneWire_writeBit(uint8_t value);
void oneWire_writeByte(uint8_t byte);
uint8_t oneWire_readbit(void);
uint8_t ds18b20_search_rom(void);
uint8_t ds18b20_read_scratchpad(uint8_t sensorNumber);
uint8_t ds18b20_write_scratchpad(uint8_t sensorNumber, uint16_t temp_alarm, uint8_t resolution);
uint8_t ds18b20_copy_scratchpad(uint8_t sensorNumber);
uint8_t ds18b20_set_default_resolution(uint8_t sensorNumber);

uint16_t ds18b20_get_scratchpad_temperature(uint8_t sensor_number);
uint16_t ds18b20_get_scratchpad_userBytes(uint8_t sensor_number);
uint16_t ds18b20_get_scratchpad_temperature(uint8_t sensor_number);
uint8_t ds18b20_get_scratchpad_config(uint8_t sensor_number);
uint8_t ds18b20_get_scratchpad_crc(uint8_t sensor_number);


unsigned char crc8(void const *mem, size_t len);

void convert_T_to_string_hex(char* temperature_str_hex, uint8_t sensorNumber);

void ds18b20_initalize(void);


#endif /* DS18B20_DS18B20_H_ */
