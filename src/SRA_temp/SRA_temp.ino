#include "OLED_Display.h"

OLED_Display oled;

void setup() {
  oled.init();
  Serial.begin(9600);
}

void loop() {
  oled.showText("32","Nu avem ora");
  delay(1000);
}
