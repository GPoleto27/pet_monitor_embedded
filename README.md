# Pet Monitor Embedded

This is the embedded component of the Pet Monitor project. 
For now, it is a simple server running on an ESP32 with a camera module. It provides endpoints for taking pictures, retrieving pictures, and streaming video.
The objective is to generate events based on a interruption from a presence sensor, and send the picture, load cell data and timestamp to the Pet Monitor Server. The event will be used to generate data about the pet's diet and serve the data to the Pet Monitor App. The server will be responsible by sending commands to the Pet Monitor Embedded to open or close the food dispenser.

## Features

- Take a picture and save it to the SD card with a unique UUID.
- Retrieve a picture by its UUID.
- Stream video from the camera.

## Todo

- [ ] Implement interruption by a presence sensor.
- [ ] Send the picture, load cell data and timestamp to the Pet Monitor Server.
- [ ] Use FreeRTOS to handle the interruption and task scheduling.
- [ ] Decide between using HTTP or MQTT to send the data to the Pet Monitor Server.
- [ ] Implement a command to open or close the food dispenser.

## Requirements

- ESP32 development board with a camera module.
- SD card for storing pictures.
- Load cell (1Kg)
- HX711 module
- Presence sensor
- Servo motor

## Setup

1. Connect your ESP32 board to your computer.
2. Install the [PlatformIO IDE extension for VSCode](https://platformio.org/install/ide?install=vscode).
3. Open the project in VSCode.
4. Build and upload the project to your ESP32 board.
5. Open the serial monitor to see the server's IP address.
6. Connect to the server's WiFi network.
7. Open the server's IP address in your browser.

## Endpoints

Once the server is running, you can access the following endpoints:

| Endpoint | Type | Description |
| --- | --- | --- |
| `/` | GET | Takes a picture and returns it as a response. |
| `/picture/<uuid>` | GET | Returns the picture with the given UUID. |
| `/stream` | GET | Streams video from the camera. |

## Security Note

This project is not intended for production use. It does not implement any form of authentication or encryption.
