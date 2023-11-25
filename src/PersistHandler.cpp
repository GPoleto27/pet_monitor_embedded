#include "PersistHandler.h"

namespace PersistHandler
{
	Preferences preferences;

	bool setup()
	{
		preferences.begin("loadCell", false);
		if (!preferences.isKey("scale"))
		{
			preferences.putFloat("scale", 1.0);
		}
		preferences.end();

		return true;
	}

	void setLoadCellScale(float scale)
	{
		preferences.begin("loadCell", false);
		preferences.putFloat("scale", scale);
		preferences.end();
	}

	float loadCellScale()
	{
		preferences.begin("loadCell", true);
		float scale = preferences.getFloat("scale", 1.0);
		preferences.end();

		return scale;
	}

	bool setDeviceMAC(String mac)
	{
		preferences.begin("device", false);
		preferences.putString("MAC", mac);
		preferences.end();

		return true;
	}

	String deviceMAC()
	{
		preferences.begin("device", true);
		String mac = preferences.getString("mac", "");
		preferences.end();

		return mac;
	}
} // namespace PersistHandler