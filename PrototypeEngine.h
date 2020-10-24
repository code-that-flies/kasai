//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_PROTOTYPEENGINE_H
#define KASAI_PROTOTYPEENGINE_H

#include "Utility.h"
#include "EventEngine.h"
#include "File.h"

class PrototypeEngine {
public:
    map<string, vector<int>> id_by_name;
    map<string, vector<Prototype*>> ptr_by_name;
    map<Prototype*, string> name_by_ptr;
    vector<string> name_by_id;

    map<string, Prototype*> boundQueries_by_tag;

    void RegisterPrototype(string name, Prototype* prototype);

    void Bind(string name, Prototype* query, bool override);

    static void End();

    unsigned int MakeID(string name, Prototype* item);
};


#endif //KASAI_PROTOTYPEENGINE_H
