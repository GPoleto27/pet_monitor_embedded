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

	void persistLoadCellScale(float scale)
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
} // namespace PersistHandler