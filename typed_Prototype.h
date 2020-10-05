//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_TYPED_PROTOTYPE_H
#define KASAI_TYPED_PROTOTYPE_H

#include <map>
#include <string>
#include "Prototype.h"

using std::map;
using std::string;

template <class T>
class typed_Prototype : public Prototype {
public:
    static T defaultValue;
    T value;
    map<string, Prototype*> * subvalues = NULL;

   // virtual void Update(T value) = 0;
   // virtual void Update(T value) = 0;

    // Loads into value and subvalues from string
    virtual void load(string raw) = 0;
};


#endif //KASAI_TYPED_PROTOTYPE_H
