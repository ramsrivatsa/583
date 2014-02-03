#include "adaptive_retry_interpret_handler.h"

void AdaptiveRetryInterpretHandler::handleSquash() {
	if ((numRegionSquashes > 0) && ((numRegionSquashes%MAX_RETRIES) == 0)) {
		// pass execution to interpreter once in a while
//		std::cout<<"Interpret " <<std::endl;
		interpretAction->perform();
	}
																				  else {
		retryAction->perform();
	}
	
//	std::cout<<"Retry"<<numRegionSquashes<<std::endl;
	numRegionSquashes++;
}
