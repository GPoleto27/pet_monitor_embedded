#pragma once

#include "macros.h"

#include "ServerHandler.h"

namespace InterruptHandler
{
    bool setup();
    void interruptTask(void *pvParameters);
    static void IRAM_ATTR Ext_INT1_ISR();
} // namespace InterruptHandler
