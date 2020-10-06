#ifndef __WEB_H__
#define __WEB_H__

#include <FS.h>
#include <ESP8266WebServer.h>

namespace WEB
{
	extern ESP8266WebServer server;
	//holds the current upload
	extern File fsUploadFile;

    void init();
	void handle();
} // namespace WEB

#endif //__WEB_H__