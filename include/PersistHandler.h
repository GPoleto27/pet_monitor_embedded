#pragma once

#include <Preferences.h>

#include "macros.h"

namespace PersistHandler
{
	extern Preferences preferences;

	bool setup();
	void persistLoadCellScale(float scale);
	float loadCellScale();
} // namespace PersistHandler