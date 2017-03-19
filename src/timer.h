//Dekalaracje funkcji
void timer_init(void);
uint8_t timer_new_tick(void);

uint16_t timer_get(void);
uint16_t timer_get_us(void);

uint16_t timer_time_elapsed(uint16_t time_stamp);
uint16_t timer_time_elapsed_us(uint16_t time_stamp);
uint16_t timer_time_difference_us(uint16_t time_1, uint16_t time_2);

#define TIMER1_CLEAR_INTERRUPT_A TIFR1 |= (1<<OCF0A);
#define TIMER1_CLEAR_INTERRUPT_B TIFR1 |= (1<<OCF0B);

#define TURN_ON_COMMUTATION_INTERRUPT TIMSK1 |= (1<<OCIE1A);
#define TURN_OFF_COMMUTATION_INTERRUPT TIMSK1 &= ~(1<<OCIE1A);

#define TURN_ON_ERROR_INTERRUPT TIMSK1 |= (1<<OCIE1B);
#define TURN_OFF_ERROR_INTERRUPT TIMSK1 &= ~(1<<OCIE1B);
