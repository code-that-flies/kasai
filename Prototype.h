//
// Created by Nathaniel Blair on 27/9/20.
//

#ifndef KASAI_PROTOTYPE_H
#define KASAI_PROTOTYPE_H

#include <string>
#include <map>

#include "Utility.h"
#include "Table.h"

class Prototype {
public:
    Prototype();;

    ~Prototype();

    //virtual void Update() = 0;
    //virtual void Update() = 0;
   // virtual void FillWithDefaults() = 0;
//   virtual bool Serialize() = 0;
    ETYPE etype;
    unsigned int identity;

    Prototype* parent = NULL;
    Table::Column * subvalues_col = nullptr;
    Table::Row * subvalues_row  = nullptr;
    Comparator* comparison = nullptr;

    void append(string name, Prototype* value);

    void append(Prototype* value);

    void _merge(map<string, Prototype*>& val);
    void _merge_subprototype(string name, Prototype* foreign, E_MERGE_MODE mode);

    void merge(Table::Column& val);
    void merge(map<string, Prototype*>& foreign, E_MERGE_MODE mode);


    Prototype* _search(string name, ETYPE type, Comparator comparison);
    Prototype* _search(string name, int value, IntComparator comparison);
    Prototype* _search(string name, float value, FloatComparator comparison);
    Prototype* _search(string name, bool value, BoolComparator comparison);
    Prototype* _search(string name, string value, StrComparator comparison);

    Prototype* _search(string tag);


    Prototype search(string name, ETYPE type, Comparator comparison, int& searchDepth);
    Prototype search(string name, int value, IntComparator comparison, int& searchDepth);
    Prototype search(string name, float value, FloatComparator comparison, int& searchDepth);
    Prototype search(string name, bool value, BoolComparator comparison, int& searchDepth);
    Prototype search(string name, string value, StrComparator comparison, int& searchDepth);

    Prototype search(Prototype& query);
    Prototype search(string tag);

    bool has_tag(string tag);
    Prototype& tag(string);

    Prototype* Get();
    Prototype* Get(int index);
    Prototype* Get(string key);
    Prototype stag(string);
    bool _has(string name,  Prototype* prototype, Comparator comparison);
    bool has(Prototype* prototype);

    virtual string to_string(int indentation);

    void initialize_col();
    void initialize_map();
};


#endif //KASAI_PROTOTYPE_H
