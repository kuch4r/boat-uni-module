///////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "ADC.h"
#include "makra.h"
#include "harddef.h"
#include "main.h"
#include "timer.h"
#include "EEPROM.h"


extern struct CAN_str CAN;
extern struct eeprom_struct non_volatile_data;

volatile uint8_t present_channel=0;

volatile struct adc_state adc_state;
volatile struct adc_data  adc_results;

// translate internal index to ADC channel num
volatile uint8_t channel_idx_to_num[ADC_MAX_CHANNEL];

SIGNAL(SIG_ADC);

void ADC_init(uint8_t act_ch, uint8_t aref_select)
{
	int8_t c;
	
	//Wybór Ÿród³a wyzwalania - 
	ADCSRB |= (1<<ADHSM) | (1<<AREFEN)| (0<<ADTS3) | (0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
	
	// ADEN - aktywowanie ADC
	// ADATE - zewnêtrzne Ÿród³o wyzwalania
	// ADPSX - ustawienie preskalera 0 1 1 - clk/8 ->2Mhz
	// ADIE - aktywowanie przerwania
	// ADSC - start pomiaru
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (0<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
			
	// REFS1	REFS0
	// 0		0	-nap odniesienia z VREF
	// 0		1	-nap odniesienia z AVcc
	// 1		1	-nap odniesienia z wew referencji
	// MUX2..0  -wybór wejscia przetwornika
	//wybór Ÿród³a odniesienia
	if(aref_select){
		//AVcc
		ADMUX |= (0<<REFS1) | (1<<REFS0);
	}
	else{
		//Internal Vref
		ADMUX |= (1<<REFS1) | (1<<REFS0);
	}
	//setting active channels
	
	adc_state.active_channel  = ADC_CHANNEL_5;
				
	if(act_ch & (1<<ADC_CHANNEL_1)){
		adc_state.active_channels[ADC_CHANNEL_1] = TRUE;
		adc_state.active_channel  = ADC_CHANNEL_1;
	} 
	if(act_ch & (1<<ADC_CHANNEL_2)){
		adc_state.active_channels[ADC_CHANNEL_2] = TRUE;
		adc_state.active_channel  = ADC_CHANNEL_2;
	}
	if(act_ch & (1<<ADC_CHANNEL_3)){
		adc_state.active_channels[ADC_CHANNEL_3] = TRUE;
		adc_state.active_channel  = ADC_CHANNEL_3;
	}
	if(act_ch & (1<<ADC_CHANNEL_4)){
		adc_state.active_channels[ADC_CHANNEL_4] = TRUE;
		adc_state.active_channel  = ADC_CHANNEL_4;
	}
	
	adc_state.active_channels[ADC_CHANNEL_5] = TRUE;
	adc_state.active_channels[ADC_CHANNEL_6] = TRUE;
	
	// init adc state struct
	adc_state.buffor_position = 0;
	
	//translate index to ADC channel num
	channel_idx_to_num[ADC_CHANNEL_1]   = AIN_1_CHANNEL;
	channel_idx_to_num[ADC_CHANNEL_2]   = AIN_2_CHANNEL;
	channel_idx_to_num[ADC_CHANNEL_3]   = AIN_3_CHANNEL;
	channel_idx_to_num[ADC_CHANNEL_4]   = AIN_4_CHANNEL;
	channel_idx_to_num[ADC_CHANNEL_5]   = AIN_5_CHANNEL;
	channel_idx_to_num[ADC_CHANNEL_6]   = AIN_6_CHANNEL;

	
	// init sum
	for( c = 0 ; c < ADC_MAX_CHANNEL ; c++ ){
		adc_state.sum[c] = 0;
	}

	// trun on first channel	
	ADC_switch(channel_idx_to_num[adc_state.active_channel]);
	

}

void ADC_switch(uint8_t channel){
		ADMUX = (ADMUX & 0xF0) | (channel & ADMUX_MASK);
}

void ADC_start_conversion(){
	ADCSRA |= (1<<ADSC);
}

// ADC task function, called every loop in main_loop
void ADC_task(void){
	static uint8_t state = ADC_MAX_CHANNEL; 
	/* if we have new results */
	if( HAS_FLAG(adc_state.flags, ADC_FLAG_CONV_COMPLETED && state == ADC_MAX_CHANNEL)) {
		//divide sum to get average value
		for( uint8_t i=0; i< ADC_MAX_CHANNEL; i++){
			//if channel is active
			if(adc_state.active_channels[i] == TRUE){
				adc_results.raw_analog_in[i] = adc_state.sum[i] / ADC_SAMPELS;
			}
			
		}
		state--;	
	}
	else if(state < ADC_MAX_CHANNEL){
		adc_results.analog_in[state] = (((uint32_t)adc_results.raw_analog_in[state] * non_volatile_data.adc_channel_scale[state]) / ADC_RAW_MAX_VAL) + non_volatile_data.adc_channel_offset[state] ;
		if(adc_results.analog_in[state] > non_volatile_data.adc_channel_max[state]){
			adc_results.analog_in[state] = non_volatile_data.adc_channel_max[state];
		}
		else if(adc_results.analog_in[state] < non_volatile_data.adc_channel_min[state]){
			adc_results.analog_in[state] = non_volatile_data.adc_channel_min[state];
		}
		if(state == 0){
			state = ADC_MAX_CHANNEL;
		}
		else{
			state--;
		}
	}
		
	/* clearing flag for conversion resulsts */
	CLEAR_FLAG(adc_state.flags, ADC_FLAG_CONV_COMPLETED);
	
};

// ADC conversion completed signal
SIGNAL(ADC_vect){
	/* save result */
	
	/* removing old result from the sum */
	if( HAS_FLAG(adc_state.flags, ADC_FLAG_HAS_FULL_BUFF) ) {
		adc_state.sum[adc_state.active_channel] -= adc_state.buffor[adc_state.active_channel][adc_state.buffor_position];
	}
	/* putting new result to the buffor */
	adc_state.buffor[adc_state.active_channel][adc_state.buffor_position] = ADC;
	/* adding new result to sum */
	adc_state.sum[adc_state.active_channel] += adc_state.buffor[adc_state.active_channel][adc_state.buffor_position];
	
	/* inc channel position counters */
	//find next active channel
	do {
		if( ++adc_state.active_channel == ADC_MAX_CHANNEL) {
			/* we checked all channels, starting from the beginning */
			adc_state.active_channel = 0;
			/* reseting buffer position */
			if( ++adc_state.buffor_position == ADC_SAMPELS ){
				/* we filled whole buffor, starting from the beginning */
				adc_state.buffor_position = 0;
				// setting flag that buffor is full (you can read results)
				SET_FLAG(adc_state.flags, ADC_FLAG_HAS_FULL_BUFF);
			}
			/* setting flag that there is new result ready */
			if( HAS_FLAG(adc_state.flags, ADC_FLAG_HAS_FULL_BUFF) ) {
				SET_FLAG(adc_state.flags, ADC_FLAG_CONV_COMPLETED);
			}
		}
	} 
	while(adc_state.active_channels[adc_state.active_channel] == FALSE);
	
	/* switching active channel */
	ADC_switch(channel_idx_to_num[adc_state.active_channel]);
	/* starting conversion */
	ADC_start_conversion();	
};
