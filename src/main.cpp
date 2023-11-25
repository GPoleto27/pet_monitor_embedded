#include "macros.h"

#include "NetworkHandler.h"
#include "TimestampHandler.h"
#include "PersistHandler.h"
#include "LoadCellHandler.h"
#include "ServoHandler.h"
#include "ImageHandler.h"
#include "InterruptHandler.h"

void setup()
{
	Serial.begin(115200);

	if (!NetworkHandler::setup())
	{
		Serial.println("Failed to setup network");
		ESP.restart();
	}
	// Always setup network before timestamp, as it need to sync time with NTP
	if (!TimestampHandler::setup())
	{
		Serial.println("Failed to setup timestamp");
		ESP.restart();
	}
	if (!PersistHandler::setup())
	{
		Serial.println("Failed to setup persist");
		ESP.restart();
	}
	// Always setup persist before load cell as it need to load scale from persist
	if (!LoadCellHandler::setup())
	{
		Serial.println("Failed to setup load cell");
		ESP.restart();
	}
	if (!ServoHandler::setup())
	{
		Serial.println("Failed to setup servo");
		ESP.restart();
	}

	if (!ImageHandler::setup())
	{
		Serial.println("Failed to setup image handler");
		ESP.restart();
	}

	xTaskCreatePinnedToCore(NetworkHandler::networkTask, "networkTask", 1024, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(TimestampHandler::timestampTask, "timestampTask", 1024, NULL, 1, NULL, 0);

	// Always setup interrupt handler last, as it depends on other handlers
	if (!InterruptHandler::setup())
	{
		Serial.println("Failed to setup interrupt handler");
		ESP.restart();
	}
}

void loop()
{
	String picPath;
	ImageHandler::takePicture(&picPath);
	Serial.println(picPath);
	delay(1000);
}
