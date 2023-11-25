#include "InterruptHandler.h"

#include "LoadCellHandler.h"
#include "TimestampHandler.h"
#include "MosquittoHandler.h"
#include "ImageHandler.h"

namespace InterruptHandler
{
    // Task handler
    TaskHandle_t interruptTaskHandle = NULL;

    bool setup()
    {
        pinMode(PRESENCE_SENSOR_PIN, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(PRESENCE_SENSOR_PIN), handleInterrupt, CHANGE);

        xTaskCreatePinnedToCore(interruptTask, "interruptTask", 1024, NULL, 1, &interruptTaskHandle, 0);

        return interruptTaskHandle != NULL;
    }

    void IRAM_ATTR handleInterrupt()
    {
        xTaskNotifyFromISR(interruptTaskHandle, 0, eNoAction, NULL);
    }

    void interruptTask(void *pvParameters)
    {
        while (true)
        {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            bool presence = digitalRead(PRESENCE_SENSOR_PIN) == HIGH;
            int weightMilligrams = LoadCellHandler::getWeightMilligrams();
            unsigned long timestamp = TimestampHandler::getTimestamp();

            String picPath;
            bool success = ImageHandler::takePicture(&picPath);

            if (!success)
            {
                Serial.println("Failed to take picture");
                continue;
            }

            InterruptData data = {presence, weightMilligrams, timestamp, picPath};
            Serial.println("Sending interrupt data");

            MosquittoHandler::publishInterruptData(&data);
            MosquittoHandler::publishPicture(picPath);

            // TODO: Send interrupt data to server
        }
    }
} // namespace InterruptHandler