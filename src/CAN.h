/*
 * CAN.h
 *
 * Created: 2013-02-03 14:16:41
 *  Author: LukaszKucharski
 */ 

//CAN id 0x03
#include "ADC.h"

#ifndef CAN_H_
#define CAN_H_

void CAN_init(uint8_t);
void CAN_task(void);

void CAN_send_SDO(void);
#define RECEPTION 1<<CONMOB1
#define TRANSMISSION 1<<CONMOB0

#define CAN_FLAG_INTERRUPT 0
#define CAN_FLAG_SYNC_RECEIVED 1
#define CAN_FLAG_UPDATE_DATA 2
#define CAN_FLAG_SDO_RECEIVED 3
#define CAN_FLAG_COMMAND_FAILED 4
#define CAN_FLAG_DATA_LENGTH_8 4
#define CAN_FLAG_DATA_LENGTH_16 5
#define CAN_FLAG_DATA_LENGTH_32 6

#define SDO_READ_COMMAND 0x40
#define SDO_WRITE_COMMAND 0x23

#define SDO_READ_POSITIVE_RESPONSE_16 MOb_data[MOb_SDO_TX][0] = 0x4B;\
MOb_data[MOb_SDO_TX][1] = MOb_data[MOb_SDO_RX][1];\
MOb_data[MOb_SDO_TX][2] = MOb_data[MOb_SDO_RX][2];\
MOb_data[MOb_SDO_TX][3] = MOb_data[MOb_SDO_RX][3];\
MOb_data[MOb_SDO_TX][6] = 0x00;\
MOb_data[MOb_SDO_TX][7] = 0x00;

#define SDO_READ_POSITIVE_RESPONSE_8 MOb_data[MOb_SDO_TX][0] = 0x4F;\
MOb_data[MOb_SDO_TX][1] = MOb_data[MOb_SDO_RX][1];\
MOb_data[MOb_SDO_TX][2] = MOb_data[MOb_SDO_RX][2];\
MOb_data[MOb_SDO_TX][3] = MOb_data[MOb_SDO_RX][3];\
MOb_data[MOb_SDO_TX][5] = 0x00;\
MOb_data[MOb_SDO_TX][6] = 0x00;\
MOb_data[MOb_SDO_TX][7] = 0x00;

#define SDO_NEGATIVE_RESPONSE MOb_data[MOb_SDO_TX][0] = 0x80;\
MOb_data[MOb_SDO_TX][1] = MOb_data[MOb_SDO_RX][1];\
MOb_data[MOb_SDO_TX][2] = MOb_data[MOb_SDO_RX][2];\
MOb_data[MOb_SDO_TX][3] = MOb_data[MOb_SDO_RX][3];\
MOb_data[MOb_SDO_TX][4] = 0x00;\
MOb_data[MOb_SDO_TX][5] = 0x00;\
MOb_data[MOb_SDO_TX][6] = 0x00;\
MOb_data[MOb_SDO_TX][7] = 0x00;

#define SDO_WRITE_POSITIVE_RESPONSE MOb_data[MOb_SDO_TX][0] = 0x60;\
MOb_data[MOb_SDO_TX][1] = MOb_data[MOb_SDO_RX][1];\
MOb_data[MOb_SDO_TX][2] = MOb_data[MOb_SDO_RX][2];\
MOb_data[MOb_SDO_TX][3] = MOb_data[MOb_SDO_RX][3];\
MOb_data[MOb_SDO_TX][4] = 0x00;\
MOb_data[MOb_SDO_TX][5] = 0x00;\
MOb_data[MOb_SDO_TX][6] = 0x00;\
MOb_data[MOb_SDO_TX][7] = 0x00;

#define MOb_0 0
#define MOb_1 1
#define MOb_2 2
#define MOb_3 3
#define MOb_4 4
#define MOb_5 5

#define MOb_SDO_TX MOb_4
#define MOb_SDO_RX MOb_5

//Sync - odbiera
#define SYNC_ID 0x080
//id bazowe - mo¿na zwiêkszaæ DIP SW
#define BASE_ID 0x10

#define SDO_RX_COMMAND MOb_data[MOb_SDO_RX][0]
#define SDO_RX_INDEX ((((uint16_t)MOb_data[MOb_SDO_RX][1])) | (MOb_data[MOb_SDO_RX][2]<<8))
#define SDO_RX_SUBINDEX MOb_data[MOb_SDO_RX][3]
#define SDO_TX_DATA_1 MOb_data[MOb_SDO_TX][4]
#define SDO_TX_DATA_2 MOb_data[MOb_SDO_TX][5]

#define SDO_RX_DATA_1 MOb_data[MOb_SDO_RX][4]
#define SDO_RX_DATA_2 MOb_data[MOb_SDO_RX][5]

struct CAN_str {
	int16_t in[ADC_MAX_CHANNEL];
		
	uint8_t diag_1;
	uint8_t diag_2;
};

struct can_state {
	uint8_t flags;
	
};


#endif /* CAN_H_ */
