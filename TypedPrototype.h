//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_TYPEDPROTOTYPE_H
#define KASAI_TYPEDPROTOTYPE_H

#include <map>
#include <string>
#include <fstream>
#include "Prototype.h"

using std::map;
using std::string;


template <class T>
class TypedPrototype : public Prototype {
public:
//    static T defaultValue;
    T value;

//    TypedPrototype() : value(defaultValue) {
//
//    };

    TypedPrototype<T>(T val, ETYPE etype);
    TypedPrototype<T>(Prototype* parent, T val, ETYPE etype);



   // virtual void Update(T value) = 0;
   // virtual void Update(T value) = 0;

    // Loads into value and subvalues from string
    virtual void load(string raw) {

    };

};

template<class T>
TypedPrototype<T>::TypedPrototype(T val, ETYPE etype) : value(val) {
    this->etype = etype;
}

template<class T>
TypedPrototype<T>::TypedPrototype(Prototype *parent, T val, ETYPE etype) : value(val) {
    this->parent = parent;
}

#endif //KASAI_TYPEDPROTOTYPE_H
