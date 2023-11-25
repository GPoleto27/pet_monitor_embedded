#include "TimestampHandler.h"

namespace TimestampHandler
{
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP, "pool.ntp.org");

	bool setup()
	{
		timeClient.begin();
		timeClient.update();
		return true;
	}

	void timestampTask(void *pvParameters)
	{
		TickType_t xLastWakeTime = xTaskGetTickCount();
		while (true)
		{
			timeClient.update();
			vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_PERIOD_MS);
		}
	}

	unsigned long getTimestamp()
	{
		return timeClient.getEpochTime();
	}
} // namespace TimestampHandler
