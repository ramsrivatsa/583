#ifndef INTERPRET_ACTION_H
#define INTERPRET_ACTION_H

#include "squash_action.h"

// parameter value
extern const int REGION_SIZE;

// FIXME the current implementation may affect the accuracy of memory in simulation. For example,
//       by placing the current core to sleep for such an extended time, the state of all the other
//       cores will most likely change significantly.

/*
 * This class represents an interpret (action). When execution is passed to the interpreter,
 * each instruction becomes much more expensive, usually by a factor of cycle latency. To
 * simulate this added cost, this action inserts a sleep instruction that effectively increases
 * the cost of each instruction by adding a sleep instruction that lasts for that amount of time.
 *
 * @author blujay
 */
class InterpretAction : public SquashAction {
public:
	void perform();
};

#endif

