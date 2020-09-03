/*
 * crc8.c
 *
 *  Created on: 11 maj 2020
 *      Author: kowma
 */
#include "main.h"
#include "ds18b20.h"

unsigned char crc8(void const *mem, size_t len) {
	uint8_t crc = 0;
	unsigned char const *data = mem;

		while (len--) {
			uint8_t inbyte = *data++;
			for (uint8_t i = 8; i; i--) {
				uint8_t mix = (crc ^ inbyte) & 0x01;
				crc >>= 1;
				if (mix) crc ^= 0x8C;
				inbyte >>= 1;
			}
		}
		return crc;
}

