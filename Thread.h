//
// Created by Nathaniel Blair on 24/10/20.
//

#ifndef KASAI_THREAD_H
#define KASAI_THREAD_H


#include "Node.h"
#include <Thread>

class Primal;
class Prototype;
class Thread : public Node {
public:
    bool endFlag;
    Primal* parent;
    std::thread thread;
    unsigned int latestCommandIndex;

    Prototype* root;

    Thread(Primal* parent);

    bool AttemptNewCommand();
};

void Run(Thread* self);


#endif //KASAI_THREAD_H
