#include "MosquittoHandler.h"
#include "ImageHandler.h"
#include "LoadCellHandler.h"
#include "ServoHandler.h"

namespace MosquittoHandler
{
    const char *ssid = WIFI_SSID;
    const char *password = WIFI_PASSWORD;

    // MQTT broker credentials (set to NULL if not required)
    const char *MQTT_username = MQTT_USER;
    const char *MQTT_password = MQTT_PASSWORD;

    // Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
    const char *mqtt_server = MQTT_SERVER;
    // For example
    // const char* mqtt_server = "192.168.1.106";

    // Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
    WiFiClient espClient;
    PubSubClient client(espClient);
    const char *MAC = WiFi.macAddress().c_str();

    bool setup()
    {
        client.setServer(MQTT_SERVER, MQTT_PORT);
        client.setCallback(messageCallback);
        // connect using username and password
        if (!client.connect(MQTT_ID, MQTT_USER, MQTT_PASSWORD))
        {
            Serial.println("MQTT connection failed");
            return false;
        }
        if (!client.subscribe("/commands"))
        {
            Serial.println("MQTT subscription failed");
            return false;
        }
        Serial.println("MQTT connection successful");

        return true;
    }

    void connect()
    {
        client.setServer(MQTT_SERVER, MQTT_PORT);
        client.setCallback(messageCallback);
        // set timeout to 60 seconds
        client.setSocketTimeout(60000);
        // connect using username and password
        while (!client.connect(MQTT_ID, MQTT_USER, MQTT_PASSWORD))
        {
            Serial.print("MQTT connection failed with state ");
            Serial.print(client.state());
            return;
        }
        Serial.println("MQTT connection successful");
        return;
    }

    void mosquittoTask(void *pvParameter)
    {
        Serial.println("Mosquitto task started");
        while (true)
        {
            client.loop();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    void messageCallback(char *topic, byte *payload, unsigned int length)
    {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++)
        {
            Serial.print((char)payload[i]);
        }
        Serial.println();

        if (strcmp(topic, "/commands") == 0)
        {
            Serial.print("Received command: ");
            Serial.println((char *)payload);
            if (strcmp((char *)payload, "take-picture") == 0)
            {
                String picPath;
                if (ImageHandler::takePicture(&picPath))
                {
                    Serial.println("Picture taken");
                    publishPicture(picPath);
                }
                else
                {
                    Serial.println("Error taking picture");
                }
            }
            else if (strcmp((char *)payload, "get-weight") == 0)
            {
                int weight = LoadCellHandler::getWeightGrams();
                Serial.print("Weight: ");
                Serial.println(weight);
                client.publish("/weight", String(weight).c_str());
            }
            else if (strcmp((char *)payload, "open-door") == 0)
            {
                ServoHandler::open();
                client.publish("/door", "open");
            }
            else if (strcmp((char *)payload, "close-door") == 0)
            {
                ServoHandler::close();
                client.publish("/door", "closed");
            }
            else if (strcmp((char *)payload, "mac") == 0)
            {
                publishDeviceMAC();
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

        client.publish("/events", packer.data(), packer.size());
    }

    void publishPicture(String picPath)
    {
        // TODO: still not able to publish picture
        byte buffer[20000];
        size_t size = ImageHandler::packPicture(picPath, buffer);

        Serial.printf("Publishing picture: %s; with pack size %d\n", picPath.c_str(), size);
        client.publish("/pictures", buffer, size);
        Serial.println("Picture published");
    }

} // namespace MosquittoHandler