#include "LoadCellHandler.h"
#include "PersistHandler.h"

namespace LoadCellHandler
{
	HX711 loadCell;

	bool setup()
	{
		loadCell.begin(LOAD_CELL_DOUT_PIN, LOAD_CELL_SCK_PIN);

		float loadCellScale = PersistHandler::loadCellScale();
		loadCell.set_scale(loadCellScale);
		loadCell.tare();
		return true;
	}

	int getWeightMilligrams()
	{
		// Convert Kg to mg
		return (int)loadCell.get_units() * 1000000;
	}
} // namespace LoadCellHandler