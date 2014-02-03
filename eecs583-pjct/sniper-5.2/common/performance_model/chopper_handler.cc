#include "chopper_handler.h"

void ChopperHandler::handleSquash() {
	chopperAction->perform();

	numRegionSquashes++;
}
