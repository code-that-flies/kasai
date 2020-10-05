//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_TABLE_H
#define KASAI_TABLE_H
#include <vector>

#include "Prototype.h"
#include "string_Prototype.h"

using std::vector;

class Table : string_Prototype {
public:
    class Column : public typed_Prototype<vector<Prototype*>> {

    };

    // TODO
    void Add(string name, Prototype defaultValue) {

    }

    // TODO
    void Add(string name) {

    }
};


#endif //KASAI_TABLE_H
