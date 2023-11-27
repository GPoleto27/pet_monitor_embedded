#pragma once

#include <HX711.h>

#include "macros.h"

namespace LoadCellHandler
{
	extern HX711 loadCell;

	bool setup();
	int getWeightGrams();
} // namespace LoadCellHandler
