#ifndef MACROS_N_FUNCTIONS_H
#define MACROS_N_FUNCTIONS_H

#include <avr/io.h>

#define set_OUTPUT_pB(pin) DDRB |= (1<<(pin-8))
#define set_INPUT_pB(pin) DDRB &= ~(1<<(pin-8))
#define write_HIGH_pB(pin) PORTB |= (1<<(pin-8))
#define write_LOW_pB(pin) PORTB &= ~(1<<(pin-8))

int read_pB(pin){
//HIGH - 1 | LOW - 0
if(PINB & (1<<(pin-8))
  return 1;
else return 0;
}

#endif