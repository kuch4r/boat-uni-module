///////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
///////////////////////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "timer.h"
#include "makra.h"
#include "harddef.h"

SIGNAL(TIMER0_OVF_vect);

static volatile uint16_t timer=0; //czas w [7,8125ms]
static volatile uint16_t timer_prv = 0; // wartoœæ timera w poprzednim cyklu;
static volatile uint8_t new_tick=0; // jeœli 1 to znaczy ¿e w tej interacji nowy tick - up³yne³o 7,8125 ms


void timer_init(void){ //real time clock

	//Timer0
	//CTC Mode
	TCCR0A |= (1<<WGM01);
	OCR0A = 249;
	//w³aczenie zg³aszania przerwania przy przepe³nieniu
    TIMSK0 |= (1<<OCIE0A);
	//Prescaler 64
	TCCR0B |= (0<<CS12)|(1<<CS11)|(1<<CS10);
	
	//Timer1
	//w³¹czenie przerwania
	//TIMSK1 |= (1<<TOIE1);
	//Prescaler 64
	TCCR1B |= (0<<CS12)|(1<<CS11)|(1<<CS10);
	//TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10);
};
  
//funckje zwraca TRUE jeœli od jej ostatniego wywo³ania up³yne³o 
uint8_t timer_new_tick(void){
	static uint16_t time_stamp=0;

	if(timer_time_elapsed_us(time_stamp) < MAIN_LOOP_PERIOD){
		return 0;
	}
	
	time_stamp = timer_get_us();
	return 1;	
};

//funkcja zwraca akutaln¹ wartoœc timera RTC
inline uint16_t timer_get(){
	return timer;
}

//zwraca czas licznika liczy w 4us jednostkach
uint16_t timer_get_us(){
	//return (TCNT1H<<8 | TCNT1L);
	return TCNT1;
}

//zwraca czas w ms jaki czas up³yn¹³ od wartoœci podanej jako argument (argument czas timera)
//Mierzy czas do 1 minuty!! potem siê przewija licznik!!
uint16_t timer_time_elapsed(uint16_t time_stamp){
	uint16_t current_timer=timer_get();
	
	if( current_timer >= time_stamp){
		return (current_timer-time_stamp);
	}
	else{
		return (current_timer+(0xFFFF-time_stamp));
	}
};

//zwraca czas w 4us jaki czas up³yn¹³ od wartoœci podanej jako argument (argument czas timera)
//Mierzy czas do 262ms!! potem siê przewija licznik!!
uint16_t timer_time_elapsed_us(uint16_t time_stamp){
	uint16_t current_timer=timer_get_us();
	
	if( current_timer >= time_stamp){
		return (current_timer-time_stamp);
	}
	else{
		return (current_timer+(0xFFFF-time_stamp));
	}
};

uint16_t timer_time_difference_us(uint16_t time_1, uint16_t time_2){
	
	if( time_2 >= time_1){
		return (time_2 - time_1);
	}
	else{
		return (time_2+(0xFFFF-time_1));
	}
};

//Przepe³nienie linika 0
SIGNAL(TIMER0_COMPA_vect) {
	//zwiêksz TIMER
	timer++;
}

//Przepe³nienie linika 1
SIGNAL(TIMER1_OVF_vect){

}
//Comapare A Timer1
SIGNAL(TIMER1_COMPA_vect){
		
}
//Comapare B Timer1
SIGNAL(TIMER1_COMPB_vect ){
	
}


