#include "adaptive_retry_chopper_handler.h"

void AdaptiveRetryChopperHandler::handleSquash() {
	if ((numRegionSquashes > 0) && ((numRegionSquashes%MAX_RETRIES) == 0)) {
		chopperAction->perform();
	}
	else {
		retryAction->perform();
	}

	numRegionSquashes++;
}
