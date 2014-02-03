#ifndef INTERPRET_HANDLER_H
#define INTERPRET_HANDLER_H

#include "squash_handler.h"
#include "interpret_action.h"

//class InterpretAction;

/*
 * This class implements a squash handler that repeatedly interprets.
 *
 * @author blujay
 */
class InterpretHandler : public SquashHandler {
public:
	InterpretHandler() {
		interpretAction = new InterpretAction();
	}

	~InterpretHandler() {
		delete interpretAction;
	}

	void handleSquash();

private:
	InterpretAction *interpretAction;
};

#endif

