#include <Arduino.h>
#include "FCTUC.h"

FCTUC bot;

void setup() {
	Serial.begin(115200);
	
	bot.begin();
	bot.beginOTA("teste");

	bot.waitStart();
}

void loop() {
	bot.println("prints");
	bot.moveMotors(-1000, 1000);
  	delay(100);
}
