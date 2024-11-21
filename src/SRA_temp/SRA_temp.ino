#include "OLED_Display.h"
#include "Clock.h"
#include "LM35.h"
#include <string.h>
#include "Button.h"
#include "LCD.h"
#include "F_EEPROM.h"
#include "pwm.h"

OLED_Display oled;
Clock clock(5000);
LM35 tempSens;
LCD myLCD(7, 6, 5, 4, 3, 2);
int temperature;
pwm myPwm;
F_EEPROM myEEPROM;
byte menuState = 0;
double eroare = 0, suma_erori = 0, eroare_anterioara = 0, derivativa = 0, output;
double dt = 0.01;  //Timp de  esantionare de 0.01S
double T_set, T_inc, T_men, T_rac, kp, ki, kd;
bool state = false;
Button btn_OK(9), btn_CANCEL(10), btn_LEFT(8), btn_RIGHT(12);
char temp_dor[15], temp_cur[15], timpRam[15];
int SRAstate = 0;  // 0 - Stop, 1 - Inc, 2 - Men, 3 - Rac
bool newState = true;
int timpRamas;

void setup() {
  btn_OK.init();
  btn_CANCEL.init();
  btn_LEFT.init();
  btn_RIGHT.init();
  oled.init();
  Serial.begin(9600);
  clock.init();
  tempSens.init();
  myLCD.init();
  myPwm.init();

  //Initializam cu valorile din EEPROM
  kp = myEEPROM.get_t_K_p();
  kd = myEEPROM.get_t_K_d();
  ki = myEEPROM.get_t_K_l();
  T_set = myEEPROM.get_t_set();
  T_inc = myEEPROM.get_t_incalzire();
  T_rac = myEEPROM.get_t_racire();
  T_men = myEEPROM.get_t_mentinere();
}

void loop() {
  uint16_t temperature = tempSens.getTemp(1);
  itoa(T_set, temp_dor, 10);
  itoa(temperature, temp_cur, 10);
  itoa(timpRamas, timpRam, 10);
  //clock.returnClock();
  oled.showText(temp_dor, temp_cur, timpRam, SRAstate);
  delay(125);

  switch (SRAstate) {
    case 1:
      if (newState) {
        timpRamas = T_inc;
        newState = false;
      } else if (timpRamas == 1) {
        SRAstate = 2;
        newState = true;
      } else {
        calculPID_si_scrierePWM();
        Serial.println(temperature);
      }
      break;
    case 2:
      if (newState) {
        timpRamas = T_men;
        newState = false;
      } else if (timpRamas == 1) {
        SRAstate = 3;
        newState = true;
      } else {
        calculPID_si_scrierePWM();
        Serial.println(temperature);
      }
      break;
    case 3:
      if (newState) {
        timpRamas = T_rac;
        newState = false;
      } else if (timpRamas == 1) {
        SRAstate = 0;
        newState = true;
      } else {
        T_set = 25;  //Resetam T_set la 25 de grade
        calculPID_si_scrierePWM();
        Serial.println(temperature);
      }
      break;
  }

  //Daca se apasa start incepem treaba
  if (btn_OK.getValue() && menuState == 0) { SRAstate = 1; };

  if (btn_RIGHT.getValue() && menuState < 7) {
    menuState++;
  } else if (btn_LEFT.getValue() && menuState > 0) {
    menuState--;
  }
  myLCD.write_menu(menuState);
  if (btn_OK.getValue())
    set_parametrii();
  delay(250);
}


// Functie pentru calculul output-ului din PID si a scrie dutycycle-ul necesar in pwm

void calculPID_si_scrierePWM() {
  eroare = T_set - temperature;
  suma_erori += eroare * dt;
  derivativa = (eroare - eroare_anterioara) / dt;
  output = (kp * eroare) + (ki * suma_erori) + (kd * derivativa);

  //Limitam ca output-ul sa nu depaseasca intervalul 0-255
  if (output > 255) output = 255;
  else if (output < 0) output = 0;
  //Salvam eroarea curenta ca fiind ultima pentru urmatoarele calcule ale output-ului
  eroare_anterioara = eroare;
  delay(10);
  Serial.println(output);

  myPwm.setDC(output);
}

//Aici initializam ceasul si-l facem sa se incrementeze la fiecare secunda si il si afisam la fiecare intrerupere, alaturi de temperatura reala

void set_parametrii() {
  double value;
  switch (menuState) {
    case 0:  //START
      break;
    case 1:  //T_SET
      value = setting();
      myEEPROM.write_t_set(value);
      break;
    case 2:  //T_Incalzire
      value = setting();
      myEEPROM.write_t_incalzire(value);
      break;
    case 3:  //T_Mentinere
      value = setting();
      myEEPROM.write_t_mentinere(value);
      break;
    case 4:  //T_Racire
      value = setting();
      myEEPROM.write_t_racire(value);
      break;
    case 5:  //Kp
      value = setting() / 10;
      myEEPROM.write_K_p(value);
      break;
    case 6:  //Ki
      value = setting() / 10;
      myEEPROM.write_K_l(value);

      break;
    case 7:  //Kd
      value = setting() / 10;
      myEEPROM.write_K_d(value);

      break;
  }
}

byte setting() {
  byte value = 0, setting = 1;
  while (setting) {
    myLCD.show_value(value);
    if (btn_RIGHT.getValue() && value < 150)
      value += 1;
    else if (btn_LEFT.getValue() && value > 0)
      value -= 1;
    myLCD.show_value(value);
    delay(250);
    if (btn_OK.getValue()) {
      setting = 0;
      return value;
    }
  }
}

ISR(TIMER1_COMPA_vect) {
  /*  int time = clock.returnClock();
  time++;
  clock.setClockTime(time);
  int secunde = time % 60;
  int minute_temp = time / 60;
  int ora = minute_temp / 60;
  int minute = minute_temp % 60;
  char fullHourFormat[13] = "";  // Inițializat corect ca șir gol
  char secH[5], minH[5], oraH[5];
  itoa(secunde, secH, 10);
  itoa(minute, minH, 10);
  itoa(ora, oraH, 10);

  strcat(fullHourFormat, oraH);
  strcat(fullHourFormat, ":");
  strcat(fullHourFormat, minH);
  strcat(fullHourFormat, ":");
  strcat(fullHourFormat, secH);
  strcat(fullHourFormat, " ");

  strcpy(clock_formatted_time, fullHourFormat);*/
  timpRamas--;
}