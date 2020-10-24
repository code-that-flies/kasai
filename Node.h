//
// Created by Nathaniel Blair on 24/10/20.
//

#ifndef KASAI_NODE_H
#define KASAI_NODE_H

#include "PrototypeEngine.h"
#include "File.h"

class Node {
public:
    PrototypeEngine* prototypeEngine;
    EventEngine* eventEngine;
    File* fileHandler;
    vector<Node*> friends;

    Node() {
        // Do nothing
    }

    Node(PrototypeEngine* prototypeEngine, EventEngine* eventEngine, File* fileHandler)
    : prototypeEngine(prototypeEngine), eventEngine(eventEngine), fileHandler(fileHandler){
        // Do nothing
    }

    ~Node() {
        for (Node* node : friends) {
            delete node;
        }
    }

    void Connect(Node* otherNode);

    void Receive(string message);

    void Relay(string message);

    virtual void Processs() {
        // Do nothing
        // Inherited classes should implement this method and do stuff
    }
};


#endif //KASAI_NODE_H
