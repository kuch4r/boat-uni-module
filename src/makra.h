
#ifndef MAKRA_H_INCLUDED
#define MAKRA_H_INCLUDED

#include "harddef.h"
// Makra upraszczaj¹ce dostêp do portów
// *** Port
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
// *** Pin
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
// *** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)

// NOPek
#define NOP() {asm volatile("nop"::);}

#define LED_0_ON PORT(LED_0_PORT) |= _BV(LED_0)
#define LED_0_OFF PORT(LED_0_PORT) &= ~(_BV(LED_0))

#define LED_1_ON PORT(LED_1_PORT) |= _BV(LED_1)
#define LED_1_OFF PORT(LED_1_PORT) &= ~(_BV(LED_1))

#define BUZZER_ON PORT(BUZZER_PORT) |= _BV(BUZZER)
#define BUZZER_OFF PORT(BUZZER_PORT) &= ~(_BV(BUZZER))

#define PWR_ON PORT(PWR_PORT) |= (1<<PWR)
#define PWR_OFF PORT(PWR_PORT) &= ~(1<<PWR)



#define SW_1_SET bit_is_clear(PIN(SW_1_PORT),SW_1)
#define SW_2_SET bit_is_clear(PIN(SW_2_PORT),SW_2)
#define SW_3_SET bit_is_clear(PIN(SW_3_PORT),SW_3)
#define SW_4_SET bit_is_clear(PIN(SW_4_PORT),SW_4)
#define SW_5_SET bit_is_clear(PIN(SW_5_PORT),SW_5)
#define SW_6_SET bit_is_clear(PIN(SW_6_PORT),SW_6)
#define SW_7_SET bit_is_clear(PIN(SW_7_PORT),SW_7)
#define SW_8_SET bit_is_clear(PIN(SW_8_PORT),SW_8)

#define GET_DIP_SW_CAN_ADDR(x) (x & DIP_SW_CAN_ADDR_MASK)
#define GET_DIP_SW_AIN_CHANNELS(x) (x & DIP_SW_AIN_CHANNELS_MASK)>>4
#define GET_DIP_SW_AREF_SOURCE(x) (x & DIP_SW_AREF_SOURCE_MASK)>>3

#define SET_FLAG(target, flag)    target = target | (1 << flag)
#define CLEAR_FLAG( target, flag) target = target & (~(1 << flag))
#define HAS_FLAG( target, flag)   (target & (0x01 << flag))


#define EEPROM_ERROR 1
#define EEPROM_OK 0

#define TRUE 1
#define FALSE 0

#endif //MAKRA_H_INCLUDED
