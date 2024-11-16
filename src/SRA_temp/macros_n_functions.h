#ifndef MACROS_N_FUNCTIONS_H
#define MACROS_N_FUNCTIONS_H

#include <avr/io.h>

#define set_OUTPUT_pB(pin) DDRB |= (1<<(pin-8))
#define set_INPUT_pB(pin) DDRB &= ~(1<<(pin-8))
#define write_HIGH_pB(pin) PORTB |= (1<<(pin-8))
#define write_LOW_pB(pin) PORTB &= ~(1<<(pin-8))
#define set_INPUT_A0() DDRC &= ~(1 << PC0)


#define T_incalzire_adr 0x00
#define T_mentinere_adr 0x01
#define T_racire_adr 0x02
#define K_P_adr 0x03
#define K_L_adr 0x04
#define K_D_adr 0x05
#define T_set_adr 0x06


int read_pB(int pin){
//HIGH - 1 | LOW - 0
if(PINB & (1<<(pin-8)))
  return 1;
return 0;
}



#endif