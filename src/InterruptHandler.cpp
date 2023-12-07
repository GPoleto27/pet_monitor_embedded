#include "InterruptHandler.h"

namespace InterruptHandler
{

    bool setup()
    {
        Serial.println("Setting up pin mode");
        pinMode(PRESENCE_SENSOR_PIN, INPUT_PULLUP);

        Serial.println("Creating interrupt task");
        xTaskCreate(interruptTask, "Interrupt task", 1024, NULL, 1, NULL);
        return true;
    }

    void interruptTask(void *pvParameters)
    {
        Serial.println("Interrupt task started");
        while (true)
        {
            if (digitalRead(PRESENCE_SENSOR_PIN) == HIGH)
            {
                vTaskDelay(100 / portTICK_PERIOD_MS);
                continue;
            }

            // wait for interrupt
            Serial.println("Interrupt detected");

            if (!ServerHandler::sendAllData())
            {
                Serial.println("Failed to send interrupt data");
                continue;
            }

            Serial.println("Interrupt data sent");

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
} // namespace InterruptHandler