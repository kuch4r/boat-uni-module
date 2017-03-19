/*
 * PSC.h
 *
 * Created: 2013-02-10 20:58:48
 *  Author: LukaszKucharski
 */ 


#ifndef PSC_H_
#define PSC_H_

#define F_PSC 500 //kHz
#define DeadTime 3//us
#define PSC_TOP 0x07FF // do ilu ma liczyæ licznik

#define PSC_LOCK_BUFFERS PCNF |= (1<<PULOCK);
#define PSC_UNLOCK_BUFFERS PCNF &= ~(1<<PULOCK);

#define SWAP0 2
#define SWAP1 3
#define SWAP2 4

#define FORWARD 0
#define REVERSE 1

#define LOW 0
#define HIGH 1
#define UNKNOWN 2

#define ADC_SYNCHRONIZATION_OFFSET 50	//15ns

void PSC_init(void);
void Enable_PSC(uint16_t);
void Disable_PSC(void);
void PSC_Task(void);

struct PSC_str {
	uint16_t torque;
	};
	
#define Psc_select_outputs(val0A,val0B,val1A,val1B,val2A,val2B) \
POC = (val2B<<POEN2B)| \
(val2A<<POEN2A)| \
(val1B<<POEN1B)| \
(val1A<<POEN1A)| \
(val0B<<POEN0B)| \
(val0A<<POEN0A);

#define PSC_ENABLE_HARDWARE_OVERCURRENT PMIC0 |= (1<<PRFM02) | (1<<PRFM01);
#define PSC_CLEAR_INTERRUPT PIFR |= (1<<PEV0);


#define PSC0A_PIN 0
#define PSC0A_PORT D
#define PSC0B_PIN 7
#define PSC0B_PORT B
#define PSC1A_PIN 0
#define PSC1A_PORT C
#define PSC1B_PIN 6
#define PSC1B_PORT B
#define PSC2A_PIN 0
#define PSC2A_PORT B
#define PSC2B_PIN 1
#define PSC2B_PORT B
#endif /* PSC_H_ */
