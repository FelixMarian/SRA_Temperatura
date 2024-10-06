#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <U8g2lib.h>
#include <string.h>

class OLED_Display {
private:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C display;  //Obiectul display
public:
  OLED_Display()
    : display(U8G2_R0, U8X8_PIN_NONE) {}
  void init() {
    display.begin();
  }

  //Functie prin care afisam temperatura si timpul
  void showText(const char* temp, const char* time) {
    char tempT[25], timeT[10];

//Pentru a nu se concatena la fiecare apelare
    strcpy(tempT, "Temperatura: ");
    strcpy(timeT, "Ora: ");

    strcat(tempT, temp);
    strcat(timeT, time);
    Serial.println(timeT);
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.drawStr(15, 24, tempT);
    display.drawStr(15, 48, timeT);
    display.sendBuffer();
  }
};

#endif