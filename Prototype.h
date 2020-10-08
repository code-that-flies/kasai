//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_PROTOTYPE_H
#define KASAI_PROTOTYPE_H

#include <string>
#include <map>

#include "Table.h"

using std::string;
using std::map;

class Archive;
class Prototype {
public:
    //virtual void Update() = 0;
    //virtual void Update() = 0;
   // virtual void FillWithDefaults() = 0;
//   virtual bool Serialize() = 0;

    Table::Column * subvalues_col = NULL;
    map<string, Prototype*> * subvalues_map  = NULL;

    virtual string to_string(int indentation);;
};



#endif //KASAI_PROTOTYPE_H
