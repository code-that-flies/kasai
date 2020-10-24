//
// Created by Nathaniel Blair on 24/10/20.
//

#include "Thread.h"
#include "Primal.h"

Thread::Thread(Primal* parent) : parent(parent), thread(&Run, this), endFlag(false) {
    parent->Child(this);
}

bool Thread::AttemptNewCommand() {
    if (parent->commands.size() - 1 > this->latestCommandIndex) {
        for (this->latestCommandIndex += 1; this->latestCommandIndex <= parent->commands.size(); latestCommandIndex++) {
            Parse(parent->commands[latestCommandIndex].tags[0], parent->commands[latestCommandIndex].command);
        }

        return true;
    }

    return false;
}

void Run(Thread *self) {
    while (!self->endFlag) {
        if(self->eventEngine->TryTrigger()) {// On successfully triggering event
            // TODO: do something
        }
        if(self->AttemptNewCommand()) {// On successfully receiving command
            // TODO: do something
        }
    }
}

