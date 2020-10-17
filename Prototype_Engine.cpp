//
// Created by Nathaniel Blair on 14/10/20.
//

#include "Prototype_Engine.h"
#include "Prototype.h"

Prototype_Engine* Prototype_Engine::prototypeEngine = new Prototype_Engine();
unsigned int Prototype_Engine::IDENTITY = 0;

void Prototype_Engine::RegisterPrototype(string name, Prototype *prototype) {
    if (this->ptr_by_name.find(name) == this->ptr_by_name.end()) {
        this->ptr_by_name[name] = vector<Prototype*>();
    }

    this->ptr_by_name[name].push_back(prototype);
    name_by_ptr[prototype] = name;
}

unsigned int Prototype_Engine::MakeID(string name, Prototype *item) {
    if (this->id_by_name.find(name) == this->id_by_name.end()) {
        this->id_by_name[name] = vector<int>();
    }

    this->id_by_name[name].push_back(this->name_by_id.size());
    this->name_by_id.push_back(name);

    RegisterPrototype(name, item);

    return this->name_by_id.size() - 1;
}

void Prototype_Engine::Bind(string name, Prototype * query, bool overwrite) {
    if (boundQueries_by_tag.find(name) == boundQueries_by_tag.end())
        boundQueries_by_tag[name] = query;
    else if (overwrite)
        boundQueries_by_tag[name] = query;
}
// TESTING NEW USERNAME
