#ifndef RETRY_HANDLER_H
#define RETRY_HANDLER_H

#include "squash_handler.h"
#include "retry_action.h"

/*
 * This class implements a squash handler that repeatedly retries.
 *
 * @author blujay
 */
class RetryHandler : public SquashHandler {
public:
	RetryHandler() {
		retryAction = new RetryAction();
	}

	~RetryHandler() {
		delete retryAction;
	}

	void handleSquash();

private:
	RetryAction *retryAction;
};

#endif

