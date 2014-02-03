#ifndef PROBABILITY_HANDLER_H
#define PROBABILITY_HANDLER_H

#include "squash_handler.h"
#include "chopper_action.h"
#include "interpret_action.h"
#include "retry_action.h"

class ProbabilityHandler : public SquashHandler {
public:
	ProbabilityHandler() {
		chopperAction = new ChopperAction;
		retryAction = new RetryAction;
		interpretAction = new InterpretAction;
	}
	
	~ProbabilityHandler() {
		delete chopperAction;
		delete retryAction;
		delete interpretAction;
	}

	void handleSquash();

private:
	ChopperAction * chopperAction;
	RetryAction * retryAction;
	InterpretAction *interpretAction;
};

#endif
