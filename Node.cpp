//
// Created by Nathaniel Blair on 24/10/20.
//

#include "Node.h"

void Node::Connect(Node *otherNode) {
    friends.push_back(otherNode);
}

void Node::Receive(string message) {
    // TODO: implement Receive
}

void Node::Relay(string message) {
    // TODO: implement relay

}
