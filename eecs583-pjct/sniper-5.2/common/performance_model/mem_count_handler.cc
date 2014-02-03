#include "mem_count_handler.h"
#include <queue>
#include "simulator.h"
#include "core_manager.h"

extern const int REGION_SIZE;

void MemCountHandler::handleSquash() {
	Core * currentCore = Sim()->getCoreManager()->getCurrentCore();

	PerformanceModel * perfModel = currentCore->getPerformanceModel();

	std::queue<Instruction*> *memInstructions = perfModel->getMemoryInstructions();

	if(memInstructions->size() < REGION_SIZE / 20 && numRegionSquashes < 5) {
		retryAction->perform();
	}
	
	else if(memInstructions->size() < REGION_SIZE / 20) {
		interpretAction->perform();
	}
	
	else {
		chopperAction->perform();
	}

	numRegionSquashes++;	
}
