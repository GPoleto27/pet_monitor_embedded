#include "LoadCellHandler.h"
#include "PersistHandler.h"

namespace LoadCellHandler
{
	HX711 loadCell;

	bool setup()
	{
		loadCell.begin(LOAD_CELL_DOUT_PIN, LOAD_CELL_SCK_PIN);

		float loadCellScale = PersistHandler::loadCellScale();
		Serial.printf("Load cell scale: %f\n", loadCellScale);
		loadCell.set_scale(loadCellScale);
		loadCell.tare();
		/*
		Serial.println("Tare done, put 1kg on the scale");
		sleep(10000);
		Serial.println("Calibrating...");
		// Calibrate known weight
		float knownWeight = 855;
		/*
		knownWeight = og_reading
		reading = x

		reading * og_reading = knownWeight * x
		x = (reading * og_reading) / knownWeight
		calibration_factor = og_reading / knownWeight

		this is grams
		float calibration_factor = loadCell.get_units(10) / knownWeight;
		loadCell.set_scale(calibration_factor);
		Serial.printf("Calibration factor: %f\nRemove the weight\n", calibration_factor);
		sleep(10000);
		loadCell.tare();
		PersistHandler::setLoadCellScale(calibration_factor);
		//Serial.println("Tare: 1000");
		Serial.printf("Calibration: %f\n", loadCell.get_units(10));
		*/
		return true;
	}

	int getWeightGrams()
	{
		// Convert Kg to g
		return (int)loadCell.get_units(10);
	}
} // namespace LoadCellHandler