//
// Created by Nathaniel Blair on 5/10/20.
//

#include "Table.h"
#include "Prototype.h"
#include "typed_Prototype.h"
#include "member_Prototype.h"
#include "Prototype_Engine.h"


// TODO
void Table::Add(string name, Prototype defaultValue) {

}

Table::Column::~Column() {
    for (auto* prototype : *this) {
        delete prototype;
    }
}

void Table::Column::append(string val) {
    push_back(new typed_Prototype<string>(val, ETYPE::STRING));
}

void Table::Column::append(int val) {

    push_back(new typed_Prototype<int>(val, ETYPE::INT));
}

void Table::Column::append(float val) {
    push_back(new typed_Prototype<float>(val,ETYPE::FLOAT));
}

void Table::Column::append(bool val) {
    push_back(new typed_Prototype<bool>(val,ETYPE::BOOL));
}

void Table::Column::append(MemberFunction mb_fn) {
    push_back(new member_Prototype(mb_fn));
}

Table::Column::Column() {

}


void Table::Row::append(string name, string val) {
    if (this->find(name) == this->end()) {
        auto* item = new typed_Prototype<string>(val, ETYPE::STRING);
        item->identity = Prototype_Engine::MakeID(name);
        (*this)[name] = item;
    }
}

void Table::Row::append(string name, bool val) {
    if (this->find(name) == this->end()) {
        auto* item = new typed_Prototype<bool>(val, ETYPE::BOOL);
        item->identity = Prototype_Engine::MakeID(name);
        (*this)[name] = item;

    }
}

void Table::Row::append(string name, int val) {
    if (this->find(name) == this->end()) {
        auto* item =  new typed_Prototype<int>(val, ETYPE::INT);
        item->identity = Prototype_Engine::MakeID(name);
        (*this)[name] = item;

    }
}

void Table::Row::append(string name, float val) {
    if (this->find(name) == this->end()) {
        auto* item = new typed_Prototype<float>(val, ETYPE::FLOAT);
        item->identity = Prototype_Engine::MakeID(name);
        (*this)[name] = item;

    }
}

void Table::Row::append(string name, MemberFunction mb_fn) {
    if (this->find(name) == this->end()) {
        auto* item = new member_Prototype(mb_fn);
        item->identity = Prototype_Engine::MakeID(name);
        (*this)[name] = item;

    }
}