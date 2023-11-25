#pragma once

#include <WiFi.h>
#include <PubSubClient.h>
#include <MsgPack.h>
#include <map>

#include "UUID.h"

#include "macros.h"

typedef struct InterruptDataStruct
{
    bool presence;
    int weightMilligrams;
    unsigned long timestamp;
    String imageFilename;
} InterruptData;

namespace MosquittoHandler
{
    extern WiFiClient espClient;
    extern PubSubClient client;

    bool setup();
    void mosquittoTask(void *pvParameter);
    void messageCallback(char *topic, byte *payload, unsigned int length);
    void publishDeviceMAC();
    void publishInterruptData(InterruptData *data);
    void publishPicture(String picPath);
}