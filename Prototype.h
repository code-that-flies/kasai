//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_PROTOTYPE_H
#define KASAI_PROTOTYPE_H

#include <string>
#include <map>

using std::string;
using std::map;

class Archive;
class Prototype {
public:
    //virtual void Update() = 0;
    //virtual void Update() = 0;
   // virtual void FillWithDefaults() = 0;
//   virtual bool Serialize() = 0;
    map<string, Prototype*> * subvalues = NULL;

    virtual string to_string(int indentation);;
};



#endif //KASAI_PROTOTYPE_H
