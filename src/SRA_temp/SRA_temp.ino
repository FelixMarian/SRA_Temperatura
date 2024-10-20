#include "OLED_Display.h"
#include "Clock.h"
#include "LM35.h"
#include <string.h>

OLED_Display oled;
Clock clock(5000);
LM35 tempSens;
int temperature;
char temp_text[20];
char clock_formatted_time[50];

void setup() {
  oled.init();
  Serial.begin(9600);
  clock.init();
  tempSens.init();
}

void loop() {
  uint16_t temperature = tempSens.getTemp(2);
  Serial.print("TEMP: ");
  Serial.println(temperature);
  itoa(temperature,temp_text,10);
  clock.returnClock();
  oled.showText(temp_text, clock_formatted_time);
  delay(1000);
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
  char fullHourFormat[50] = " ";
  char secH[5], minH[5], oraH[5];
  itoa(secunde, secH, 10);
  itoa(minute, minH, 10);
  itoa(ora, oraH, 10);
  Serial.println(secH);
  Serial.println(minH);
  Serial.println(oraH);
  strcat(fullHourFormat, oraH);
  strcat(fullHourFormat, ":");
  strcat(fullHourFormat, minH);
  strcat(fullHourFormat, ":");
  strcat(fullHourFormat, secH);
  strcpy(clock_formatted_time, fullHourFormat);
  strcpy(fullHourFormat, "");
}