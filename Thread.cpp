//
// Created by Nathaniel Blair on 24/10/20.
//

#include "Thread.h"
#include "Primal.h"

Thread::Thread(Primal* parent)
: parent(parent), thread(&Run, this), endFlag(false),
Node(new PrototypeEngine(), new EventEngine(), new File()) {
    parent->Child(this);

    // TODO: intialise fileHandler with correct directory
}

bool Thread::AttemptNewCommand() {
    Prototype* result = nullptr;
    if (parent->commands.size() - 1 > this->latestCommandIndex) {
        for (this->latestCommandIndex += 1; this->latestCommandIndex <= parent->commands.size(); latestCommandIndex++) {
            result = Parse(parent->commands[latestCommandIndex].tags[0], parent->commands[latestCommandIndex].command);

            if (result != nullptr) {
                parent->MergeIntoResult(result, latestCommandIndex);
            }
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

