//
// Created by Nathaniel Blair on 27/9/20.
//

#include "Prototype.h"
#include "typed_Prototype.h"

struct isValue
{
    string s_value;
    int i_value;
    float f_value;

    isValue(int value) : i_value(value) {}

    isValue(string value) : s_value(value) {}

    isValue(float value) : f_value(value) {}

    bool operator()(const Prototype *prototype) const
    {
        if (prototype->etype == ETYPE::NONE)
            return false;
        else if (prototype->etype == ETYPE::STRING) {
            return ((typed_Prototype<string>*)prototype)->value == s_value;
        }
        else if (prototype->etype == ETYPE::INT) {
            return ((typed_Prototype<int>*)prototype)->value == i_value;
        }
        else if (prototype->etype == ETYPE::FLOAT) {
            return ((typed_Prototype<float>*)prototype)->value == f_value;
        }
    }
};

unsigned int Prototype::IDENTITY = 0; // TESTING NEW USERNAME

Prototype::Prototype() {
    IDENTITY++;
    this->identity = IDENTITY;
}

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
    if (subvalues_map != nullptr) {
        for( auto const& [index, val] : *subvalues_map ) {
            delete val;
        }
        delete subvalues_map;
    }
    if (subvalues_col != nullptr) {
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
    if (subvalues_col == nullptr)
        subvalues_col = new Table::Column();

    subvalues_col->push_back(value);
}



Prototype Prototype::search(string name, ETYPE type, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != nullptr) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, type, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, type, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, type, comparison)->subvalues_col);


    return result;
}



Prototype Prototype::search(string name, int value, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != nullptr) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, value, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value, comparison)->subvalues_col);


    return result;
}
Prototype Prototype::search(string name, float value, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != nullptr) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, value, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value, comparison)->subvalues_col);


    return result;
}



Prototype Prototype::search(string name, string value, Comparator comparison) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != nullptr) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(name, value, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value, comparison).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value, comparison)->subvalues_col);


    return result;
}

Prototype Prototype::search(string tag) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_map != nullptr) {
        for( auto const& [index, prototype] : *subvalues_map ) {
            result.merge(*prototype->search(tag).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(tag).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.subvalues_col->push_back(_search(tag));

    return result;
}

void Prototype::initialize_col() {
    this->subvalues_col = new Table::Column();

}

void Prototype::_merge(map<string, Prototype*>& val) {

}

void Prototype::_merge_subprototype(string name, Prototype *foreign, E_MERGE_MODE mode) {
    switch (mode) {
        case E_MERGE_MODE::AVERAGE: {
            switch (foreign->etype) {
                case ETYPE::FLOAT: {
                    auto val = ((typed_Prototype<float>*)(*subvalues_map)[name])->value;
                    ((typed_Prototype<float>*)(*subvalues_map)[name])->value = ( val + ((typed_Prototype<float>*)foreign)->value ) / 2;
                    break;
                }
                case ETYPE::INT: {
                    auto val = ((typed_Prototype<int>*)(*subvalues_map)[name])->value;
                    ((typed_Prototype<int>*)(*subvalues_map)[name])->value = ( val + ((typed_Prototype<float>*)foreign)->value ) / 2;
                    break;
                }
            }
            break;
        }
        case E_MERGE_MODE::LEFT_INNER: {
            if ((*subvalues_map)[name]->subvalues_col != nullptr) {
                (*subvalues_map)[name]->merge(*foreign->subvalues_col);
            }
            else {
                (*subvalues_map)[name]->initialize_col();
                (*subvalues_map)[name]->merge(*foreign->subvalues_col);
            }
            break;
        }
        case E_MERGE_MODE::RIGHT_INNER: {
            switch (foreign->etype) {
                case ETYPE::FLOAT: {
                    ((typed_Prototype<float>*)(*subvalues_map)[name])->value = ((typed_Prototype<float>*)foreign)->value;
                    break;
                }
                case ETYPE::INT: {
                    ((typed_Prototype<int>*)(*subvalues_map)[name])->value = ((typed_Prototype<int>*)foreign)->value;
                    break;
                }
                case ETYPE::STRING: {
                    ((typed_Prototype<string>*)(*subvalues_map)[name])->value = ((typed_Prototype<string>*)foreign)->value;
                    break;
                }


            }
            break;
        }
    }

    if (foreign->subvalues_map != nullptr) {
        if ((*subvalues_map)[name]->subvalues_map == nullptr)
            initialize_map();

        (*subvalues_map)[name]->merge(*foreign->subvalues_map, mode);
    }
}

void Prototype::merge(Table::Column& val) {
    if (this->subvalues_col != nullptr)
        this->subvalues_col->insert( (*subvalues_col).end(), val.begin(), val.end() );
    else if (this->parent != nullptr)
        this->parent->merge(val);
    else {
        // Do nothing
    }

}

void Prototype::merge(map<string, Prototype*>& foreign, E_MERGE_MODE mode) {
    if (this->subvalues_map != nullptr) {
        for (auto const&[name, foreign_subprototype] : foreign) {
            if (mode != E_MERGE_MODE::OUTER &&
            this->subvalues_map->find(name) != this->subvalues_map->end()) {
                if ((*subvalues_map)[name]->etype == foreign_subprototype->etype) {
                    this->_merge_subprototype(name, foreign_subprototype, mode);
                }
            }
            else {
                (*this->subvalues_map)[name] = foreign_subprototype;
            }
        }
    }
    else if (this->parent != nullptr) {
        this->parent->merge(foreign, mode);
    }
    else {
        // Do nothing
    }

}

Prototype Prototype::search(Prototype &query) {
    Prototype result;
    result.initialize_col();
    if (query.subvalues_col != nullptr) {
        for (auto *val : *(query.subvalues_col)) {


            //   Use val->comparison to and val->type to search() for the value
            //   ([Post mortem NOTE: uses val->comparison and val->type via the has() function])

            // Search [#person brother: [name: "Ryan"]] // DONE
            // Search(person).foreach.has(brother: { name: "Ryan" }) // DONE

            // people = search(person) // DONE TODO: TEST
            // foreach(person in people): // DONE TODO: TEST
            //          append person to result if person.has: // DONE TODO: TEST
            //              brother: // TODO: TEST
            //                  name: "Ryan" // TODO: TEST

            if (val->has(&query))
                result.subvalues_col->push_back(val);
        }

    }
}

// TODO: rigorously test the has function
bool Prototype::has(Prototype *query) {
    // def has(prototype):
    //     for each property in prototype, if no match in this return false
    //     for each property match in prototype
    //          if not match.has(property) return false
    //     default: return true


    if (this->subvalues_map != nullptr) {
        for (auto const&[name, prototype] : *query->subvalues_map) {
            if (!_has(name, prototype, *prototype->comparison)) {
                return false;
            }
        }

        return true;
    }
}

bool Prototype::_has(string query_name, Prototype *query, Comparator comparison) {
    if(subvalues_map->find(query_name) != subvalues_map->end()) {
        auto* subprototype = (*subvalues_map)[query_name];

        if (query->etype != ETYPE::NONE) {
            if (query->etype == subprototype->etype) {
                if (comparison(query, subprototype)) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        if (query->subvalues_map != nullptr && subprototype->subvalues_map != nullptr) {
            for (auto const&[name, query_subprototype] : *query->subvalues_map) {
                if (subprototype->_has(name, query_subprototype, *query_subprototype->comparison)) {
                }
                else {
                    return false;
                }
            }
        }
    }

}

Prototype &Prototype::tag(string tag) {
    if (subvalues_map != nullptr) {
        if (subvalues_map->find("tags") != subvalues_map->end()) {
            auto tags = *(*subvalues_map)["tags"]->subvalues_col;
            if (std::find_if(tags.begin(), tags.end(), isValue(0)) == tags.end()) {
                tags.append(tag);
            }
        }
    }
    else {
        initialize_map();

        return this->tag(tag);
    }
}

bool Prototype::has_tag(string tag) {
    if (subvalues_map->find("tags") != subvalues_map->end()) {
        auto tags = *(*subvalues_map)["tags"]->subvalues_col;
        return std::find_if(tags.begin(), tags.end(), isValue(tag)) != tags.end();
    }
    else {
        return false;
    }
}

void Prototype::initialize_map() {
    subvalues_map = new map<string, Prototype*>();
}

Prototype *Prototype::_search(string tag) {
    if (has_tag(tag))
        return this;
    else {
        return nullptr;
    }
}
