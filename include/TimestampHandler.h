#pragma once

#include <WiFiUdp.h>
#include <NTPClient.h>

#include "macros.h"

namespace TimestampHandler
{
	extern WiFiUDP ntpUDP;
	extern NTPClient timeClient;

	bool setup();
	void timestampTask(void *pvParameters);
	unsigned long getTimestamp();
} // namespace TimestampHandler
