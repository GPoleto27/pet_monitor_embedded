#pragma once
#include "HTTPClient.h"

#include "NetworkHandler.h"
#include "ImageHandler.h"
#include "LoadCellHandler.h"
#include "TimestampHandler.h"
#include "macros.h"

namespace ServerHandler
{
    extern HTTPClient http;

    bool setup();
    bool sendImage(String picPath);
    bool sendPacket(String picPath);
    bool sendAllData();
} // namespace ServerHandler