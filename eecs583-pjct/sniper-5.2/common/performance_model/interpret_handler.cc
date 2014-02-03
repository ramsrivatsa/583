#include "interpret_handler.h"

void InterpretHandler::handleSquash() {
	interpretAction->perform();

	numRegionSquashes++;
}
