#ifndef LM35_H
#define LM35_H

class LM35{
private:
public:
  void init(){
    set_INPUT_A0();

    //Setam tensiunea de referinta ca fiind cea interna
    ADMUX |= (1<<REFS0);

    //Setam prescalar 128 => Frecv 125KHz si dam enable la CAN
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN);
  }
  uint16_t getTemp(uint8_t ch){
    ADMUX = (ADMUX & 0xF0 | ch & 0x0F);
    
    //Activam conversia efectiva
    ADSCRA |= (1<<ADSC);

    while(ADSCRA & (1<<ADSC)); //Se asteapta 

    return ADC;
  }

};


#endif