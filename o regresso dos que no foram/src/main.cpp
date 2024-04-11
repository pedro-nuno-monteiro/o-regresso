#include <Arduino.h>
#include "FCTUC.h"

FCTUC bot;

void setup() {
	Serial.begin(115200);
	
	bot.begin();

	bot.waitStart();
}

void loop() {

	bot.moveMotors(500, 500);
  delay(100);
}
