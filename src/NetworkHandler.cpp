#include "NetworkHandler.h"

namespace NetworkHandler
{
	bool setup()
	{

		if (!connectToNetwork())
		{
			Serial.println("Failed to connect to WiFi");
			return false;
		}
		// xTaskCreate(NetworkHandler::networkTask, "networkTask", 1024, NULL, 1, NULL);
		return true;
	}

	bool connectToNetwork()
	{
		unsigned long maxWaitTime = millis() + 10000;
		WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
		Serial.print("Connecting to WiFi");
		while (WiFi.status() != WL_CONNECTED)
		{
			if (millis() > maxWaitTime)
			{
				Serial.println();
				Serial.println("Failed to connect to WiFi");
				return false;
			}
			Serial.print(".");
			delay(100);
		}
		Serial.println();
		Serial.print("Connected! IP address: ");
		Serial.println(WiFi.localIP());

		PersistHandler::setDeviceMAC(WiFi.macAddress());

		return true;
	}

	void networkTask(void *pvParameters)
	{
		Serial.println("Network task started");
		while (true)
		{
			if (WiFi.status() != WL_CONNECTED)
			{
				Serial.println("Disconnected from WiFi");
				bool reconnectSuccess = connectToNetwork();
				if (!reconnectSuccess)
				{
					Serial.println("Failed to reconnect to WiFi");
					ESP.restart();
				}
			}
			// print stack size
			Serial.print("Network task stack size: ");
			Serial.println(uxTaskGetStackHighWaterMark(NULL));

			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}
}