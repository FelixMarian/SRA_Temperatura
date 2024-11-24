#ifndef F_EEPROM_H
#define F_EEPROM_H

#include <EEPROM.h>
#include "macros_n_functions.h"


class F_EEPROM{
private:
  double t_set,t_incalzire, t_mentinere, t_racire, K_p, K_l, K_d;
public:
F_EEPROM() {
    EEPROM.get(T_set_adr, t_set);
    EEPROM.get(T_incalzire_adr, t_incalzire);
    EEPROM.get(T_mentinere_adr, t_mentinere);
    EEPROM.get(T_racire_adr, t_racire);
    EEPROM.get(K_P_adr, K_p);
    EEPROM.get(K_L_adr, K_l);
    EEPROM.get(K_D_adr, K_d);
}


void write_t_set(double value) { EEPROM.put(T_set_adr, value); }
void write_t_incalzire(double value) { EEPROM.put(T_incalzire_adr, value); }
void write_t_mentinere(double value) { EEPROM.put(T_mentinere_adr, value); }
void write_t_racire(double value) { EEPROM.put(T_racire_adr, value); }
void write_K_p(double value) { EEPROM.put(K_P_adr, value); }
void write_K_l(double value) { EEPROM.put(K_L_adr, value); }
void write_K_d(double value) { EEPROM.put(K_D_adr, value); }

  double get_t_set(){return t_set;}
  double get_t_incalzire(){return t_incalzire;}
  double get_t_mentinere(){return t_mentinere;}
  double get_t_racire(){return t_racire;}
  double get_t_K_p(){return K_p;}
  double get_t_K_l(){return K_l;}
  double get_t_K_d(){return K_d;}


};

#endif