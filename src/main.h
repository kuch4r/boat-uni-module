/*
 * BLDC.h
 *
 * Created: 2013-02-03 23:50:15
 *  Author: LukaszKucharski
 */ 


#ifndef BLDC_H_
#define BLDC_H_



void HAL_init(void);
void LED_blinking(uint8_t led_0, uint8_t led_1);
void LEDs_OFF(void);
void main_loop(void);

uint8_t Get_DIP_sw_word(void);

#define DS18B20 1
#define ADC_IN 0






#endif /* BLDC_H_ */