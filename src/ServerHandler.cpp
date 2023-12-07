#include "ServerHandler.h"

namespace ServerHandler
{
    HTTPClient http;

    bool setup()
    {
        // Send HTTP GET request to server
        http.begin(SERVER_HOST, SERVER_PORT, "/");
        int httpCode = http.GET();

        if (httpCode != HTTP_CODE_OK)
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            return false;
        }
        // Get payload string of HTTP response
        String payload = http.getString();
        Serial.println(payload);
        return true;
    }

    bool sendImage(String picPath)
    {
        String path = "/" + picPath;
        File file = SD_MMC.open(path, FILE_READ);
        if (!file)
        {
            Serial.println("Failed to open file in reading mode");
            file.close();
            return false;
        }

        http.begin(SERVER_HOST, SERVER_PORT, "/image?filename=" + picPath);
        http.addHeader("Content-Type", "image/jpeg");
        int httpCode = http.sendRequest("POST", &file, file.size());

        if (httpCode > 0)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
        else
        {
            Serial.println("Error on HTTP request");
            file.close();
            http.end();
            return false;
        }

        // Close the file.
        file.close();
        http.end();
        return httpCode == HTTP_CODE_OK;
    }

    bool sendPacket(String picPath)
    {
        bool presence = digitalRead(PRESENCE_SENSOR_PIN) == HIGH;
        int weightGrams = LoadCellHandler::getWeightGrams();
        unsigned long timestamp = TimestampHandler::getTimestamp();

        http.begin(SERVER_HOST, SERVER_PORT, "/event");
        // post as form
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String body = "type=" + String(presence) + "&weight=" + String(weightGrams) + "&timestamp=" + String(timestamp) + "&image=" + picPath;
        int httpCode = http.POST(body);

        if (httpCode > 0)
        {
            String payload = http.getString();
            Serial.println(payload);
            return httpCode == HTTP_CODE_CREATED;
        }
        else
        {
            Serial.println("Error on HTTP request");
            return false;
        }
    }

    bool sendAllData()
    {
        String picPath;
        if (!ImageHandler::takePicture(&picPath))
        {
            Serial.println("Failed to take picture");
            return false;
        }
        if (!sendPacket(picPath))
        {
            Serial.println("Failed to send packet");
            return false;
        }
        if (!sendImage(picPath))
        {
            Serial.println("Failed to take picture");
            return false;
        }
        Serial.println("Successfully sent all data");
        return true;
    }
} // namespace ServerHandler
