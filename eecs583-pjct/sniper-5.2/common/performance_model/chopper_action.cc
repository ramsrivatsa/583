#include "chopper_action.h"
#include "simulator.h"
#include "core_manager.h"
#include <queue>

//class CoreManager;

void ChopperAction::perform() {
	// get current core from simulator
	Core *currentCore = Sim()->getCoreManager()->getCurrentCore();
	
	// get perf model from current core
	PerformanceModel *perfModel = currentCore->getPerformanceModel();

	std::map<uint64_t, std::set<Operand*>> *memAccesses = perfModel->getMemoryAccesses();

	// get memory instructions from perfModel
	std::queue<Instruction*> *memInstructions = perfModel->getMemoryInstructions();
	
	// set all memory instructions to translated
	while(memInstructions->size()) {
		Instruction *ins = memInstructions->front();
		ins->translate();
		memInstructions->pop();
	}

	// penalize the core for translation
	//FIXME fix frequency here when we find an accurate way to get it
        const ComponentPeriod *componentPeriod = currentCore->getDvfsDomain();
        unsigned long long freq = componentPeriod->getPeriodInFreqMHz();
/*	unsigned long long freq = SimGetOwnFreqMHz();*/
	double period = (double)1/(double(freq*MEGA));
	int sleepDuration = static_cast<int>(ceil(3000*period*(NANO)));

	SubsecondTime currentTime = perfModel->getElapsedTime();
	
	SubsecondTime wakeupTime = currentTime + SubsecondTime::NS(sleepDuration);

	perfModel->queueDynamicInstruction(new SyncInstruction(wakeupTime, SyncInstruction::SLEEP));

	for(std::map<uint64_t, std::set<Operand *>>::iterator it = memAccesses->begin(); it != memAccesses->end(); it++) {
		it->second.clear();
	}
	memAccesses->clear();
}	
