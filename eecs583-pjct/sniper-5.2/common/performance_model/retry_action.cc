#include "retry_action.h"
#include "simulator.h"
#include "core_manager.h"

void RetryAction::perform() {
	// get current core from simulator
	Core *currentCore = Sim()->getCoreManager()->getCurrentCore();
	
	// get perf model from current core
	PerformanceModel *perfModel = currentCore->getPerformanceModel();
	
	// calculate current time
	SubsecondTime currentTime = perfModel->getElapsedTime();

	SubsecondTime executionCost = perfModel->getCost();

	// get core frequency
	// FIXME this does not return an accurate core frequency - reported
	// frequency is usually much lower than expected
	const ComponentPeriod *componentPeriod = currentCore->getDvfsDomain();
	unsigned long long freq = componentPeriod->getPeriodInFreqMHz();
	//unsigned long long freq = SimGetOwnFreqMHz();

        //unsigned long long freq = SimGetOwnFreqMHz();
        double period = (double)1/(double(freq*MEGA));
        double decPeriodInNS = ((period*(double)(NANO)));
        //int sleepDuration = static_cast<int>(ceil(decPeri));

#if DEBUG
	std::cout << "Frequency of core is " << freq << " MHz" << std::endl;
        std::cout << "Period of core is " << period << " s" << std::endl;
        std::cout << "Period of core is " << decPeriodInNS << " ns" << std::endl;
	std::cout << "Sleep duration: " << sleepDuration << " ns" << std::endl;
#endif

	SubsecondTime wakeupTime = currentTime + executionCost; // SubsecondTime::NS(sleepDuration);

	perfModel->queueDynamicInstruction(new SyncInstruction(wakeupTime, SyncInstruction::SLEEP));
}
