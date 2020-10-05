//
// Created by Nathaniel Blair on 27/9/20.
//

#include "float_Prototype.h"

void float_Prototype::load(string raw) {
    this->value = std::stof(raw);
}