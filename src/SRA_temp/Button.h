#ifndef BUTTON_H
#define BUTTON_H

#include "macros_n_functions.h"

class Button{
private:
  int pin;
public:
  Button(int pinC){
    pin = pinC;
  }
  void init(){
    set_INPUT_pB(pin);
  }
  int getValue(){
    return read_pB(pin);
  }
};

#endif