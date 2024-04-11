#include <Arduino.h>
#include "../lib/FCTUC/FCTUC.h"

FCTUC bot;

void setup() {
	Serial.begin(115200);
	
	bot.begin();

	bot.waitStart();
}

void loop() {

	bot.moveMotors(500, 500)

}
