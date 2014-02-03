#include "probability_handler.h"
#include <queue>
#include "simulator.h"
#include "core_manager.h"

extern const int REGION_SIZE;

void ProbabilityHandler::handleSquash() {
	Core * currentCore = Sim()->getCoreManager()->getCurrentCore();

	PerformanceModel * perfModel = currentCore->getPerformanceModel();

	std::queue<Instruction*> *memInstructions = perfModel->getMemoryInstructions();

	double probability = ((memInstructions->size()/REGION_SIZE) + (numRegionSquashes / 5)) / 2;

	if(probability < 0.3) {
		retryAction->perform();
	}
	
	else if(probability > 0.3 && probability < 0.6) {
		interpretAction->perform();
	}
	
	else {
		chopperAction->perform();
	}

	numRegionSquashes++;	
}
