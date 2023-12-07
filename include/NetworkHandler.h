#pragma once

#include <WiFi.h>
#include <WebServer.h>

#include "PersistHandler.h"
#include "macros.h"

namespace NetworkHandler
{
	bool setup();
	bool connectToNetwork();
	void networkTask(void *pvParameters);
}