#ifndef ADAPTIVE_RETRY_INTERPRET_HANDLER_H
#define ADAPTIVE_RETRY_INTERPRET_HANDLER_H

#include "squash_handler.h"
#include "retry_action.h"
#include "interpret_action.h"


/*
 * This class implements an adaptive retry squash handler that
 * passes execution to an interpreter after a number of retries.
 *
 * @author blujay
 */
class AdaptiveRetryInterpretHandler : public SquashHandler {
public:
	AdaptiveRetryInterpretHandler() {
		retryAction = new RetryAction();
		interpretAction = new InterpretAction();
	}

	~AdaptiveRetryInterpretHandler() {
		delete retryAction;
		delete interpretAction;
	}

	void handleSquash();

private:
	RetryAction *retryAction;
	InterpretAction *interpretAction;
};

#endif

