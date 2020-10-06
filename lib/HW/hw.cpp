#include "hw.h"
#include <SPI.h>

#define FAN_PIN 15

bool curr_state = false;

SPISettings settings = SPISettings(5000000, MSBFIRST, SPI_MODE1);

void HW::init()
{
	pinMode(FAN_PIN, OUTPUT);
	digitalWrite(FAN_PIN, HIGH);
	//SPI.begin();
	//SPI.setHwCs(false);
}

void HW::state(bool state)
{
	if (state && !curr_state) {
		//SPI.beginTransaction(settings);
		digitalWrite(FAN_PIN, HIGH);
		curr_state = true;
	} else if (!state && curr_state) {
		//SPI.endTransaction();
		digitalWrite(FAN_PIN, LOW);
		curr_state = false;
	}
}

void HW::on()
{
	state(true);
}

void HW::off()
{
	state(false);
}