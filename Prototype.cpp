//
// Created by Nathaniel Blair on 27/9/20.
//

#include "Prototype.h"
#include "typed_Prototype.h"

string Prototype::to_string(int indentation) {
    return "";
}


void Prototype::append(string name, int value) {
    auto val = new typed_Prototype<int>(this, value, ETYPE::INT);
    (*subvalues_map)[name] = val;
}

void Prototype::append(string name, float value) {
    (*subvalues_map)[name] = new typed_Prototype<float>(this, value, ETYPE::FLOAT);
}

void Prototype::append(string name, string value) {
    (*subvalues_map)[name] = new typed_Prototype<string>(this, value, ETYPE::STRING);
}

void Prototype::append(string name, Prototype *value) {
    (*subvalues_map)[name] = value;
    value->parent = this;
    value->etype = ETYPE::NONE;
}

Prototype::~Prototype() {
    if (subvalues_map != NULL) {
        for( auto const& [index, val] : *subvalues_map ) {
            delete val;
        }
        delete subvalues_map;
    }
    if (subvalues_col != NULL) {
        for( auto const& val : *subvalues_col ) {
            delete val;
        }
        delete subvalues_col;
    }
}

Prototype *Prototype::_search(string name, ETYPE type, Comparator comparison) {
    if (subvalues_map->find(name) != subvalues_map->end()) {
        if ((*subvalues_map)[name]->etype == type)
            return (*subvalues_map)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
}

Prototype *Prototype::_search(string name, int value, Comparator comparison) {
    if (subvalues_map->find(name) != subvalues_map->end()) {
        if ((*subvalues_map)[name]->etype == ETYPE::INT)
            return (*subvalues_map)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
    return nullptr;
}

Prototype *Prototype::_search(string name, float value, Comparator comparison) {
    if (subvalues_map->find(name) != subvalues_map->end()) {
        if ((*subvalues_map)[name]->etype == ETYPE::FLOAT)
            return (*subvalues_map)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
}

Prototype *Prototype::_search(string name, string value, Comparator comparison) {
    if (subvalues_map->find(name) != subvalues_map->end()) {
        if ((*subvalues_map)[name]->etype == ETYPE::STRING)
            return (*subvalues_map)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
}

void Prototype::append(Prototype *value) {
    if (subvalues_col == NULL)
        subvalues_col = new Table::Column();

    subvalues_col->push_back(value);
}



Prototype Prototype::search(string name, ETYPE type, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != NULL) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, type).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != NULL) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, type).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, type)->subvalues_col);


    return result;
}



Prototype Prototype::search(string name, int value, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != NULL) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, type).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != NULL) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, type).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, type)->subvalues_col);


    return result;
}
Prototype Prototype::search(string name, float value, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != NULL) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, value).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != NULL) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value)->subvalues_col);


    return result;
}



Prototype Prototype::search(string name, string value, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != NULL) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, type).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != NULL) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, type).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, type)->subvalues_col);


    return result;
}

void Prototype::initialize_col() {
    this->subvalues_col = new Table::Column();

}

void Prototype::merge(Table::Column& val) {
    if (this->subvalues_col != NULL)
        this->subvalues_col->insert( (*subvalues_col).end(), val.begin(), val.end() );
    else if (this->parent != NULL)
        this->parent->merge(val);
    else {
        // Do nothing
    }

}

Prototype Prototype::search(Prototype &query) {
    if (query.subvalues_col != nullptr) {
        for (auto* val : *(query.subvalues_col))

            // Use val->comparison to and val->type to search() for the value

            // Search [#person brother: [name: "Ryan"]]
            // Search(person).has(brother: { name: "Ryan" })

            // def has(prototype):
            //     for each property in prototype, if no match in this return false
            //     for each property match in prototype
            //          if not match.has(property) return false
            //     default: return true

    }
}
