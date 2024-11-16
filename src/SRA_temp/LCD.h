#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal.h>

class LCD {
private:
  byte RS, E, D4, D5, D6, D7;
  LiquidCrystal lcd16x2;
public:
  LCD(byte RSc, byte Ec, byte D4c, byte D5c, byte D6c, byte D7c)
    : RS(RSc), E(Ec), D4(D4c), D5(D5c), D6(D6c), D7(D7c),
      lcd16x2(RS, E, D4, D5, D6, D7) {}
  void init() {
    lcd16x2.begin(16, 2);
  }

  void show_value(int value){
    lcd16x2.setCursor(0,0);
    lcd16x2.print("               ");
    lcd16x2.setCursor(4,0);
    lcd16x2.print(value);
  }

  void write_menu(byte state) {
    switch (state) {
      case 0:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("-> START     ");
        break;
      case 1:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("Set T_SET        ");
        break;
      case 2:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("Set T_Incalzire        ");
        break;
      case 3:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("Set T_mentinere       ");
        break;
      case 4:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("Set T_racire          ");
        break;
      case 5:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("Set Kp          ");
        break;
      case 6:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("Set Kl          ");
        break;
      case 7:
        lcd16x2.setCursor(0, 0);
        lcd16x2.write("Set Kd          ");
        break;
      default:
        Serial.println("Unknown state");
        break;
    }
  }
};

#endif