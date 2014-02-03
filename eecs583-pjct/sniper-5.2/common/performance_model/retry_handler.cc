#include "retry_handler.h"

void RetryHandler::handleSquash() {
	retryAction->perform();

	numRegionSquashes++;
}
