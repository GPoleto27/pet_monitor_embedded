#pragma once

#include "esp_camera.h"
#include "UUID.h"
#include "FS.h"               // File system ESP32
#include "SD_MMC.h"           // SD Card ESP32
#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"    // Disable brownour problems

#include "macros.h"

namespace ImageHandler
{
	extern UUID uuid;

	bool setup();
	bool takePicture(String *picPath);
} // namespace ImageHandler
