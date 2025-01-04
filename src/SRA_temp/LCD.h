#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <U8g2lib.h>

class LCD_display {
private:
  U8G2_SH1106_128X64_NONAME_F_HW_I2C oled; // Obiectul OLED pentru U8G2

public:
  // Constructor
  LCD_display()
    : oled(U8G2_R0, /* reset=*/ U8X8_PIN_NONE) {}

  // Inițializarea OLED-ului
  void init() {
    oled.begin();
    oled.clearBuffer();
    oled.setFont(u8g2_font_6x10_tf); // Font mic pentru o afișare optimă
  }

  // Afișează o valoare pe OLED
  void show_value(int value) {
    oled.clearBuffer();
    oled.setCursor(0, 10);  // Cursor pe prima linie
    oled.print("Valoare: ");
    oled.setCursor(60, 10);
    oled.print(value);
    oled.sendBuffer();
  }

  // Afișează meniul în funcție de stare
  void write_menu(byte state) {
    oled.clearBuffer(); // Ștergem buffer-ul pentru a nu suprascrie textul

    switch (state) {
      case 0:
        oled.setCursor(0, 10);
        oled.print("-> START");
        break;
      case 1:
        oled.setCursor(0, 10);
        oled.print("Set T_SET");
        break;
      case 2:
        oled.setCursor(0, 10);
        oled.print("Set T_Incalzire");
        break;
      case 3:
        oled.setCursor(0, 10);
        oled.print("Set T_mentinere");
        break;
      case 4:
        oled.setCursor(0, 10);
        oled.print("Set T_racire");
        break;
      case 5:
        oled.setCursor(0, 10);
        oled.print("Set Kp");
        break;
      case 6:
        oled.setCursor(0, 10);
        oled.print("Set Kl");
        break;
      case 7:
        oled.setCursor(0, 10);
        oled.print("Set Kd");
        break;
      default:
        oled.setCursor(0, 10);
        oled.print("Unknown state");
        break;
    }

    oled.sendBuffer(); // Trimite buffer-ul pentru a afișa textul
  }
};

#endif