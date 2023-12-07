#pragma once

#include <Preferences.h>

#include "macros.h"

namespace PersistHandler
{
	extern Preferences preferences;

	bool setup();
	void setLoadCellScale(float scale);
	float loadCellScale();
	bool setDeviceMAC(String mac);
	String deviceUUID();
} // namespace PersistHandler