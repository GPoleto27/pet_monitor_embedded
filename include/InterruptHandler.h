#pragma once

#include "macros.h"

#include "ServerHandler.h"


namespace InterruptHandler
{
    bool setup();
    void interruptTask(void *pvParameters);
} // namespace InterruptHandler
