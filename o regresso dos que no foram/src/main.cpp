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
	bot.moveMotors(0, 500);
  	delay(100);
}
