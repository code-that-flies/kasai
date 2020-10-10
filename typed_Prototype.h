//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_TYPED_PROTOTYPE_H
#define KASAI_TYPED_PROTOTYPE_H

#include <map>
#include <string>
#include <fstream>
#include "Prototype.h"

using std::map;
using std::string;


template <class T>
class typed_Prototype : public Prototype {
public:
//    static T defaultValue;
    T value;

//    typed_Prototype() : value(defaultValue) {
//
//    };

    typed_Prototype<T>(T val, ETYPE etype);
    typed_Prototype<T>(Prototype* parent, T val, ETYPE etype);



   // virtual void Update(T value) = 0;
   // virtual void Update(T value) = 0;

    // Loads into value and subvalues from string
    virtual void load(string raw) {

    };

};

template<class T>
typed_Prototype<T>::typed_Prototype(T val, ETYPE etype) : value(val) {
    this->etype = etype;
}

template<class T>
typed_Prototype<T>::typed_Prototype(Prototype *parent, T val, ETYPE etype) : value(val) {
    this->parent = parent;
}

#endif //KASAI_TYPED_PROTOTYPE_H
