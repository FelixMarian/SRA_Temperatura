#ifndef BUTTON_H
#define BUTTON_H

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
    read_pB();
  }
};

#endif