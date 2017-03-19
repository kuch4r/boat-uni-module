
//struktura do przechowywania danych w eepromie
struct eeprom_struct {
	//0x2001
	int16_t adc_channel_offset[ADC_MAX_CHANNEL]; 
	int16_t adc_channel_scale[ADC_MAX_CHANNEL];
	int16_t adc_channel_max[ADC_MAX_CHANNEL];
	int16_t adc_channel_min[ADC_MAX_CHANNEL];
	
	//CRC
	uint16_t crc; //musi byæ ostatnie ¿eby crc siê liczy³o
};

uint8_t eeprom_read(void);
uint8_t eeprom_write(void);

void eeprom_restore_defaults(void);

#define EEPROM_READ_RETRIES 3
#define EEPROM_WRITE_RETRIES 2