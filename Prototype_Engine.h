//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_PROTOTYPE_ENGINE_H
#define KASAI_PROTOTYPE_ENGINE_H

#include "Utility.h"
#include "Event.h"
#include "File.h"

class Prototype_Engine {
public:
    map<string, vector<int>> id_by_name;
    map<string, vector<Prototype*>> ptr_by_name;
    map<Prototype*, string> name_by_ptr;
    vector<string> name_by_id;

    map<string, Prototype*> boundQueries_by_tag;

    static unsigned int IDENTITY;

    static Prototype_Engine* prototypeEngine;

    void RegisterPrototype(string name, Prototype* prototype);

    void Bind(string name, Prototype* query, bool override);

    static void End() {
        delete prototypeEngine;
        delete Event::event_engine;

        for( auto const& [key, val] : File::files ) {
            delete val;
        }
    }

    unsigned int MakeID(string name, Prototype* item);
};


#endif //KASAI_PROTOTYPE_ENGINE_H
