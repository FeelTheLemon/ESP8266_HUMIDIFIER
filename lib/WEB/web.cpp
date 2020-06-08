#include "web.h"
#include <wifi.h>
#include "webfs.h"


ESP8266WebServer WEB::server(80);
//holds the current upload
File WEB::fsUploadFile;

void WEB::init()
{
	SPIFFS.begin();
	{
		Dir dir = SPIFFS.openDir("/");
		while (dir.next()) {
			//String fileName = dir.fileName();
			//size_t fileSize = dir.fileSize();
			//DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
		}
	//DBG_OUTPUT_PORT.printf("\n");
	}


	//SERVER INIT
	//list directory
	server.on("/list", HTTP_GET, handleFileListOld);
	server.on("/listnew", HTTP_GET, handleFileList);
	//load editor
	server.on("/edit", HTTP_GET, []() {
	if (!handleFileRead("/edit.htm")) {
		server.send(404, "text/plain", "FileNotFound");
	}
	});
	//create file
	server.on("/edit", HTTP_PUT, handleFileCreate);
	//delete file
	server.on("/edit", HTTP_DELETE, handleFileDelete);
	//first callback is called after the request has ended with all parsed arguments
	//second callback handles file uploads at that location
	server.on("/edit", HTTP_POST, []() {
		server.send(200, "text/plain", "");
	}, handleFileUpload);

	//called when the url is not defined here
	//use it to load content from SPIFFS
	server.onNotFound([]() {
	if (!handleFileRead(server.uri())) {
		server.send(404, "text/plain", "FileNotFound");
	}
	});

	//get heap status, analog input value and all GPIO statuses in one json call
	server.on("/all", HTTP_GET, []() {
		String json = "{";
		json += "\"heap\":" + String(ESP.getFreeHeap());
		json += ", \"analog\":" + String(analogRead(A0));
		json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
		json += "}";
		server.send(200, "text/json", json);
		json = String();
	});
	server.begin();
	//DBG_OUTPUT_PORT.println("HTTP server started");
}

void WEB::handle()
{
	server.handleClient();
}