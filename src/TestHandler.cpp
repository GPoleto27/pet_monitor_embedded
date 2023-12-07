#include "TestHandler.h"

namespace TestHandler
{
    bool setup()
    {
        xTaskCreatePinnedToCore(testerTask, "testerTask", 50000, NULL, 1, NULL, 0);
        return true;
    }

    void testerTask(void *pvParameters)
    {
        while (true)
        {
            ServerHandler::sendAllData();
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }
} // namespace TestHandler