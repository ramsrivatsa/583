#ifndef RETRY_ACTION_H
#define RETRY_ACTION_H

#include "squash_action.h"

class RetryAction : public SquashAction {
public:
	void perform();
};

#endif

