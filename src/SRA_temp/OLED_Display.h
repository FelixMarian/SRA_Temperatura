#ifndef display_DISPLAY_H
#define display_DISPLAY_H

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
    strcpy(temp_dorC, " | ");
    strcpy(temp_curC, "Temp: ");
    strcpy(timeReC, "Timp ramas: ");

    if(state == 0) strcpy(stateC, "Hold on    ");
    if(state == 1) strcpy(stateC, "Incalzire   ");
    if(state == 2) strcpy(stateC, "Mentinere   ");
    if(state == 3) strcpy(stateC, "Racire    ");

    strcat(temp_curC, temp_cur);
    strcat(temp_curC, temp_dorC);
    strcat(temp_curC, temp_dor);
    strcat(timeReC, timeRe);

    display.setFont(u8x8_font_chroma48medium8_r);
    display.drawString(0, 6, temp_curC);  
    display.drawString(0, 7, timeReC);  
    display.drawString(0, 4, stateC);  
  }

    // Afișează o valoare pe display
  void show_value(int value) {
    display.setCursor(0, 10);  // Cursor pe prima linie
    display.print("Valoare: ");
    display.setCursor(60, 10);
    display.print(value);
  }

  // Afișează meniul în funcție de stare
  void write_menu(byte state) {

    switch (state) {
      case 0:
        display.setCursor(0, 50);
        display.print("-> START     ");
        break;
      case 1:
        display.setCursor(0, 10);
        display.print("Set T_SET     ");
        break;
      case 2:
        display.setCursor(0, 10);
        display.print("Set T_Incalzire ");
        break;
      case 3:
        display.setCursor(0, 10);
        display.print("Set T_mentinere ");
        break;
      case 4:
        display.setCursor(0, 10);
        display.print("Set T_racire   ");
        break;
      case 5:
        display.setCursor(0, 10);
        display.print("Set Kp      ");
        break;
      case 6:
        display.setCursor(0, 10);
        display.print("Set Kl      ");
        break;
      case 7:
        display.setCursor(0, 10);
        display.print("Set Kd      ");
        break;
      default:
        display.setCursor(0, 10);
        display.print("Unknown state");
        break;
    }
  }
};

#endif
