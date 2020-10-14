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
    vector<string> name_by_id;

    static unsigned int IDENTITY;

    static Prototype_Engine* prototypeEngine;

    static void End() {
        delete prototypeEngine;
        delete Event::event_engine;

        for( auto const& [key, val] : File::files ) {
            delete val;
        }
    }

    static unsigned int MakeID(string name) {
        if (prototypeEngine->id_by_name.find(name) == prototypeEngine->id_by_name.end()) {
            prototypeEngine->id_by_name[name] = vector<int>();
        }
        prototypeEngine->id_by_name[name].push_back(prototypeEngine->name_by_id.size());
        prototypeEngine->name_by_id.push_back(name);

        return prototypeEngine->name_by_id.size() - 1;
    }
};


#endif //KASAI_PROTOTYPE_ENGINE_H
