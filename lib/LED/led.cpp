#include "led.h"
#include <log.h>

#include <Arduino.h>
#include <WS2812FX.h>
#include <FS.h>

#define LED_COUNT 1
#define LED_PIN 13

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define CONFIG_PATH "/led.config"
#define CONFIG_SAVE 5000

unsigned long config_last_save = 0;
bool config_modified = false;

struct led_config
{
	uint32_t color;
	uint8_t brightness;
	uint16_t speed;
	uint8_t mode;
} config;

// FFFFFFFF6553500

void LED::load_config()
{
	LOG::Log("LED::load_config()");
	SPIFFS.begin();
	if (SPIFFS.exists(CONFIG_PATH))
	{
		File file = SPIFFS.open(CONFIG_PATH, "r");

		char buf[19];
		memset(buf, '\0', sizeof(buf));

		char * color = buf;
		char * brightness = color + 6 + 1;
		char * speed = brightness + 2 + 1;
		char * mode = speed + 5 + 1;

		file.read((uint8_t*)color, 6);
		file.read((uint8_t*)brightness, 2);
		file.read((uint8_t*)speed, 5);
		file.read((uint8_t*)mode, 2);

		config.color = strtol(color, NULL, 16);
		config.brightness = strtol(brightness, NULL, 16);
		config.speed = strtol(speed, NULL, 10);
		config.mode = strtol(mode, NULL, 10);

		file.close();
	}
	else
	{
		config.color = 0xffa230;
		config.brightness = 255;
		config.speed = 0;
		config.mode = FX_MODE_STATIC;
		config_modified = true;
	}
}

void LED::save_config()
{
	LOG::Log("LED::save_config()");
	File file = SPIFFS.open(CONFIG_PATH, "w");
	char buf[15];

	sprintf(buf, "%06X%02X%05d%02d", 
		config.color,
		config.brightness,
		config.speed,
		config.mode
	);

	file.write((uint8_t*)&buf, sizeof(buf));
	file.close();
}

void LED::init()
{
	LOG::Log("LED::init()");
	load_config();

	ws2812fx.init();
	ws2812fx.setBrightness(config.brightness);
	ws2812fx.setSpeed(config.speed);
	ws2812fx.setColor(config.color);
	ws2812fx.setMode(config.mode);
	ws2812fx.start();
}

void LED::handle()
{
	ws2812fx.service();

	unsigned long now = millis();
	if (config_modified && now - config_last_save > CONFIG_SAVE)
	{
		save_config();
		config_modified = false;
		config_last_save = now;
	}
}

void LED::on()
{
	LOG::Log("LED::on()");
	ws2812fx.setBrightness(config.brightness);
}

void LED::off()
{
	LOG::Log("LED::off()");
	ws2812fx.setBrightness(0);
}

void LED::setBrightness(uint8_t b)
{
	LOG::Log("LED::setBrightness()");
	config.brightness = b;
	config_modified = true;
	ws2812fx.setBrightness(b);
}

uint8_t LED::getBrightness()
{
	LOG::Log("LED::getBrightness()");
	return ws2812fx.getBrightness();
}


void LED::setColor(uint8_t r, uint8_t g, uint8_t b)
{
	LOG::Log("LED::setColor(r,g,b)");
	ws2812fx.setColor(r, g, b);
	config.color = ws2812fx.getColor();
	config_modified = true;
}

void LED::setColor(uint32_t c)
{
	LOG::Log("LED::setColor(c)");
	config.color = c;
	config_modified = true;
	ws2812fx.setColor(c);
}

uint32_t LED::getColor()
{
	return ws2812fx.getColor();
}

void LED::setMode(uint8_t m)
{
	config.mode = m;
	config_modified = true;
	ws2812fx.setMode(m);
}

uint8_t LED::getMode()
{
	return ws2812fx.getMode();
}

void LED::setSpeed(uint16_t s)
{
	config.speed = s;
	config_modified = true;
	ws2812fx.setSpeed(s);
}

uint16_t LED::getSpeed()
{
	return ws2812fx.getSpeed();
}