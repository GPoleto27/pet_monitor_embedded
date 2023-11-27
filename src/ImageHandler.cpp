#include "ImageHandler.h"

namespace ImageHandler
{
	UUID uuid;

	bool setup()
	{
		uint32_t seed1 = random(999999999);
		uint32_t seed2 = random(999999999);
		uuid.seed(seed1, seed2);
		uuid.generate();
		uuid.toCharArray();
		Serial.print("UUID Setup: ");
		Serial.println(uuid);

		WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

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
			Serial.println("PSRAM found");
			config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
			config.jpeg_quality = 10;
			config.fb_count = 2;
		}
		else
		{
			Serial.println("PSRAM not found");
			config.frame_size = FRAMESIZE_SVGA;
			config.jpeg_quality = 12;
			config.fb_count = 1;
		}

		esp_err_t err = esp_camera_init(&config);
		if (err != ESP_OK)
		{
			Serial.printf("Camera init failed with error 0x%x\n", err);
			return false;
		}
		Serial.println("Camera init success");

		if (!SD_MMC.begin())
		{
			Serial.println("SD Card Mount Failed");
			return false;
		}
		uint8_t cardType = SD_MMC.cardType();
		if (cardType == CARD_NONE)
		{
			Serial.println("No SD Card attached");
			return false;
		}
		Serial.print("SD Card Type: ");
		Serial.println(cardType == CARD_SD ? "SDSC" : cardType == CARD_SDHC ? "SDHC"
																			: "UNKNOWN");

		return true;
	}

	bool takePicture(String *picPath)
	{
		// Path where new picture will be saved in SD Card
		uuid.generate();

		String path = "/" + String(uuid.toCharArray()) + ".jpg";

		fs::FS &fs = SD_MMC;

		camera_fb_t *fb = NULL;

		// Take Picture with Camera
		fb = esp_camera_fb_get();
		if (!fb)
		{
			Serial.println("Camera capture failed");
			return false;
		}

		File file = fs.open(path.c_str(), FILE_WRITE);
		if (!file)
		{
			Serial.println("Failed to open file in writing mode");
			file.close();
			esp_camera_fb_return(fb);
			return false;
		}

		file.write(fb->buf, fb->len); // payload (image), payload length
		Serial.printf("Saved file to path: %s\n", path.c_str());
		file.close();
		esp_camera_fb_return(fb);

		*picPath = path;
		return true;
	}

	size_t packPicture(String picPath, byte *buffer)
	{
		Serial.printf("Packing picture: %s\n", picPath.c_str());

		fs::FS &fs = SD_MMC;

		File file = fs.open(picPath.c_str(), FILE_READ);
		if (!file)
		{
			Serial.println("Failed to open file for reading");
			return 0;
		}

		String bufferString;

		while (file.available())
		{
			bufferString += (char)file.read();
		}

		file.close();

		Serial.printf("Picture size: %d\n", bufferString.length());

		buffer = (byte *)malloc(bufferString.length());
		memcpy(buffer, bufferString.c_str(), bufferString.length());

		return bufferString.length();
	}
}
