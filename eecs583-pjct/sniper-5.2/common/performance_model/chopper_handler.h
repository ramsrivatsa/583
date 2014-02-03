#ifndef CHOPPER_HANDLER_H
#define CHOPPER_HANDLER_H

#include "squash_handler.h"
#include "chopper_action.h"

class ChopperHandler : public SquashHandler {
public:
	ChopperHandler() {
		chopperAction = new ChopperAction();
	}

	~ChopperHandler() {
		delete chopperAction;
	}

	void handleSquash();

private:
	ChopperAction *chopperAction;
};

#endif
