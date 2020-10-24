//
// Created by Nathaniel Blair on 27/9/20.
//
#include <vector>
#include <string>
#include <map>


using std::string;
using std::vector;
using std::map;

#ifndef KASAI_PRIMAL_H
#define KASAI_PRIMAL_H

#include "Prototype.h"
#include "Node.h"
#include "Thread.h"

class Primal : public Node {
public:
    struct Command {
        vector<string> tags;
        string command;

        Command(vector<string> tags, string command);
        Command(string type, string command);
    };


    vector<Thread*> children;
    vector<Command> commands;
    map<int, Prototype> results;

    void Child(Thread* child);

    Primal();

    void NewThread();

    void MergeIntoResult(Prototype* result, unsigned int index);

    void End();
};

#endif //KASAI_PRIMAL_H
