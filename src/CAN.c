
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "main.h"
#include "CAN.h"
#include "uart.h"
#include "makra.h"
#include "ADC.h"
#include "EEPROM.h"
#include "ds18b20.h"


volatile uint8_t MOb_data[6][8];
volatile uint8_t device_id;

struct CAN_str CAN;
volatile struct can_state can_state;
extern struct eeprom_struct non_volatile_data;
extern uint8_t active_channels[];

volatile uint16_t int_counter=0;


//Inicjalizacja CAN
void CAN_init(uint8_t dip_sw){
	uint16_t mob_0_id = SYNC_ID;		//SYNC
	uint16_t mob_1_id = 0x200 + BASE_ID; //RPDO 1
	uint16_t mob_2_id = 0x180 + BASE_ID; //TPDO 1
	uint16_t mob_3_id = 0x280 + BASE_ID; //TPDO 2
	uint16_t mob_4_id = 0x580 + BASE_ID; //SDO TX
	uint16_t mob_5_id = 0x600 + BASE_ID; //SDO RX
	
	//dodwania do adresu ustawienia z DIP switchy
	mob_1_id += dip_sw;
	mob_2_id += dip_sw;
	mob_3_id += dip_sw;
	mob_4_id += dip_sw;
	mob_5_id += dip_sw;
	
	CANGCON = ( 1 << SWRES );   // Software reset
	CANTCON = 0x00;             // CAN timing prescaler set to 0
	
	CANBT1	|= 0x0E; //CAN Baud set 125kbit
	CANBT2	|= 0x0C; //CAN Baud set 125kbit
	CANBT3	|= 0x37; //CAN Baud set 125kbit
	
	for ( int8_t mob=0; mob<6; mob++ ) {
		CANPAGE = ( mob << 4 );   // Selects Message Object 0-5
		CANCDMOB = 0x00;          // Disable mob
		CANSTMOB = 0x00;          // Clear mob status register;
	}
			
	
	//MOb 0 configuration SYNC
	CANPAGE	 = (MOb_0<<4);
	CANIDM4 = 0x00; //full mask setting
	CANIDM3 = 0x00; //full mask setting
	CANIDM2 = 0xFF; //full mask setting
	CANIDM1 = 0xFF; //full mask setting
	CANCDMOB = 0x00;
	CANCDMOB |= RECEPTION;
	CANIDT4 = 0x00;
	CANIDT3 = 0x00;
	CANIDT2 = ((mob_0_id & 0x07) << 5);
	CANIDT1 = (mob_0_id >> 3);
	CANIE2	|= (1<<IEMOB0); //Enable Interrupt MOb 0
	
	//MOb 1 configuration odbierana paczka RPDO 1 0x200+
	CANPAGE	 = (MOb_1<<4);
	CANIDM4 = 0x00; //full mask setting
	CANIDM3 = 0x00; //full mask setting
	CANIDM2 = 0xFF; //full mask setting
	CANIDM1 = 0xFF; //full mask setting
	CANCDMOB = 0x00;
	CANCDMOB |= RECEPTION | ( 8 << DLC0);
	CANIDT4 = 0x00;
	CANIDT3 = 0x00;
	CANIDT2 = ((mob_1_id & 0x07) << 5);
	CANIDT1 = (mob_1_id >> 3);
	CANIE2	|= (1<<IEMOB1); //Enable Interrupt MOb 1
	
	//MOb 2 configuration nadawana paczka TPDO 1 0x180+
	CANPAGE	 = (MOb_2<<4);
	CANIDM4 = 0x00; //full mask setting
	CANIDM3 = 0x00; //full mask setting
	CANIDM2 = (0x07 <<5); //full mask setting
	CANIDM1 = 0xFF; //full mask setting
	CANCDMOB = 0x00;
	//CANCDMOB |= TRANSMISSION;
	CANIDT4 = 0x00;
	CANIDT3 = 0x00;
	CANIDT2 = ((mob_2_id & 0x07) << 5);
	CANIDT1 = (mob_2_id >> 3);
	CANIE2	|= (1<<IEMOB2); //Enable Interrupt MOb 2
	
	
	//MOb 3 configuration nadawana paczka TPDO 2 0x280+
	CANPAGE	 = (MOb_3<<4);
	CANIDM4 = 0x00; //full mask setting
	CANIDM3 = 0x00; //full mask setting
	CANIDM2 = (0x07 <<5); //full mask setting
	CANIDM1 = 0xFF; //full mask setting
	CANCDMOB = 0x00;
	//CANCDMOB |= TRANSMISSION;
	CANIDT4 = 0x00;
	CANIDT3 = 0x00;
	CANIDT2 = ((mob_3_id & 0x07) << 5);
	CANIDT1 = (mob_3_id >> 3);
	CANIE2	|= (1<<IEMOB3); //Enable Interrupt MOb 3
	
	//MOb 4 configuration nadawana paczka SDO TX
	CANPAGE	 = (MOb_4<<4);
	CANIDM4 = 0x00; //full mask setting
	CANIDM3 = 0x00; //full mask setting
	CANIDM2 = (0x07 <<5); //full mask setting
	CANIDM1 = 0xFF; //full mask setting
	CANCDMOB = 0x00;
	//CANCDMOB |= TRANSMISSION;
	CANIDT4 = 0x00;
	CANIDT3 = 0x00;
	CANIDT2 = ((mob_4_id & 0x07) << 5);
	CANIDT1 = (mob_4_id >> 3);
	CANIE2	|= (1<<IEMOB4); //Enable Interrupt MOb 4
	
	//MOb 5 configuration odbierana paczka SDO RX
	CANPAGE	 = (MOb_5<<4);
	CANIDM4 = 0x00; //full mask setting
	CANIDM3 = 0x00; //full mask setting
	CANIDM2 = 0xFF; //full mask setting
	CANIDM1 = 0xFF; //full mask setting
	CANCDMOB = 0x00;
	CANCDMOB |= RECEPTION | ( 8 << DLC0);
	CANIDT4 = 0x00;
	CANIDT3 = 0x00;
	CANIDT2 = ((mob_5_id & 0x07) << 5);
	CANIDT1 = (mob_5_id >> 3);
	CANIE2	|= (1<<IEMOB5); //Enable Interrupt MOb 5
	
	
	CLEAR_FLAG(can_state.flags, CAN_FLAG_INTERRUPT);
	
	CANGCON |= (1<<ENASTB); //Enable controller
	
	CANGIE	|= ( 1 << ENIT ) | (1<<ENRX); //Enable Receive Interrupt
}

//Obs³uga przerwania
SIGNAL ( CAN_INT_vect ){              // use interrupts
	
	//SET_FLAG(can_state.flags, CAN_FLAG_INTERRUPT);
	int_counter++;
	
	CANPAGE = CANHPMOB & 0xF0;      // Selects MOB with highest priority interrupt 
				
		if((CANPAGE>>4) == MOb_0){ //SYNC - odbiera
			if(CANSTMOB & ( 1 << RXOK)){	//obiór SYNC-a
				SET_FLAG(can_state.flags, CAN_FLAG_SYNC_RECEIVED);				//ustaw flagê ob³sugi synca
			}
			CANSTMOB=0x00;
			CANCDMOB = 0x00;			//restart MOB-a
			CANCDMOB |= RECEPTION;		
		}
		
		else if((CANPAGE>>4) == MOb_1){ //RPDO 1 - odbiera
			if(CANSTMOB & ( 1 << RXOK)){	//
				for(uint8_t byte_nr=0; byte_nr<8; byte_nr++ ){//przepisanie danych z rejestru do tablicy
					MOb_data[MOb_1][byte_nr] = CANMSG;
				}		
			}
			CANSTMOB=0x00;
			CANCDMOB = 0x00;			//restart MOB-a
			CANCDMOB |= RECEPTION;
		}
		
		//czyszczenie flag przerwania dla paczek nadaj¹cych
		else if((CANPAGE>>4) == MOb_2){//TPDO 1
			CANSTMOB=0x00;
		}
		
		else if((CANPAGE>>4) == MOb_3){//TPDO 2
			CANSTMOB=0x00;
		}
		
		else if((CANPAGE>>4) == MOb_4){//SDO TX
			CANSTMOB=0x00;
		}
		
		else if((CANPAGE>>4) == MOb_5){ //SDO RX - odbiera
			if(CANSTMOB & ( 1 << RXOK)){	
				for(uint8_t byte_nr=0; byte_nr<8; byte_nr++ ){//przepisanie danych z rejestru do tablicy
					MOb_data[MOb_5][byte_nr] = CANMSG;
				}
			}
			SET_FLAG(can_state.flags, CAN_FLAG_SDO_RECEIVED);
			CANSTMOB=0x00;
			CANCDMOB = 0x00;			//restart MOB-a
			CANCDMOB |= RECEPTION;
		}

} 

/*ISR(BADISR_vect)
{
    SET_CAN_interrupt_flag
}*/

void CAN_task(){
	//obs³uga SDO
	if( HAS_FLAG(can_state.flags, CAN_FLAG_SDO_RECEIVED) ){
		/*---ODCZYT---*/
		if(SDO_RX_COMMAND == SDO_READ_COMMAND){//gdy komenda odczytu
			switch (SDO_RX_INDEX){//sprawdzanie po indexie
				
				case 0x2000:
					SET_FLAG(can_state.flags, CAN_FLAG_DATA_LENGTH_16);
					if(SDO_RX_SUBINDEX < ADC_MAX_CHANNEL){
						//sprawdzanie po subidenxie
						if(adc_state.active_channels[SDO_RX_SUBINDEX] == TRUE){
							SDO_TX_DATA_1 = adc_results.raw_analog_in[SDO_RX_SUBINDEX];
							SDO_TX_DATA_2 = adc_results.raw_analog_in[SDO_RX_SUBINDEX] >> 8;	
						}
						else{
							SDO_TX_DATA_1 = temp_results.raw_temp_in[SDO_RX_SUBINDEX];
							SDO_TX_DATA_2 = temp_results.raw_temp_in[SDO_RX_SUBINDEX] >> 8;
						}
					}
					else{
						SET_FLAG(can_state.flags, CAN_FLAG_COMMAND_FAILED);
					}
				break;
				
				case 0x2001:
				case 0x2002:
				case 0x2003:
				case 0x2004:
				case 0x2005:
				case 0x2006:
					SET_FLAG(can_state.flags, CAN_FLAG_DATA_LENGTH_16);
					switch(SDO_RX_SUBINDEX){
						case 0x00:
							SDO_TX_DATA_1 = non_volatile_data.adc_channel_offset[(uint8_t)SDO_RX_INDEX - 1];
							SDO_TX_DATA_2 = non_volatile_data.adc_channel_offset[(uint8_t)SDO_RX_INDEX - 1] >> 8;
						break;
						
						case 0x01:
							SDO_TX_DATA_1 = non_volatile_data.adc_channel_scale[(uint8_t)SDO_RX_INDEX - 1];
							SDO_TX_DATA_2 = non_volatile_data.adc_channel_scale[(uint8_t)SDO_RX_INDEX - 1] >> 8;
						break;
						
						case 0x02:
							SDO_TX_DATA_1 = non_volatile_data.adc_channel_max[(uint8_t)SDO_RX_INDEX - 1];
							SDO_TX_DATA_2 = non_volatile_data.adc_channel_max[(uint8_t)SDO_RX_INDEX - 1] >> 8;
						break;
						
						case 0x03:
							SDO_TX_DATA_1 = non_volatile_data.adc_channel_min[(uint8_t)SDO_RX_INDEX - 1];
							SDO_TX_DATA_2 = non_volatile_data.adc_channel_min[(uint8_t)SDO_RX_INDEX - 1] >> 8;
						break;
						
						default:
							SET_FLAG(can_state.flags, CAN_FLAG_COMMAND_FAILED);
					}
				break;
			
				
				default:
					SET_FLAG(can_state.flags, CAN_FLAG_COMMAND_FAILED);
			}
			//uzupe³nianie pozosta³ych pól pakietu
			if(!(HAS_FLAG(can_state.flags, CAN_FLAG_COMMAND_FAILED))){
				if(HAS_FLAG(can_state.flags, CAN_FLAG_DATA_LENGTH_16)){
					SDO_READ_POSITIVE_RESPONSE_16;

				}
				else if(HAS_FLAG(can_state.flags, CAN_FLAG_DATA_LENGTH_8)){
					SDO_READ_POSITIVE_RESPONSE_8;
				}
			}
			else{
				SDO_NEGATIVE_RESPONSE;
			}
		}
		/*--ZAPIS--*/
		else if(SDO_RX_COMMAND & SDO_WRITE_COMMAND){//gdy komenda zapisu
			switch (SDO_RX_INDEX){
				
				case 0x2001:
				case 0x2002:
				case 0x2003:
				case 0x2004:
				case 0x2005:
				case 0x2006:
					switch(SDO_RX_SUBINDEX){
						case 0x00:
							non_volatile_data.adc_channel_offset[(uint8_t)SDO_RX_INDEX - 1] = SDO_RX_DATA_1 | (SDO_RX_DATA_2 << 8);
						break;
						
						case 0x01:
							non_volatile_data.adc_channel_scale[(uint8_t)SDO_RX_INDEX - 1] = SDO_RX_DATA_1 | (SDO_RX_DATA_2 << 8);
						break;
						
						case 0x02:
							non_volatile_data.adc_channel_max[(uint8_t)SDO_RX_INDEX - 1] = SDO_RX_DATA_1 | (SDO_RX_DATA_2 << 8);
						break;
						
						case 0x03:
						non_volatile_data.adc_channel_min[(uint8_t)SDO_RX_INDEX - 1] = SDO_RX_DATA_1 | (SDO_RX_DATA_2 << 8);
						break;
						
						default:
							SET_FLAG(can_state.flags, CAN_FLAG_COMMAND_FAILED);
					}
				break;
								
				case 0x2007:
					SET_FLAG(can_state.flags, CAN_FLAG_DATA_LENGTH_16);
					eeprom_restore_defaults();
				break;
					
				default:
					SET_FLAG(can_state.flags, CAN_FLAG_COMMAND_FAILED);
			}
			
			if(!(HAS_FLAG(can_state.flags, CAN_FLAG_COMMAND_FAILED))){
				SDO_WRITE_POSITIVE_RESPONSE;
				eeprom_write();
			}
			else{
				SDO_NEGATIVE_RESPONSE;
			}
			
		}
		//gdy nieprawid³owa komenda
		else{
			SDO_NEGATIVE_RESPONSE;
		}
		
		//procedura wysy³ania odpowiedzi
		CAN_send_SDO();
		
		CLEAR_FLAG(can_state.flags, CAN_FLAG_SDO_RECEIVED);
		 
	}
	/*---PDO---*/
	//aktualizacja danych
	else if(HAS_FLAG(can_state.flags, CAN_FLAG_UPDATE_DATA)){//mechanizm przepisywania danych PDO - po ka¿dym sync przepisywane s¹ œwie¿e dane
		static uint8_t frame=MOb_1;
		if(frame == MOb_1 ){ //RPDO 1
			//CAN.status_word = MOb_data[frame][1]<<8 | MOb_data[frame][0];
			frame++;
		}
		else if(frame == MOb_2 ){ //TPDO 1 - nadaje
			MOb_data[frame][0] = CAN.in[0];
			MOb_data[frame][1] = CAN.in[0] >> 8;
			MOb_data[frame][2] = CAN.in[1];
			MOb_data[frame][3] = CAN.in[1] >> 8;
			MOb_data[frame][4] = CAN.in[2];
			MOb_data[frame][5] = CAN.in[2] >> 8;
			MOb_data[frame][6] = CAN.in[3];
			MOb_data[frame][7] = CAN.in[3]>> 8;
			frame++;
		}
		else if(frame == MOb_3 ){ //TPDO 2 nadaje
			MOb_data[frame][0] = CAN.in[4];
			MOb_data[frame][1] = CAN.in[4] >> 8;
			MOb_data[frame][2] = CAN.in[5];
			MOb_data[frame][3] = CAN.in[5] >> 8;
			MOb_data[frame][4] = CAN.diag_1;
			MOb_data[frame][5] = CAN.diag_2;
			MOb_data[frame][6] = 0x00;
			MOb_data[frame][7] = 0x00;
			CLEAR_FLAG(can_state.flags, CAN_FLAG_UPDATE_DATA);
			frame = MOb_1;
		}						
	}
	//wysy³anie danych po syncu
	else if( HAS_FLAG(can_state.flags, CAN_FLAG_SYNC_RECEIVED) ){ //){ //Zleæ transmisjê po koleji transmisjê kolejnych paczek
		static uint8_t mob=MOb_2; //pierwszy MOb odbieraj¹cy
		CANPAGE = ( mob << 4 );						// Selects Message Object 0-5
		if((CANEN2 & ( 1 << mob )) == 0){		//Jeœli MOb jest wolny
			for(uint8_t byte_nr=0; byte_nr<8; byte_nr++  ){
				CANMSG = MOb_data[mob][byte_nr];
			}
			//czyszczenie rejestru statusu
			CANSTMOB = 0x00;
			//komenda nadania
			CANCDMOB = TRANSMISSION | ( 8 << DLC0);//zleæ transmisjê 8 bajtów
		}
		if(mob >= MOb_3 ){
			 mob=MOb_2;
			 CLEAR_FLAG(can_state.flags, CAN_FLAG_SYNC_RECEIVED);
			 SET_FLAG(can_state.flags, CAN_FLAG_UPDATE_DATA);
		}		
		else mob++;
	}
}

void CAN_send_SDO(){
		CANPAGE = ( MOb_SDO_TX << 4 );						// Selects Message Object 0-5
		if((CANEN2 & ( 1 << MOb_SDO_TX )) == 0){		//Jeœli MOb jest wolny
			for(uint8_t byte_nr=0; byte_nr<8; byte_nr++  ){
				CANMSG = MOb_data[MOb_SDO_TX][byte_nr];
			}
		}
		//czyszczenie rejestru statusu
		CANSTMOB = 0x00;
		//komenda nadania
		CANCDMOB = TRANSMISSION | ( 8 << DLC0);//zleæ transmisjê 8 bajtów
}

