//
// Created by Nathaniel Blair on 27/9/20.
//

#include "Prototype.h"
#include "TypedPrototype.h"
#include "Util.h"




Prototype::Prototype() {
}

string Prototype::to_string(int indentation) {
    return "";
}

void Prototype::append(string name, Prototype *value) {
    (*subvalues_row)[name] = value;
    value->parent = this;
    value->etype = ETYPE::NONE;
}

Prototype::~Prototype() {
    if (subvalues_row != nullptr) {
        for( auto const& [index, val] : *subvalues_row ) {
            delete val;
        }
        delete subvalues_row;
    }
    if (subvalues_col != nullptr) {
        for( auto const& val : *subvalues_col ) {
            delete val;
        }
        delete subvalues_col;
    }
}

Prototype *Prototype::_search(string name, ETYPE type, Comparator comparison) {
    if (subvalues_row->find(name) != subvalues_row->end()) {
        if ((*subvalues_row)[name]->etype == type)
            return (*subvalues_row)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
}

Prototype *Prototype::_search(string name, int value, IntComparator comparison) {
    if (subvalues_row->find(name) != subvalues_row->end()) {
        if ((*subvalues_row)[name]->etype == ETYPE::INT)
            return (*subvalues_row)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
    return nullptr;
}

Prototype *Prototype::_search(string name, bool value, BoolComparator comparison) {
    if (subvalues_row->find(name) != subvalues_row->end()) {
        if ((*subvalues_row)[name]->etype == ETYPE::INT)
            return (*subvalues_row)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
    return nullptr;
}

Prototype *Prototype::_search(string name, float value, FloatComparator comparison) {
    if (subvalues_row->find(name) != subvalues_row->end()) {
        if ((*subvalues_row)[name]->etype == ETYPE::FLOAT)
            return (*subvalues_row)[name];
        else
            return nullptr;
    }
    else {
        return nullptr;
    }
}

Prototype *Prototype::_search(string name, string value, StrComparator comparison) {
    if (subvalues_row->find(name) != subvalues_row->end()) {
        if ((*subvalues_row)[name]->etype == ETYPE::STRING)
            return (*subvalues_row)[name];
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



Prototype Prototype::search(string name, ETYPE type, Comparator comparison, int& searchDepth) {
    auto result = Prototype();
    result.initialize_col();
    searchDepth++;
    if (this->subvalues_row != nullptr) {
        for( auto const& [index, prototype] : *subvalues_row ) {
            result.merge(*prototype->search(name, type, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, type, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, type, comparison)->subvalues_col);


    return result;
}

Prototype Prototype::search(string name, bool value, BoolComparator comparison, int &searchDepth) {
    auto result = Prototype();
    result.initialize_col();
    searchDepth++;
    if (this->subvalues_row != nullptr) {
        for( auto const& [index, prototype] : *subvalues_row ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value, comparison)->subvalues_col);


    return result;
}



Prototype Prototype::search(string name, int value, IntComparator comparison, int& searchDepth) {
    auto result = Prototype();
    result.initialize_col();
    searchDepth++;
    if (this->subvalues_row != nullptr) {
        for( auto const& [index, prototype] : *subvalues_row ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value, comparison)->subvalues_col);


    return result;
}
Prototype Prototype::search(string name, float value, FloatComparator comparison, int& searchDepth) {
    auto result = Prototype();
    result.initialize_col();
    searchDepth++;
    if (this->subvalues_row != nullptr) {
        for( auto const& [index, prototype] : *subvalues_row ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value, comparison)->subvalues_col);


    return result;
}



Prototype Prototype::search(string name, string value, StrComparator comparison, int& searchDepth) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_row != nullptr) {
        for( auto const& [index, prototype] : *subvalues_row ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }
    if (this->subvalues_col != nullptr) {
        for( auto const& prototype : *subvalues_col ) {
            result.merge(*prototype->search(name, value, comparison, searchDepth).subvalues_col); // TODO: test this statement's functionality, it MAY not work!
        }
    }

    result.merge(*_search(name, value, comparison)->subvalues_col);


    return result;
}

Prototype Prototype::search(string tag) {
    auto result = Prototype();
    result.initialize_col();
    if (this->subvalues_row != nullptr) {
        for( auto const& [index, prototype] : *subvalues_row ) {
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
                    auto val = ((TypedPrototype<float>*)(*subvalues_row)[name])->value;
                    ((TypedPrototype<float>*)(*subvalues_row)[name])->value = (val + ((TypedPrototype<float>*)foreign)->value ) / 2;
                    break;
                }
                case ETYPE::INT: {
                    auto val = ((TypedPrototype<int>*)(*subvalues_row)[name])->value;
                    ((TypedPrototype<int>*)(*subvalues_row)[name])->value = (val + ((TypedPrototype<float>*)foreign)->value ) / 2;
                    break;
                }
            }
            break;
        }
        case E_MERGE_MODE::LEFT_INNER: {
            if ((*subvalues_row)[name]->subvalues_col != nullptr) {
                (*subvalues_row)[name]->merge(*foreign->subvalues_col);
            }
            else {
                (*subvalues_row)[name]->initialize_col();
                (*subvalues_row)[name]->merge(*foreign->subvalues_col);
            }
            break;
        }
        case E_MERGE_MODE::RIGHT_INNER: {
            switch (foreign->etype) {
                case ETYPE::FLOAT: {
                    ((TypedPrototype<float>*)(*subvalues_row)[name])->value = ((TypedPrototype<float>*)foreign)->value;
                    break;
                }
                case ETYPE::INT: {
                    ((TypedPrototype<int>*)(*subvalues_row)[name])->value = ((TypedPrototype<int>*)foreign)->value;
                    break;
                }
                case ETYPE::STRING: {
                    ((TypedPrototype<string>*)(*subvalues_row)[name])->value = ((TypedPrototype<string>*)foreign)->value;
                    break;
                }


            }
            break;
        }
    }

    if (foreign->subvalues_row != nullptr) {
        if ((*subvalues_row)[name]->subvalues_row == nullptr)
            initialize_map();

        (*subvalues_row)[name]->merge(*foreign->subvalues_row, mode);
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
    if (this->subvalues_row != nullptr) {
        for (auto const&[name, foreign_subprototype] : foreign) {
            if (mode != E_MERGE_MODE::OUTER &&
                this->subvalues_row->find(name) != this->subvalues_row->end()) {
                if ((*subvalues_row)[name]->etype == foreign_subprototype->etype) {
                    this->_merge_subprototype(name, foreign_subprototype, mode);
                }
            }
            else {
                (*this->subvalues_row)[name] = foreign_subprototype;
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
    if (subvalues_col != nullptr) {
        for (auto *val : *(subvalues_col)) {


            //   Use val->comparison to and val->type to search() for the value
            //   ([Post mortem NOTE: uses val->comparison and val->type via the has() function])

            // Search [#person brother: [name: "Ryan"]] // DONE
            // Search(person).foreach.has(brother: { name: "Ryan" }) // DONE

            // people = search(person) // DONE (separate) TODO: TEST
            // foreach(person in people): // DONE TODO: TEST
            //          append person to result if person.has: // DONE TODO: TEST
            //              brother: // TODO: TEST
            //                  name: "Ryan" // TODO: TEST


            if (val->has(&query))
                result.subvalues_col->push_back(val);
        }
    }

    if (subvalues_row != nullptr) {
        for (auto const&[name, prototype] : *subvalues_row) {
            if (prototype->has(&query)) {
                result.subvalues_col->push_back(prototype);
            }
        }
    }

    return result;
}

// TODO: rigorously test the has function
bool Prototype::has(Prototype *query) {
    // def has(prototype):
    //     for each property in prototype, if no match in this return false
    //     for each property match in prototype
    //          if not match.has(property) return false
    //     default: return true


    if (this->subvalues_row != nullptr) {
        for (auto const&[name, prototype] : *query->subvalues_row) {
            if (!_has(name, prototype, *prototype->comparison)) {
                return false;
            }
        }

        return true;
    }
}

bool Prototype::_has(string query_name, Prototype *query, Comparator comparison) {
    if(subvalues_row->find(query_name) != subvalues_row->end()) {
        auto* subprototype = (*subvalues_row)[query_name];

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

        if (query->subvalues_row != nullptr && subprototype->subvalues_row != nullptr) {
            for (auto const&[name, query_subprototype] : *query->subvalues_row) {
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
    if (subvalues_row != nullptr) {
        if (subvalues_row->find("tags") != subvalues_row->end()) {
            auto tags = *(*subvalues_row)["tags"]->subvalues_col;
            if (std::find_if(tags.begin(), tags.end(), Util<string>::isValue(tag)) == tags.end()) {
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
    if (subvalues_row->find("tags") != subvalues_row->end()) {
        auto tags = *(*subvalues_row)["tags"]->subvalues_col;
        return std::find_if(tags.begin(), tags.end(), Util<string>::isValue(tag)) != tags.end();
    }
    else {
        return false;
    }
}

void Prototype::initialize_map() {
    subvalues_row = new Table::Row();
}

Prototype *Prototype::_search(string tag) {
    if (has_tag(tag))
        return this;
    else {
        return nullptr;
    }
}
