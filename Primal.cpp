//
// Created by Nathaniel Blair on 27/9/20.
//

#include "Primal.h"

Primal::Command::Command(vector<string> tags, string command) : tags(tags), command(command) {
    // Do nothing
}

Primal::Command::Command(string type, string command) : command(command) {
    this->tags = vector<string>();
    tags.push_back(type);
}

void Primal::Child(Thread *child) {
    this->children.push_back(child);
    this->Connect(child);
}

Primal::Primal() : Node(new PrototypeEngine(), new EventEngine(), new File()){

}

void Primal::NewThread() {
    auto* thread = new Thread(this);

    Child(thread);
}

void Primal::End() {
    for(auto* thread: children) {
        thread->endFlag = true;
        thread->thread.join();
    }
}

void Primal::MergeIntoResult(Prototype *result, unsigned int index) {
    if (results.find(index) != results.end())
        results[index] = *result;
    else
        results[index].merge(*result->subvalues_col);
    // Tidy up
    delete result;
}
