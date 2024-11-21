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
  void showText(const char* temp_dor, const char* temp_cur, const char* timeRe, int state) {
    char temp_dorC[25], temp_curC[25], timeReC[25], stateC[10];

    // Pentru a nu se concatena la fiecare apelare, "resetam" textul
    strcpy(temp_dorC, "Temp dorita: ");
    strcpy(temp_curC, "Temp reala: ");
    strcpy(timeReC, "Timp ramas: ");

    if(state == 0) strcpy(stateC, "Hold on");
    if(state == 1) strcpy(stateC, "Incalzire");
    if(state == 2) strcpy(stateC, "Mentinere");
    if(state == 3) strcpy(stateC, "Racire");

    strcat(temp_dorC, temp_dor);
    strcat(temp_curC, temp_cur);
    strcat(timeReC, timeRe);

    display.setFont(u8x8_font_chroma48medium8_r);
    display.drawString(0, 1, temp_dorC); 
    display.drawString(0, 3, temp_curC);  
    display.drawString(0, 5, timeReC);  
    display.drawString(0, 7, stateC);  
  }
};

#endif
