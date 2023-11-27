#include "TimestampHandler.h"

namespace TimestampHandler
{
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP, "pool.ntp.org");

	bool setup()
	{
		timeClient.begin();
		timeClient.update();
		Serial.print("Current time (UTC): ");
		Serial.println(timeClient.getFormattedTime());
		return true;
	}

	void timestampTask(void *pvParameters)
	{
		Serial.println("Timestamp task started");
		while (true)
		{
			timeClient.update();
			vTaskDelay(2000 / portTICK_PERIOD_MS);
		}
	}

	unsigned long getTimestamp()
	{
		// UTC-3
		return timeClient.getEpochTime() - 3 * 60 * 60;
	}
} // namespace TimestampHandler
