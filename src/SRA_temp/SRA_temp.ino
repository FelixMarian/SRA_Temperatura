#include "OLED_Display.h"
#include "Clock.h"
#include "LM35.h"
#include <string.h>
#include "Button.h"
#include "LCD.h"

OLED_Display oled;
Clock clock(5000);
LM35 tempSens;
LCD myLCD(7,6,5,4,3,2);
int temperature;
byte menuState=0;
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
  uint16_t temperature = tempSens.getTemp(2);
  itoa(temperature,temp_text,10);
  clock.returnClock();
  oled.showText(temp_text, clock_formatted_time);
  delay(125);

  if(btn_RIGHT.getValue() && menuState < 7){
    menuState++;
  } else if (btn_LEFT.getValue() && menuState > 0){
    menuState--;
  }
  myLCD.write_menu(menuState);

}


//Aici initializam ceasul si-l facem sa se incrementeze la fiecare secunda si il si afisam la fiecare intrerupere, alaturi de temperatura reala


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
  
  strcpy(clock_formatted_time, fullHourFormat);
}