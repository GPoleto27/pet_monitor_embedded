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
		xTaskCreate(TimestampHandler::timestampTask, "timestampTask", 9*1024, NULL, 1, NULL);
		return true;
	}

	void timestampTask(void *pvParameters)
	{
		Serial.println("Timestamp task started");
		while (true)
		{
			timeClient.update();
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}

	unsigned long getTimestamp()
	{
		// UTC-3
		return timeClient.getEpochTime() - 3 * 60 * 60;
	}
} // namespace TimestampHandler
