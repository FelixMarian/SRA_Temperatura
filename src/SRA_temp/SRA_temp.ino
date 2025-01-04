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
int temperature;
pwm myPwm;
F_EEPROM myEEPROM;
byte menuState = 0;
double eroare = 0, suma_erori = 0, eroare_anterioara = 0, derivativa = 0, output;
double dt = 0.1;  //Timp de  esantionare de 0.1S
double T_set, T_inc, T_men, T_rac, kp, ki, kd;
bool state = false;
Button btn_OK(8), btn_CANCEL(9), btn_LEFT(10), btn_RIGHT(12);
char temp_dor[15], temp_cur[15], timpRam[15];
int SRAstate = 0;  // 0 - Stop, 1 - Inc, 2 - Men, 3 - Rac
bool newState = true;
int timpRamas;
double Temp_interm, pas_crestere, pas_scadere, temp_initial;

unsigned long startTime, startTimeOkBtn = 0, startTimeBtn;

void setup() {
  btn_OK.init();
  btn_CANCEL.init();
  btn_LEFT.init();
  btn_RIGHT.init();
  oled.init();
  Serial.begin(9600);
  clock.init();
  tempSens.init();
  myPwm.init();
  startTime = millis();
  startTimeBtn = millis();

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
  temperature = tempSens.getTemp(1);
  itoa(T_set, temp_dor, 10);
  itoa(temperature, temp_cur, 10);
  itoa(timpRamas, timpRam, 10);
  //clock.returnClock();
  oled.showText(temp_dor, temp_cur, timpRam, SRAstate);



  switch (SRAstate) {
    case 1:
      if (newState) {
        timpRamas = T_inc;
        temp_initial = temperature;
        newState = false;
      } else if (timpRamas == 0) {
        newState = true;
        SRAstate = 2;
      } else {
        //Incrementam la o secunda temp_intermediara
        if (millis() - startTime > 880) {
          startTime = millis();
          Temp_interm += pas_crestere; 
          timpRamas--;

          // Corectare pentru a nu depăși T_set
          if (Temp_interm >= T_set - pas_crestere) {
            Temp_interm = T_set;
          }

          // Debugging
          /*Serial.print("Temp_interm: ");
          Serial.println(Temp_interm);
          Serial.print("T_set: ");
          Serial.println(T_set);
          // Afișăm valorile pentru fiecare element
          Serial.print(" Er: ");
          Serial.print(eroare);

          Serial.print(" Tmp_a: ");
          Serial.print(temperature);

          Serial.print(" Target: ");
          Serial.print(Temp_interm);

          Serial.print(" Suma e: ");
          Serial.print(suma_erori);

          Serial.print(" Deriv: ");
          Serial.print(derivativa);

          Serial.print(" Output: ");
          Serial.print(output);

          Serial.print(" Kp: ");
          Serial.print(kp, 3);

          Serial.print(" Ki: ");
          Serial.print(ki, 3);

          Serial.print(" Kd: ");
          Serial.println(kd, 3);*/
        }

        calculPID_si_scrierePWM();
      }
      break;
    case 2:
      if (newState) {
        timpRamas = T_men;
        newState = false;
      } else if (timpRamas == 0) {
        SRAstate = 3;
        newState = true;
        
      } else {
        if (millis() - startTime > 880) {
          startTime = millis();
          timpRamas--;
        }
        calculPID_si_scrierePWM();
      }
      break;
    case 3:
      if (newState) {
        timpRamas = T_rac;
        T_set = temp_initial;
        pas_scadere = (temperature-temp_initial)/T_rac;
        newState = false;
      } else if (timpRamas == 0) {
        SRAstate = 0;
        newState = true;
      } else {
        if (millis() - startTime > 880) {
          startTime = millis();
          Temp_interm -= pas_scadere; 
          timpRamas--;

         /* Serial.print("Temp_interm: ");
          Serial.println(Temp_interm);
          Serial.print("T_set: ");
          Serial.println(T_set);
          // Afișăm valorile pentru fiecare element
          Serial.print(" Er: ");
          Serial.print(eroare);

          Serial.print(" Tmp_a: ");
          Serial.print(temperature);

          Serial.print(" Target: ");
          Serial.print(Temp_interm);

          Serial.print(" Suma e: ");
          Serial.print(suma_erori);

          Serial.print(" Deriv: ");
          Serial.print(derivativa);

          Serial.print(" Output: ");
          Serial.print(output);

          Serial.print(" Kp: ");
          Serial.print(kp, 3);

          Serial.print(" Ki: ");
          Serial.print(ki, 3);

          Serial.print(" Kd: ");
          Serial.println(kd, 3);*/
        }
        calculPID_si_scrierePWM();

      }
      break;
  }


  if (millis() - startTimeBtn > 500) {
    startTimeBtn = millis();
    //Daca se apasa start incepem treaba
    if (btn_OK.getValue() && menuState == 0) {
      SRAstate = 1;
      pas_crestere = (double)(T_set - temperature) / T_inc;
      Temp_interm = temperature;
    };

    if (btn_RIGHT.getValue() && menuState < 7) {
      menuState++;
    } else if (btn_LEFT.getValue() && menuState > 0) {
      menuState--;
    }
    oled.write_menu(menuState);
    if (btn_OK.getValue())
      set_parametrii();
  }
}


// Functie pentru calculul output-ului din PID si a scrie dutycycle-ul necesar in pwm

void calculPID_si_scrierePWM() {
  eroare = Temp_interm - temperature;                              // Calculează eroarea
  suma_erori += eroare * dt;                                       // Integrarea erorii
  derivativa = (eroare - eroare_anterioara) / dt;                  // Derivata erorii
  output = (kp * eroare) + (ki * suma_erori) + (kd * derivativa);  // Calculul final al PID

  // Limităm output-ul să nu depășească intervalul 0-255
  if (output > 255) output = 255;
  else if (output < 0) output = 0;


  // Salvăm eroarea curentă ca fiind ultima pentru următoarele calcule ale output-ului
  eroare_anterioara = eroare;

  delay(1);  // Pauză pentru a permite citirea serialului

  // Setăm PWM-ul
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
      value = setting() / 1000;
      myEEPROM.write_K_d(value);

      break;
  }
}

double setting() {
  double value = 0, setting = 1;
  while (setting) {
    oled.show_value(value);
    if (btn_RIGHT.getValue() && value < 150)
      value += 1;
    else if (btn_LEFT.getValue() && value > 0)
      value -= 1;
    oled.show_value(value);
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
  //timpRamas--;
}