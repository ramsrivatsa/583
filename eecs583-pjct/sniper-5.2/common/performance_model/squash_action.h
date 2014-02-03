#ifndef SQUASH_ACTION_H
#define SQUASH_ACTION_H

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

//#include "simulator.h"
//#include "core_manager.h"

#define DEBUG 0

const int CYCLE_LATENCY = 30;
const int MEGA = 1000000;
const int NANO = 1000000000;

/*
 * This is an abstract class that represents a squash action.
 *
 * @author blujay
 */
class SquashAction {
public:
	// EFFECTS: Performs an action on the current basic block
	virtual void perform() = 0;
};

#endif

