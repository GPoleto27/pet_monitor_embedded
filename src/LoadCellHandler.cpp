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
		// loadCell.tare();
		Serial.println("Tare: 1000");
		// Serial.printf("Tare: %f\n", loadCell.get_units());
		return true;
	}

	int getWeightGrams()
	{
		// Convert Kg to g
		// return (int)loadCell.get_units() * 1000;
		return 1000;
	}
} // namespace LoadCellHandler