//
// Created by Nathaniel Blair on 27/9/20.
//

#include "string_Prototype.h"

void string_Prototype::load(string raw) {
    this->value = string(raw);
}

string string_Prototype::to_string(int indentation) {
    string result = value + ":\n";

/*
    for( auto const& [key, val] : subvalues ) {
        result += "\t" + key + ": " + val->to_string(1)
    }
*/
}
