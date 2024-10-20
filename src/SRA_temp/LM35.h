#ifndef LM35_H
#define LM35_H

#include <avr/io.h>
#include "macros_n_functions.h"

class LM35 {
private:
public:
  void init() {
    set_INPUT_A2();

    //Setam tensiunea de referinta ca fiind cea interna 5V
    ADMUX = (ADMUX & ~(1 << REFS1)) | (1 << REFS0);

    //Setam prescalar 128 => Frecv 125KHz si dam enable la CAN
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
  }
  uint16_t getTemp(uint8_t ch) {
    ADMUX = (ADMUX & 0xF0 | ch & 0x0F);

    //Activam conversia efectiva
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC))
      ;  //Se asteapta

    float temperature = (ADC * 5.0 / 1023.0) * 100;



    return (uint16_t)ADC;
  
  }
};


#endif