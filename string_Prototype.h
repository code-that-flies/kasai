//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_STRING_PROTOTYPE_H
#define KASAI_STRING_PROTOTYPE_H

#include <string>
#include "typed_Prototype.h"


using std::string;

class string_Prototype : public typed_Prototype<string> {
public:
    void load(string raw) override;

    string to_string(int indentation) override;
};


#endif //KASAI_STRING_PROTOTYPE_H
