#include "OLED_Display.h"
#include "Clock.h"
#include "LM35.h"
#include <string.h>
#include "Button.h"
#include "LCD.h"
#include "F_EEPROM.h"

OLED_Display oled;
Clock clock(5000);
LM35 tempSens;
LCD myLCD(7, 6, 5, 4, 3, 2);
int temperature;
F_EEPROM myEEPROM;
byte menuState = 0;

Button btn_OK(9), btn_CANCEL(10), btn_LEFT(8), btn_RIGHT(12);
char temp_text[15];
char clock_formatted_time[13];

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
}

void loop() {
  uint16_t temperature = tempSens.getTemp(0);
  itoa(temperature, temp_text, 10);
  clock.returnClock();
  oled.showText(temp_text, clock_formatted_time);
  delay(125);

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


//Aici initializam ceasul si-l facem sa se incrementeze la fiecare secunda si il si afisam la fiecare intrerupere, alaturi de temperatura reala

void set_parametrii() {
  byte value;
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
      value = setting();
      myEEPROM.write_K_p(value);
      break;
    case 6:  //Kl
      value = setting();
      myEEPROM.write_K_l(value);

      break;
    case 7:  //Kd
      value = setting();
      myEEPROM.write_K_d(value);

      break;
  }
}

byte setting() {
  byte value = 0, setting = 1;
  while (setting) {
    myLCD.show_value(value);
    if (btn_RIGHT.getValue() && value < 60)
      value += 5;
    else if (btn_RIGHT.getValue() && value > 0)
      value -= 5;
    myLCD.show_value(value);
    delay(250);
    if (btn_OK.getValue()) {
      setting = 0;
      return value;
    }
  }
}

ISR(TIMER1_COMPA_vect) {
  int time = clock.returnClock();
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

  strcpy(clock_formatted_time, fullHourFormat);
}