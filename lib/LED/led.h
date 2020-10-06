#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

namespace LED
{
	void init();
	void handle();
	void load_config();
	void save_config();

	void on();
	void off();

	void setBrightness(uint8_t b);
	uint8_t getBrightness();

	void setColor(uint8_t r, uint8_t g, uint8_t b);
	void setColor(uint32_t c);
	uint32_t getColor();

	void setMode(uint8_t m);
	uint8_t getMode();

	void setSpeed(uint16_t s);
	uint16_t getSpeed();
} // namespace LED

#endif //__LED_H__