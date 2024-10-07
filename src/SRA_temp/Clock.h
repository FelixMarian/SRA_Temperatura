#ifndef CLOCK_H
#define CLOCK_H

#include <avr/io.h>
#include <avr/interrupt.h>

class Clock {
private:
  int time;
  char *clock;
public:
  Clock(int timeC) {
    time = timeC;
  }
  //Aici setam timerul 1 sa faca o intrerupere la o secunda
  void init() {
    //Resetam Timer1
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    //Folosim un prescalar de 1024 in modul CTC deoarece avem frecv. 16MHz si 16MHz/1024 = 15.625KHz, timer 1 numara pana la 65535, deci tre sa ii dam noi un maxim prin ORC1A.
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);
    OCR1A = 15625;

    //Cand se ajunge la OCR1A activam intreruperea
    TIMSK1 |= (1 << OCIE1A);
    sei();
  }
 
  void returnClock() {
    ISR(TIMER1_COMPA_vect) {
      time++;
      int secunde = time % 60;
      int minute_temp = time / 60;
      int ora = minute_temp / 60;
      int minute = minute_temp % 60;
      Serial.print("Ora: ");
      Serial.print(ora);
      Serial.print(" Min: ");
      Serial.print(minute);
      Serial.print(" Sec: ");
      Serial.print(secunde);
    }
  }

  void setClockTime(int initial_time) {
    time = initial_time;
  }
};

#endif