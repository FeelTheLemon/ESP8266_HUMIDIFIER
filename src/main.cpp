#include <Arduino.h>
#include <ota.h>
#include <wifi.h>
#include <web.h>

#define LED_BUILTIN 2

void setup()
{
	// put your setup code here, to run once:
	WiFiE::init();
	OTA::init();
	WEB::init();
	pinMode(LED_BUILTIN, OUTPUT);
}

char light = 0;
void blink()
{
	light = !light;
	digitalWrite(LED_BUILTIN, light);
}

void loop()
{
	WiFiE::handle();
	OTA::handle();
	WEB::handle();
	blink();
}