#pragma once

#include <HX711.h>

#include "macros.h"

namespace LoadCellHandler
{
	extern HX711 loadCell;

	bool setup();
	int getWeightMilligrams();
} // namespace LoadCellHandler
