#include "OLED_Display.h"
#include "Clock.h"

OLED_Display oled;
Clock clock;


void setup() {
  oled.init();
  Serial.begin(9600);
  clock.init();
  clock.setClockTime(0);
}

void loop() {
  oled.showText("32","Nu avem ora");
  clock.returnClock();
  delay(1000);
}
