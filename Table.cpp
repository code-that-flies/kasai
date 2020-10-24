//
// Created by Nathaniel Blair on 5/10/20.
//

#include "Table.h"
#include "Prototype.h"
#include "TypedPrototype.h"
#include "MemberPrototype.h"
#include "PrototypeEngine.h"


// TODO
void Table::Add(string name, Prototype defaultValue) {

}

Table::Column::~Column() {
    for (auto* prototype : *this) {
        delete prototype;
    }
}

void Table::Column::append(string val) {
    push_back(new TypedPrototype<string>(val, ETYPE::STRING));
}

void Table::Column::append(int val) {

    push_back(new TypedPrototype<int>(val, ETYPE::INT));
}

void Table::Column::append(float val) {
    push_back(new TypedPrototype<float>(val, ETYPE::FLOAT));
}

void Table::Column::append(bool val) {
    push_back(new TypedPrototype<bool>(val, ETYPE::BOOL));
}

void Table::Column::append(MemberFunction mb_fn) {
    push_back(new MemberPrototype(mb_fn));
}

Table::Column::Column() {

}


void Table::Row::append(Node* currentNode, string name, string val, bool override) {
    if (this->find(name) == this->end() || override) {
        auto* item = new TypedPrototype<string>(val, ETYPE::STRING);
        item->identity = currentNode->prototypeEngine->MakeID(name, item);
        (*this)[name] = item;
    }
}

void Table::Row::append(Node* currentNode, string name, bool val, bool override) {
    if (this->find(name) == this->end() || override) {
        auto* item = new TypedPrototype<bool>(val, ETYPE::BOOL);
        item->identity = currentNode->prototypeEngine->MakeID(name, item);
        (*this)[name] = item;

    }
}

void Table::Row::append(Node* currentNode, string name, int val, bool override) {
    if (this->find(name) == this->end() || override) {
        auto* item =  new TypedPrototype<int>(val, ETYPE::INT);
        item->identity = currentNode->prototypeEngine->MakeID(name, item);
        (*this)[name] = item;

    }
}

void Table::Row::append(Node* currentNode, string name, float val, bool override) {
    if (this->find(name) == this->end() || override) {
        auto* item = new TypedPrototype<float>(val, ETYPE::FLOAT);
        item->identity = currentNode->prototypeEngine->MakeID(name, item);
        (*this)[name] = item;

    }
}

void Table::Row::append(Node* currentNode, string name, MemberFunction mb_fn, bool override) {
    if (this->find(name) == this->end() || override) {
        auto* item = new MemberPrototype(mb_fn);
        item->identity = currentNode->prototypeEngine->MakeID(name, item);
        (*this)[name] = item;

    }
}

// Override is always
void Table::Row::commit(Node* currentNode, string name, string val, ETYPE etype) {
    // TODO ( this->commit(name, parse(val, etype)) );
}

// Override is always
void Table::Row::commit(Node* currentNode, string name, Prototype *item) {
    item->identity = currentNode->prototypeEngine->MakeID(name, item);
    (*this)[name] = item;
}

// TODO: replace these fillins with '<', '>', '==', '>=', '<=', '!='
bool StrComparison(Prototype* prototype, string value) {
    return true;
}
// TODO: replace these fillins with '<', '>', '==', '>=', '<=', '!='
bool IntComparison(Prototype* prototype, int value) {
    return true;
}
// TODO: replace these fillins with '<', '>', '==', '>=', '<=', '!='
bool FloatComparison(Prototype* prototype, float value) {
    return true;
}
// TODO: replace these fillins with '<', '>', '==', '>=', '<=', '!='
bool BoolComparison(Prototype* prototype, bool value) {
    return true;
}

// TODO: test and VALIDATE cursor / searchDepth iteration count
void Table::Data_Structure::AttemptToTriggerEvent(Node* currentNode, string name, string val) {
    // Search the search queries bound to events for commits
    // Remember the search depth
    // Use the search depth and go to the destination and go up by that depth amount and has() with the query from there to double check that the commit has a match

    for (auto const&[eventName, query] : currentNode->prototypeEngine->boundQueries_by_tag) {
        auto searchDepth = -1;

        auto searchResult = query->search(name, val, StrComparison, searchDepth);

        if (searchResult.subvalues_col->empty())
            continue;

        auto* cursor = this->parent;

        for (int i = 0; i < searchDepth; i++) {
            cursor = cursor->parent;
        }

        auto hasQuery = cursor->has(query);

        if (hasQuery) {
            EventEngine::Trigger(eventName, cursor);
        }
    }
}

// TODO: test and VALIDATE cursor / searchDepth iteration count
void Table::Data_Structure::AttemptToTriggerEvent(Node* currentNode, string name, int val) {
    // Search the search queries bound to events for commits
    // Remember the search depth
    // Use the search depth and go to the destination and go up by that depth amount and has() with the query from there to double check that the commit has a match

    for (auto const&[eventName, query] : currentNode->prototypeEngine->boundQueries_by_tag) {
        auto searchDepth = -1;

        query->search(name, val, IntComparison, searchDepth);

        auto* cursor = this->parent;

        for (int i = 0; i < searchDepth; i++) {
            cursor = cursor->parent;
        }

        auto hasQuery = cursor->has(query);

        if (hasQuery) {
            EventEngine::Trigger(eventName, cursor);
        }
    }
}

// TODO: test and VALIDATE cursor / searchDepth iteration count
void Table::Data_Structure::AttemptToTriggerEvent(Node* currentNode, string name, float val) {
    // Search the search queries bound to events for commits
    // Remember the search depth
    // Use the search depth and go to the destination and go up by that depth amount and has() with the query from there to double check that the commit has a match

    for (auto const&[eventName, query] : currentNode->prototypeEngine->boundQueries_by_tag) {
        auto searchDepth = -1;

        query->search(name, val, FloatComparison, searchDepth);

        auto* cursor = this->parent;

        for (int i = 0; i < searchDepth; i++) {
            cursor = cursor->parent;
        }

        auto hasQuery = cursor->has(query);

        if (hasQuery) {
            EventEngine::Trigger(eventName, cursor);
        }
    }
}

// TODO: test and VALIDATE cursor / searchDepth iteration count
void Table::Data_Structure::AttemptToTriggerEvent(Node* currentNode, string name, bool val) {
    // Search the search queries bound to events for commits
    // Remember the search depth
    // Use the search depth and go to the destination and go up by that depth amount and has() with the query from there to double check that the commit has a match

    for (auto const&[eventName, query] : currentNode->prototypeEngine->boundQueries_by_tag) {
        auto searchDepth = -1;

        query->search(name, val, BoolComparison, searchDepth);

        auto* cursor = this->parent;

        for (int i = 0; i < searchDepth; i++) {
            cursor = cursor->parent;
        }

        auto hasQuery = cursor->has(query);

        if (hasQuery) {
            EventEngine::Trigger(eventName, cursor);
        }
    }
}
