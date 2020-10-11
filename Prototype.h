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

enum ETYPE {
    STRING, INT, FLOAT, NONE=0
};

typedef bool (*Comparator)(Prototype*, Prototype*);
class Prototype {
public:

    Prototype() {

    };

    ~Prototype();

    //virtual void Update() = 0;
    //virtual void Update() = 0;
   // virtual void FillWithDefaults() = 0;
//   virtual bool Serialize() = 0;
    ETYPE etype;

    Prototype* parent = NULL;
    Table::Column * subvalues_col = nullptr;
    map<string, Prototype*> * subvalues_map  = nullptr;
    Comparator* comparison = nullptr;

    void append(string name, int value);
    void append(string name, float value);
    void append(string name, string value);
    void append(string name, Prototype* value);

    void append(Prototype* value);

    void merge(Table::Column& val);

    Prototype* _search(string name, ETYPE type, Comparator comparison);
    Prototype* _search(string name, int value, Comparator comparison);
    Prototype* _search(string name, float value, Comparator comparison);
    Prototype* _search(string name, string value, Comparator comparison);

    Prototype search(string name, ETYPE type, Comparator comparison);
    Prototype search(string name, int value, Comparator comparison);
    Prototype search(string name, float value, Comparator comparison);
    Prototype search(string name, string value, Comparator comparison);

    Prototype search(Prototype& query);

    Prototype* Get();
    Prototype* Get(int index);
    Prototype* Get(string key);

    bool _has(string name,  Prototype* prototype, Comparator comparison);
    bool has(Prototype* prototype);

    virtual string to_string(int indentation);

    void initialize_col();
};



#endif //KASAI_PROTOTYPE_H
