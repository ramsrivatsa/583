#ifndef SQUASH_HANDLER_H
#define SQUASH_HANDLER_H
#define MAX_RETRIES 5
/*#include "core.h"
#include "performance_model.h"
#include "fastforward_performance_model.h"
#include "branch_predictor.h"
#include "simulator.h"
#include "simple_performance_model.h"
#include "iocoom_performance_model.h"
#include "magic_performance_model.h"
#include "oneipc_performance_model.h"
#include "interval_performance_model.h"
#include "core_manager.h"
#include "config.hpp"
#include "stats.h"
#include "dvfs_manager.h"
#include "instruction_tracer.h"*/

/*
 * This abstract class represents a squash handler.
 *
 * @author blujay
 */
class SquashHandler {
public:
	SquashHandler() : numRegionSquashes(0), numTotalSquashes(0), commitStatus(false) {}

	// EFFECTS: Performs actions for squash if conflict is detected in PerformanceModel
        //          and increments the squash count.
	virtual void handleSquash() = 0;

	// RETURNS: Number of squashes handled in the current core.
	virtual int getSquashCount() { return numTotalSquashes; }

	virtual void resetRegionCount() {
		numTotalSquashes += numRegionSquashes;
		numRegionSquashes = 0;
	}

	virtual bool canCommit() {
		return commitStatus;
	}

protected:
	int numRegionSquashes;
	int numTotalSquashes;
	bool commitStatus;
};

#endif

