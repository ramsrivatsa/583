#include "interpret_action.h"
#include "simulator.h"
#include "core_manager.h"

void InterpretAction::perform() {
	// get current core from simulator
	Core *currentCore = Sim()->getCoreManager()->getCurrentCore();
	
	// get perf model from current core
	PerformanceModel *perfModel = currentCore->getPerformanceModel();

	std::map<uint64_t, std::set<Operand*>> *memAccesses = perfModel->getMemoryAccesses();	

	// calculate current time
	SubsecondTime currentTime = perfModel->getElapsedTime();

	SubsecondTime executionCost = perfModel->getCost();

	// get core frequency
	// FIXME this does not return an accurate core frequency - reported
	// frequency is usually much lower than expected
	/* const ComponentPeriod *componentPeriod = currentCore->getDvfsDomain();
        unsigned long long freq = componentPeriod->getPeriodInFreqMHz();
	double period = (double)1/(double(freq*MEGA));
	double decPeriodInNS = ((period*(NANO)));
	int sleepDuration = static_cast<int>(ceil(decPeriodInNS*(double)CYCLE_LATENCY*(double)REGION_SIZE));
	*/

#if DEBUG
	/*std::cout << "Frequency of core is " << freq << " MHz" << std::endl;
        std::cout << "Period of core is " << period << " s" << std::endl;
        std::cout << "Period of core is " << decPeriodInNS << " ns" << std::endl;
	std::cout << "Sleep duration: " << sleepDuration << " ns" << std::endl;
	*/
	std::cout << "Execution cost: " << executionCost.getPS() << " ps" << std::endl;
	std::cout << "Cycle latency: " << CYCLE_LATENCY << std::endl;
#endif

	SubsecondTime wakeupTime = currentTime + CYCLE_LATENCY*executionCost; //SubsecondTime::NS(sleepDuration);

	perfModel->queueDynamicInstruction(new SyncInstruction(wakeupTime, SyncInstruction::SLEEP));

	for(std::map<uint64_t, std::set<Operand*>>::iterator it = memAccesses->begin(); it != memAccesses->end(); it++) {
		it->second.clear();
	}
	memAccesses->clear();

}

