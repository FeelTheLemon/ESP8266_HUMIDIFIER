#include "wifi.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager


char WiFiE::hostname[15];

void configModeCallback (WiFiManager *myWiFiManager) {
	Serial.println("Entered config mode");
	Serial.println(WiFi.softAPIP());
	//if you used auto generated SSID, print it
	Serial.println(myWiFiManager->getConfigPortalSSID());
}

void WiFiE::init()
{
	sprintf(hostname, "esp8266-%06x", ESP.getChipId()); // you can override it

	WiFiManager wifiManager;
	//reset settings - for testing
	//wifiManager.resetSettings();

	//WiFi.setAutoConnect(true);
	//WiFi.setAutoReconnect(true);

	wifiManager.setAPCallback(configModeCallback);

	if(!wifiManager.autoConnect(hostname)) {
		Serial.println("failed to connect and hit timeout");
		//reset and try again, or maybe put it to deep sleep
		ESP.reset();
		delay(1000);
	} 
	MDNS.begin(hostname);
}

void WiFiE::handle()
{
	MDNS.update();
}