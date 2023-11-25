#pragma once

#include "macros.h"

namespace InterruptHandler
{
    extern TaskHandle_t interruptTaskHandle;

    bool setup();
    void IRAM_ATTR handleInterrupt();
    void interruptTask(void *pvParameters);
} // namespace InterruptHandler
