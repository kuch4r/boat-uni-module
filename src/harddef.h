
#ifndef HARDDEF_H_INCLUDED
#define HARDDEF_H_INCLUDED

//warning debug disables autopower off feature!!
//#define DEBUG

//console baud	
#define BAUD 57600


//czas co który wykonywana jest pêtla g³owna [4us]
#define MAIN_LOOP_PERIOD 25 //4us*25=0,1ms


//Wejœcia
#define AIN_1_PORT B
#define AIN_1 7
#define AIN_1_CHANNEL 4

#define AIN_2_PORT B
#define AIN_2 5
#define AIN_2_CHANNEL 6

#define AIN_3_PORT C
#define AIN_3 6
#define AIN_3_CHANNEL 10

#define AIN_4_PORT C
#define AIN_4 5
#define AIN_4_CHANNEL 9

#define AIN_5_PORT C
#define AIN_5 4
#define AIN_5_CHANNEL 8

#define AIN_6_PORT B
#define AIN_6 2
#define AIN_6_CHANNEL 5

#define ONE_WIRE_AIN_1 &PORT(AIN_1_PORT), &DDR(AIN_1_PORT), &PIN(AIN_1_PORT), (1<<AIN_1)
#define ONE_WIRE_AIN_2 &PORT(AIN_2_PORT), &DDR(AIN_2_PORT), &PIN(AIN_2_PORT), (1<<AIN_2)
#define ONE_WIRE_AIN_3 &PORT(AIN_3_PORT), &DDR(AIN_3_PORT), &PIN(AIN_3_PORT), (1<<AIN_3)
#define ONE_WIRE_AIN_4 &PORT(AIN_4_PORT), &DDR(AIN_4_PORT), &PIN(AIN_4_PORT), (1<<AIN_4)
#define ONE_WIRE_AIN_5 &PORT(AIN_5_PORT), &DDR(AIN_5_PORT), &PIN(AIN_5_PORT), (1<<AIN_5)
#define ONE_WIRE_AIN_6 &PORT(AIN_6_PORT), &DDR(AIN_6_PORT), &PIN(AIN_6_PORT), (1<<AIN_6)


//LEDy
#define LED_0_PORT D
#define LED_0 2

#define LED_1_PORT B
#define LED_1 4


//DIP SW
#define SW_1_PORT D
#define SW_1 7

#define SW_2_PORT D
#define SW_2 6

#define SW_3_PORT D
#define SW_3 5

#define SW_4_PORT B
#define SW_4 1

#define SW_5_PORT B
#define SW_5 0

#define SW_6_PORT C
#define SW_6 1

#define SW_7_PORT D
#define SW_7 1

#define SW_8_PORT C
#define SW_8 0

//Porty kierunku wyci¹garki


//Podtrzymanie zasilania
#define PWR_PORT B
#define PWR 3

//Buzzer
#define BUZZER_PORT C
#define BUZZER 7


//wsp pr¹du
#define ADC_CHANNEL_DEFAULT_SCALE ADC_RAW_MAX_VAL
#define ADC_CHANNEL_DEFAULT_OFFSET 0
#define ADC_CHANNEL_DEFAULT_MAX ADC_RAW_MAX_VAL
#define ADC_CHANNEL_DEFAULT_MIN 0




#define ADC_RAW_MAX_VAL 0x3FF

//czas do auto wy³¹czenia
#define TIME_TO_LIVE 50000

//okres migania diod [ms]
#define LED_BLINKING_PERIOD 300

//iloœæ kana³ów ADC
#define ADC_CHANNELS_COUNT 11

//czas konwersji temperatury
#define TEMP_CONVERSION_TIME 800

//DIP SW can address mask
#define DIP_SW_CAN_ADDR_MASK 0x07
				
#define DIP_SW_AIN_CHANNELS_MASK 0xF0

#define DIP_SW_AREF_SOURCE_MASK 0x08


#endif //HARDDEF_H_INCLUDED

