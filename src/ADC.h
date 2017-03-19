//Dekalaracje funkcji

#ifndef ADC_H_
#define ADC_H_
SIGNAL(SIG_ADC);

void ADC_init(uint8_t act_ch, uint8_t aref_select);
void ADC_task(void);
void ADC_switch(uint8_t channel);
void ADC_start_conversion(void);

#define ADMUX_MASK 0x0F

#define CLEAR_ADIF_FLAG ADCSRA &= ~(1<<ADIF);

#define ADC_ENABLE_INTERRUPT ADCSRA |= (1<<ADIE);
#define ADC_DISABLE_INTERRUPT ADCSRA &= ~(1<<ADIE);

//samples to calculate average from - max 64
#define ADC_SAMPELS 15

#define ADC_MAX_CHANNEL     6

#define ADC_CHANNEL_1 0
#define ADC_CHANNEL_2 1
#define ADC_CHANNEL_3 2
#define ADC_CHANNEL_4 3
#define ADC_CHANNEL_5 4
#define ADC_CHANNEL_6 5



#define ADC_FLAG_CONV_COMPLETED 0
#define ADC_FLAG_HAS_FULL_BUFF  1

struct adc_state {
	uint8_t flags;
	uint8_t active_channel;
	uint8_t buffor_position;
	volatile uint16_t buffor[ADC_MAX_CHANNEL][ADC_SAMPELS];
	volatile uint16_t sum[ADC_MAX_CHANNEL];
	volatile uint8_t active_channels[ADC_MAX_CHANNEL];
};
struct adc_data {
	uint16_t raw_analog_in[ADC_MAX_CHANNEL];
	int16_t analog_in[ADC_MAX_CHANNEL];
};

volatile struct adc_data  adc_results;
volatile struct adc_state adc_state;

#endif