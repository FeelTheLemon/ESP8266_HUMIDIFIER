#include <Arduino.h>
#include <ota.h>
#include <wifi.h>
#include <web.h>
#include <led.h>
#include <log.h>
#include <hw.h>

#define LED_BUILTIN 2

void setup()
{
	// put your setup code here, to run once:
	pinMode(LED_BUILTIN, OUTPUT);

	WiFiE::init();
	OTA::init();

	LED::init();
	HW::init();
	WEB::init();

	LOG::Log("Init Done!");
}

char light = 0;
void blink()
{
	light = !light;
	digitalWrite(LED_BUILTIN, light);   // turn the LED on (HIGH is the voltage level)
}

void loop()
{
	WiFiE::handle();
	OTA::handle();
	WEB::handle();
	LED::handle();
	blink();
}