//
// Created by Nathaniel Blair on 27/9/20.
//

#include "Primal.h"

Primal::Command::Command(vector<string> tags, string command) : tags(tags), command(command) {
    // Do nothing
}

void Primal::Child(Thread *child) {
    this->children.push_back(child);
    this->Connect(child);
}

Primal::Primal() : Node(new PrototypeEngine(), new EventEngine(), new File()){

}

void Primal::NewThread() {
    Thread* thread = new Thread(this);

    Child(thread);
}

void Primal::End() {
    for(auto* thread: children) {
        thread->endFlag = true;
        thread->thread.join();
    }
}
