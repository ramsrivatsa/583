#ifndef MEM_COUNT_HANDLER_H
#define MEM_COUNT_HANDLER_H

#include "squash_handler.h"
#include "chopper_action.h"
#include "interpret_action.h"
#include "retry_action.h"

class MemCountHandler : public SquashHandler {
public:
	MemCountHandler() {
		chopperAction = new ChopperAction;
		retryAction = new RetryAction;
		interpretAction = new InterpretAction;
	}
	
	~MemCountHandler() {
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
