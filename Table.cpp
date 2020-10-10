//
// Created by Nathaniel Blair on 5/10/20.
//

#include "Table.h"
#include "Prototype.h"
#include "typed_Prototype.h"

#include <vector>

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

Table::Column::Column() {

}

