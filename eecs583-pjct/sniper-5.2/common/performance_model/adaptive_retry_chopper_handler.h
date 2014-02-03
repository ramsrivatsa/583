#ifndef ADAPTIVE_RETRY_CHOPPER_HANDLER_H
#define ADAPTIVE_RETRY_CHOPPER_HANDLER_H

#include "squash_handler.h"
#include "retry_action.h"
#include "chopper_action.h"


class AdaptiveRetryChopperHandler : public SquashHandler {
public:
	AdaptiveRetryChopperHandler() {
		retryAction = new RetryAction();
		chopperAction = new ChopperAction();
	}

	~AdaptiveRetryChopperHandler() { 
		delete retryAction;
		delete chopperAction;
	}
	
	void handleSquash();

private:
	RetryAction *retryAction;
	ChopperAction *chopperAction;
};

#endif
