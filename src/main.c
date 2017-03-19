/*
 * BLDC.c
 *
 * Created: 2013-02-03 11:12:14
 *  Author: LukaszKucharski
 
 CKSEL 0001
 */ 


#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <util/crc16.h>
#include "CAN.h"
#include "main.h"
#include "harddef.h"
#include "makra.h"
#include "timer.h"
#include "ADC.h"
#include "uart.h"
#include "EEPROM.h"
#include "onewire.h"
#include "ds18b20.h"


extern struct CAN_str CAN;
extern struct eeprom_struct non_volatile_data;


volatile uint16_t life_timer = 0;
volatile uint16_t time_stamp = 0;

uint8_t DIP_sw_word = 0;
uint8_t channel_config = 0;

FILE uart_output = FDEV_SETUP_STREAM(uart_putc_s, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getc_s, _FDEV_SETUP_READ);


int main(void) {
	
	HAL_init();
	DIP_sw_word = Get_DIP_sw_word();
	channel_config = GET_DIP_SW_AIN_CHANNELS(DIP_sw_word);
	timer_init();
	ADC_init(~channel_config , GET_DIP_SW_AREF_SOURCE(DIP_sw_word));
	uart_init(BAUD);
	CAN_init(GET_DIP_SW_CAN_ADDR(DIP_sw_word));
	ds18b20_init(channel_config);
	

	stdout = &uart_output;
	stdin  = &uart_input;
	
	sei();

	uart_puts_P("Unimodule\nCompilation date ");
	uart_puts_P(__DATE__);
	uart_puts_P("\n");
	uart_puts_P("channel config:\n");
	uart_puts_P("ch 1:");
	if((channel_config & (1<<ADC_CHANNEL_1)) == ADC_IN){
		uart_puts_P("analog\n");
	}
	else{
		uart_puts_P("ds18b20\n");
	}
	uart_puts_P("ch 2:");
	if((channel_config & (1<<ADC_CHANNEL_2)) == ADC_IN){
		uart_puts_P("analog\n");
	}
	else{
		uart_puts_P("ds18b20\n");
	}
	uart_puts_P("ch 3:");
	if((channel_config & (1<<ADC_CHANNEL_3)) == ADC_IN){
		uart_puts_P("analog\n");
	}
	else{
		uart_puts_P("ds18b20\n");
	}
	uart_puts_P("ch 4:");
	if((channel_config & (1<<ADC_CHANNEL_4)) == ADC_IN){
		uart_puts_P("analog\n");
	}
	else{
		uart_puts_P("ds18b20\n");
	}
	uart_puts_P("ch 5:analog\n");
	uart_puts_P("ch 6:analog\n");
	
	//próba odczytu konfiguracji z EEPROMU, jeœli odczyt siê nie powiód³ przypisujemy wartoœci domyœlne
	if(eeprom_read() == EEPROM_ERROR){
		eeprom_restore_defaults();
		uart_puts_P("Loading default values");
	}
	
	
	//PWR_ON;
	LED_0_ON;
	LED_1_OFF;
	
	ADC_start_conversion();
	
	time_stamp=timer_get();
	life_timer=timer_get();
	
    while(1) {
		if( timer_new_tick() ){
			main_loop();
		}
	}
};

void main_loop() {
	
	ADC_task();
	ds18b20_task();
	//przepisanie wartoœci
	for( uint8_t i=0 ; i < TEMP_MAX_CHANNEL ; i++){
		if((channel_config & (1<<i)) == ADC_IN){
			CAN.in[i] = adc_results.analog_in[i];
		}
		else{
			CAN.in[i] = temp_results.temp_in[i];
		}
	}
	CAN.in[ADC_CHANNEL_5] = adc_results.analog_in[ADC_CHANNEL_5];
	CAN.in[ADC_CHANNEL_6] = adc_results.analog_in[ADC_CHANNEL_6];
			
	CAN_task();
}


void LED_blinking(uint8_t led_0, uint8_t led_1){
	static uint16_t led_time_stamp=0;
	static uint8_t led_status=0;
	
		if(timer_time_elapsed(led_time_stamp)>LED_BLINKING_PERIOD){
			
			led_time_stamp = timer_get();
			
			if(led_status == 0){
				if(led_0)
				LED_0_ON;
				if(led_1)
				LED_1_ON;
				led_status = 1;
			}
			else{
				if(led_0)
				LED_0_OFF;
				if(led_1)
				LED_1_OFF;
				led_status = 0;
			}
		}
};

void LEDs_OFF(void){
	LED_0_OFF;
	LED_1_OFF;
	
};

void HAL_init(void){
	
	//w³¹czenie PLL na 64MHz
	PLLCSR |= (1<PLLF) | (1<<PLLE);
	
	//ustawienie porty na wyjœciowe dla LED-ow
	DDR(LED_0_PORT) |= (1<<LED_0);
	DDR(LED_1_PORT) |= (1<<LED_1);

	LED_0_ON;
	LED_1_ON;
	
	//ustawienei podtrzymanie zasilania na wyjœcie
	DDR(PWR_PORT) |= (1<<PWR);
	
	//Inicjalizacja DIP Switchy
	DDR(SW_1_PORT) &= ~(1<<SW_1);
	DDR(SW_2_PORT) &= ~(1<<SW_2);
	DDR(SW_3_PORT) &= ~(1<<SW_3);
	DDR(SW_4_PORT) &= ~(1<<SW_4);
	DDR(SW_5_PORT) &= ~(1<<SW_5);
	DDR(SW_6_PORT) &= ~(1<<SW_6);
	DDR(SW_7_PORT) &= ~(1<<SW_7);
	DDR(SW_8_PORT) &= ~(1<<SW_8);
	
	//w³aczenie pull-up-ów
	PORT(SW_1_PORT) |= 1<<SW_1;
	PORT(SW_2_PORT) |= 1<<SW_2;
	PORT(SW_3_PORT) |= 1<<SW_3;
	PORT(SW_4_PORT) |= 1<<SW_4;
	PORT(SW_5_PORT) |= 1<<SW_5;
	PORT(SW_6_PORT) |= 1<<SW_6;
	PORT(SW_7_PORT) |= 1<<SW_7;
	PORT(SW_8_PORT) |= 1<<SW_8;
	
		
	//Inicjalizacja wyjœæ
	DDR(BUZZER_PORT) |= (1<<BUZZER);
	
	//aktywacja przerwañ
	//PCICR |= (1<<PCIE0) | (1<<PCIE1);
	
	//PCMSK0 |= (1<<PCINT2);
	//PCMSK1 |= (1<<PCINT12);
	
};

uint8_t Get_DIP_sw_word(void){
	uint8_t result=0;
	if(SW_1_SET)
		result |= 1<<0;
	if(SW_2_SET)
		result |= 1<<1;
	if(SW_3_SET)
		result |= 1<<2;
	if(SW_4_SET)
		result |= 1<<3;
	if(SW_5_SET)
		result |= 1<<4;
	if(SW_6_SET)
		result |= 1<<5;
	if(SW_7_SET)
		result |= 1<<6;
	if(SW_8_SET)
		result |= 1<<7;
	return result; 
	
};

SIGNAL (PCINT0_vect){ //przerwanie od WINCH_UP
	
};

SIGNAL (PCINT1_vect){ //przerwanie od WINCH_DOWN
	
};
