#include "log.h"
#include <WiFiUdp.h>

WiFiUDP channel;

void LOG::Log(String msg)
{
	#ifdef LOGGING

	const char* buf = msg.c_str();
	size_t buf_size = strlen(buf);

	channel.beginPacket(LOG_SERVER_IP, LOG_SERVER_PORT);
	channel.write(buf, buf_size);
	channel.write("\n");
	channel.endPacket();
	
	#endif
}