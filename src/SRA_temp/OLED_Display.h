#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <U8x8lib.h>
#include <string.h>

class OLED_Display {
private:
  U8X8_SH1106_128X64_NONAME_HW_I2C display;  // Obiectul display pentru U8X8

public:
  OLED_Display()
    : display(U8X8_PIN_NONE) {}

  void init() {
    display.begin();
  }

  // Funcție pentru afișarea temperaturii și timpului
  void showText(const char* temp, const char* time) {
    char tempT[25], timeT[10];

    // Pentru a nu se concatena la fiecare apelare, "resetam" textul
    strcpy(tempT, "Temperatura: ");
    strcpy(timeT, "Ora: ");

    strcat(tempT, temp);
    strcat(timeT, time);
    
    display.setFont(u8x8_font_chroma48medium8_r);
    display.drawString(0, 1, tempT); 
    display.drawString(0, 3, timeT);  
  }
};

#endif
