#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#define WIFI_SSID "petmonitor"
#define WIFI_PASSWORD "monitorpet"

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

// Pin definition for peripheral devices
#define PRESENCE_SENSOR_PIN 14
#define LOAD_CELL_DOUT_PIN 16
#define LOAD_CELL_SCK_PIN 4
#define SERVO_MOTOR_PIN 13

#define SERVO_MOTOR_OPEN_ANGLE 0
#define SERVO_MOTOR_CLOSED_ANGLE 90

#define SERVER_HOST "192.168.0.100"
#define SERVER_PORT 5000

#define configUSE_TASK_NOTIFICATIONS 1
