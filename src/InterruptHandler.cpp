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

        xTaskCreate(interruptTask, "interruptTask", 1024, NULL, 1, &interruptTaskHandle);

        return interruptTaskHandle != NULL;
    }

    void IRAM_ATTR handleInterrupt()
    {
        xTaskNotifyFromISR(interruptTaskHandle, 0, eNoAction, NULL);
    }

    void interruptTask(void *pvParameters)
    {
        Serial.println("Interrupt task started");
        while (true)
        {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            Serial.println("Interrupt detected");

            bool presence = digitalRead(PRESENCE_SENSOR_PIN) == HIGH;
            int weightGrams = LoadCellHandler::getWeightGrams();
            unsigned long timestamp = TimestampHandler::getTimestamp();

            String picPath;
            bool success = ImageHandler::takePicture(&picPath);

            if (!success)
            {
                Serial.println("Failed to take picture");
                continue;
            }

            InterruptData data = {presence, weightGrams, timestamp, picPath};
            Serial.println("Sending interrupt data");

            // TODO: Send interrupt data to server
            MosquittoHandler::publishInterruptData(&data);
            MosquittoHandler::publishPicture(picPath);
        }
    }
} // namespace InterruptHandler