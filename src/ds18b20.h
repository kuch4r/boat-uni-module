/* ds18b20.h - a part of avr-ds18b20 library
 *
 * Copyright (C) 2016 Jacek Wieczorek
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.	See the LICENSE file for details.
 */

#ifndef DS18B20_H
#define DS18B20_H

#include <inttypes.h>
#include "onewire.h"

#define TEMP_CHANNEL_1 0
#define TEMP_CHANNEL_2 1
#define TEMP_CHANNEL_3 2
#define TEMP_CHANNEL_4 3

#define DS18B20_ERROR_OK	0
#define DS18B20_ERROR_COMM	1
#define DS18B20_ERROR_CRC	2
#define DS18B20_ERROR_PULL	3
#define DS18B20_ERROR_OTHER 4
#define DS18B20_OK DS18B20_ERROR_OK

#define DS18B20_COMMAND_READ_ROM 0x33
#define DS18B20_COMMAND_MATCH_ROM 0x55
#define DS18B20_COMMAND_SKIP_ROM 0xCC
#define DS18B20_COMMAND_CONVERT 0x44
#define DS18B20_COMMAND_WRITE_SP 0x4E
#define DS18B20_COMMAND_READ_SP 0xBE
#define DS18B20_COMMAND_COPY_SP 0x48

#define DS18B20_RES09 ( 0 << 5 )
#define DS18B20_RES10 ( 1 << 5 )
#define DS18B20_RES11 ( 2 << 5 )
#define DS18B20_RES12 ( 3 << 5 )

#define DS18B20_MUL 16

#define TEMP_MAX_CHANNEL 4

#define BEGIN_CONVERSION 0
#define WAITING_FOR_CONVERSION 1
#define READ_REUSLTS 2
#define CONVERT_RESULTS 3

#define DO_NOT_CHECK_ID NULL

void ds18b20_init(uint8_t act_ch);
void ds18b20_task(void);
extern uint8_t ds18b20convert( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *rom );
extern uint8_t ds18b20rsp( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *rom, uint8_t *sp );
extern uint8_t ds18b20wsp( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *rom, uint8_t th, uint8_t tl, uint8_t conf );
extern uint8_t ds18b20csp( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *rom );
extern uint8_t ds18b20read( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *rom, int16_t *temperature ) ;
extern uint8_t ds18b20rom( volatile uint8_t *port, volatile uint8_t *direction, volatile uint8_t *portin, uint8_t mask, uint8_t *rom );

struct temp_data {
	int16_t raw_temp_in[TEMP_MAX_CHANNEL];
	int16_t temp_in[TEMP_MAX_CHANNEL];
};

struct temp_state {
	uint8_t active_channels[TEMP_MAX_CHANNEL];
	uint8_t state;
	uint8_t active_channel;
	uint16_t time_stamp;
};

struct temp_state temp_state;
struct temp_data temp_results;


#endif
