#include <Arduino.h>
#include <WiFi.h>

#include "esp_camera.h"
#include "UUID.h"
#include "FS.h"               // SD Card ESP32
#include "SD_MMC.h"           // SD Card ESP32
#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"    // Disable brownour problems


#define WIFI_SSID "Guilherme_Oesteline"
#define WIFI_PASSWORD "naotemsenha"

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

int pictureNumber = 0;
WiFiServer server(80);
WiFiClient client;
UUID uuid;
uint32_t start, stop, randomtime;

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // Serial.println();

  uint32_t seed1 = random(999999999);
  uint32_t seed2 = random(999999999);
  uuid.seed(seed1, seed2);
  uuid.generate();
  uuid.toCharArray();
  Serial.print("UUID Setup: ");
  Serial.println(uuid);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  if (!SD_MMC.begin())
  {
    Serial.println("SD Card Mount Failed");
    return;
  }

  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No SD Card attached");
    return;
  }

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected.");
  server.begin();
  Serial.println("Server started.");
}

void take_picture()
{
  camera_fb_t *fb = NULL;

  // Take Picture with Camera
  fb = esp_camera_fb_get();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    return;
  }

  // Path where new picture will be saved in SD Card
  uuid.generate();

  String path = "/" + String(uuid.toCharArray()) + ".jpg";

  fs::FS &fs = SD_MMC;
  Serial.printf("Picture file name: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file in writing mode");
  }
  else
  {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
  }
  file.close();
  esp_camera_fb_return(fb);

  // send image to client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: image/jpeg");
  client.println("Connection: close"); // the connection will be closed after completion of the response
  client.println();
  client.write(fb->buf, fb->len);
  delay(1);
  client.stop();
}

void get_picture(String id)
{
  // Read JPG file
  String path = "/" + id + ".jpg";

  fs::FS &fs = SD_MMC;
  Serial.printf("Picture file name: %s\n", path.c_str());

  File file = fs.open(path.c_str(), FILE_READ);
  if (!file)
  {
    // send 404 error
    Serial.println("Failed to open file in reading mode");
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/html");
    client.println("Connection: close"); // the connection will be closed after completion of the response
    client.println();
    client.println("<!DOCTYPE HTML><html><body><h1>404 Not Found</h1></body></html>");
    delay(1);
    client.stop();
  }
  else
  {
    // send image to client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: image/jpeg");
    client.println("Connection: close"); // the connection will be closed after completion of the response
    client.println();
    while (file.available())
    {
      client.write(file.read());
    }
    delay(1);
    client.stop();
  }
}

void stream()
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
  client.println("Connection: close"); // the connection will be closed after completion of the response
  client.println();
  while (client.connected())
  {

    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb)
    {
      Serial.println("Camera capture failed");
      return;
    }
    client.println("--frame");
    client.println("Content-Type: image/jpeg");
    client.println("Content-Length: " + String(fb->len));
    client.println();
    client.write(fb->buf, fb->len);
    client.println();
    esp_camera_fb_return(fb);
  }
}

void loop()
{
  while (!client)
  {
    delay(100);
    client = server.available(); // listen for incoming clients
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);

  // GET / HTTP/1.1
  // GET /picture/<id> HTTP/1.1
  // GET /gallery HTTP/1.1

  // filter for GET / HTTP/1.1
  if (request.indexOf("GET / HTTP/1.1") != -1)
  {
    take_picture();
  }

  // filter for GET /picture/<id> HTTP/1.1
  if (request.indexOf("GET /picture/") != -1)
  {
    String id = request.substring(request.indexOf("/picture/") + 9, request.indexOf("HTTP") - 1);
    get_picture(id);
  }

  if (request.indexOf("GET /stream HTTP/1.1") != -1)
  {
    stream();
  }

  if (client.connected())
  {
    client.stop();
  }

  // filter for GET /gallery HTTP/1.1
  // if

  // take_picture();

  // Serial.println("Client disconnected");

  /*
  Serial.println("Going to sleep now");
  delay(2000);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
  */
}
