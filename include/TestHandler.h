#pragma once

#include "macros.h"

#include "ServerHandler.h"

namespace TestHandler
{
    bool setup();
    void testerTask(void *pvParameters);
} // namespace TestHandler