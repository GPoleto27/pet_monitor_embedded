#include "InterruptHandler.h"

namespace InterruptHandler
{

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    TaskHandle_t interruptTaskHandle = NULL;

    bool setup()
    {
        Serial.println("Setting up pin mode");

        Serial.println("Creating interrupt task");
        xTaskCreatePinnedToCore(interruptTask, "Interrupt task", 50000, NULL, 1, &interruptTaskHandle, 0);

        /*
        https://github.com/espressif/esp-who/issues/90
        we can only once initialize the GPIO interrupt service - either via attachInterrupt or via esp_camera_init (interrupt needed for vsync).
        Since we use the camera, we can't use attachInterrupt with his low level initializing of GPIO interrupt service.
        */
        Serial.println("Attaching interrupt");
        gpio_num_t gpio_num = static_cast<gpio_num_t>(PRESENCE_SENSOR_PIN);
        esp_err_t err = gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY);
        if (err != ESP_OK)
        {
            Serial.printf("set pull mode failed with error 0x%x \r\n", err);
        }
        err = gpio_isr_handler_add(gpio_num, reinterpret_cast<gpio_isr_t>(&Ext_INT1_ISR), NULL);
        if (err != ESP_OK)
        {
            Serial.printf("handler add failed with error 0x%x \r\n", err);
        }
        err = gpio_set_intr_type(gpio_num, GPIO_INTR_NEGEDGE);
        if (err != ESP_OK)
        {
            Serial.printf("set intr type failed with error 0x%x \r\n", err);
        }
        return true;
    }

    static void IRAM_ATTR Ext_INT1_ISR()
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(interruptTaskHandle, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }

    void interruptTask(void *pvParameters)
    {
        Serial.println("Interrupt task started");
        while (true)
        {
            // wait for interrupt
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            Serial.println("Interrupt detected");
            Serial.println("Sending interrupt data");
            if (!ServerHandler::sendAllData())
            {
                Serial.println("Failed to send interrupt data");
                ulTaskNotifyTake(pdTRUE, 0);
                continue;
            }
            Serial.println("Interrupt data sent");
            // clear the task notification value
            ulTaskNotifyTake(pdTRUE, 0);
        }
    }
} // namespace InterruptHandler