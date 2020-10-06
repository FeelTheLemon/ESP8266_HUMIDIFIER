#ifndef __LOG_H__
#define __LOG_H__

#include <stdint.h>
#include <WString.h>

#define LOG_SERVER_IP "10.1.1.153"
#define LOG_SERVER_PORT 1337

#define LOGGING

namespace LOG
{
	void Log(String msg);
} // namespace LOG

#endif //__LOG_H__