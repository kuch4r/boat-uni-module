/*
 * PSC.c
 *
 * Created: 2013-02-10 20:58:28
 *  Author: LukaszKucharski
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PSC.h"
#include "makra.h"
#include "harddef.h"
#include "CAN.h"
#include "ADC.h"

struct PSC_str PSC;
extern struct CAN_str CAN;


void PSC_init(){
	
	//Inicjalizacja port�w na wyj�ciowe i ustawienie wyj�� na 0
	DDR(PSC0A_PORT) |= (1<<PSC0A_PIN);
	PORT(PSC0A_PORT) &= ~(_BV(PSC0A_PIN));
	PORT(PSC0A_PORT) |= (_BV(PSC0A_PIN));
	
	
	//rejestr konfiguracjyny
	//PULOCK - jednoczena zmiana rejestr�w gdy zwoniony na 0
	//PMODE - 0 One Ramp Mode
	//POPx - 1 aktywny stan wysoki
	PCNF |= (0<<PULOCK) | (0<<PMODE) | (1<<POPB) | (1<<POPA);
	
	//PPRE - prescaler na wej�ciu modu�o- brak preskalera
	//PCLKSEL - �r�d�o zegara PLL
	//PRUN - start modu�u
	PCTL = (1<<PPRE1) | (0<<PPRE0) | (0<<PCLKSEL) | (0<<SWAP0) | (0<<SWAP1) | (0<<SWAP2);
	
	//POVEN - zabezpieczenie przed overlapem
	//PELEV - stan wysoki powoduje b��d
	//PFLTE0 filtr cyforwy na wejsciu fault
	//PRFM - zatrzymaj modu� w przypadku b��du
	//PMIC0 |= (0<<POVEN0) | (1<<PELEV0) | (1<<PFLTE0) | (1<<PRFM02) | (1<<PRFM01);
	PMIC0 |= (1<<POVEN0) | (0<<PELEV0) | (0<<PFLTE0) | (0<<PRFM02) | (0<<PRFM01);
	
	//Przewanie od Overcurrent
	//PIM |= (1<<PEVE0);
	
	//Ustawia TOP licznika
	POCR_RB = PSC_TOP;
	
	//W��cz kana� 0A
	POC = (1<<POEN0A);
	
	//ustawia warto�� pocz�tkow� momentu
	PSC.torque = TORQUE_INIT_VAL;
	
	Disable_PSC();
	
}

inline void Enable_PSC(uint16_t init_torque){
	POCR0RA = ((uint32_t)PSC_TOP*init_torque)/1000;
	PCTL |= (1<<PRUN);
}

inline void Disable_PSC(){
	PCTL &= ~(1<<PRUN);
}

void PSC_Task(){
	static uint16_t torque_prev = 0;
	static uint16_t duty_cycle=0;
	
	if(PSC.torque != torque_prev){
		
		duty_cycle = PSC.torque;
			
		if(duty_cycle>1000){
			duty_cycle=1000;
		}
		
		POCR0RA = ((uint32_t)PSC_TOP*duty_cycle)/1000;
		
		torque_prev = PSC.torque;
	 }
	
		 
}

//przerwanie od overcurrent
SIGNAL (PSC_FAULT_vect){
	
	PSC_CLEAR_INTERRUPT
}