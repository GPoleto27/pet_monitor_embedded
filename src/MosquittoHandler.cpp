#include "MosquittoHandler.h"
#include "ImageHandler.h"
#include "LoadCellHandler.h"
#include "ServoHandler.h"

namespace MosquittoHandler
{
    WiFiClient espClient;
    PubSubClient client(espClient);
    const char *MAC = WiFi.macAddress().c_str();

    bool setup()
    {
        client.setServer(MQTT_SERVER, MQTT_PORT);
        client.setCallback(processMessage);
        return true;
    }

    void mosquittoTask(void *pvParameter)
    {
        while (!client.connected())
        {
            Serial.println("Connecting to MQTT...");
            if (client.connect("ESP32Client"))
            {
                Serial.println("connected");
                client.subscribe((String(MAC) + "/commands").c_str());
                publishDeviceMAC();
            }
            else
            {
                Serial.print("failed with state ");
                Serial.print(client.state());
                vTaskDelay(2000 / portTICK_PERIOD_MS);
            }
        }

        while (true)
        {
            client.loop();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    void processMessage(char *topic, byte *payload, unsigned int length)
    {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++)
        {
            Serial.print((char)payload[i]);
        }
        Serial.println();

        if (strcmp(topic, (String(MAC) + "/commands").c_str()) == 0)
        {
            if (strcmp((char *)payload, "take-picture") == 0)
            {
                String picPath;
                if (ImageHandler::takePicture(&picPath))
                {
                    Serial.println("Picture taken");
                    client.publish((String(MAC) + "/pictures").c_str(), picPath.c_str());
                }
                else
                {
                    Serial.println("Error taking picture");
                }
            }
            else if (strcmp((char *)payload, "get-weight") == 0)
            {
                int weight = LoadCellHandler::getWeightMilligrams();
                Serial.print("Weight: ");
                Serial.println(weight);
                client.publish((String(MAC) + "/weight").c_str(), String(weight).c_str());
            }
            else if (strcmp((char *)payload, "open-door") == 0)
            {
                ServoHandler::open();
                client.publish((String(MAC) + "/door").c_str(), "open");
            }
            else if (strcmp((char *)payload, "close-door") == 0)
            {
                ServoHandler::close();
                client.publish((String(MAC) + "/door").c_str(), "closed");
            }
        }
    }

    void publishDeviceMAC()
    {
        client.publish("/mac", MAC);
    }

    void publishInterruptData(InterruptData *data)
    {
        MsgPack::Packer packer;
        packer.packBool(data->presence);
        packer.packInt32(data->weightMilligrams);
        packer.packInt64(data->timestamp);
        packer.packString32(data->imageFilename);

        client.publish((String(MAC) + "/events").c_str(), packer.data(), packer.size());
    }
} // namespace MosquittoHandler