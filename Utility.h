//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_UTILITY_H
#define KASAI_UTILITY_H

#include <vector>
#include <string>
#include <map>

using std::string;
using std::vector;
using std::map;

class Prototype;

enum class ETYPE {
    STRING, INT, FLOAT, BOOL, MEMBER_FUNCTION, NONE = 0
};

enum E_MERGE_MODE {
    LEFT_INNER, RIGHT_INNER, OUTER, AVERAGE
};

typedef bool (*Comparator)(Prototype*, Prototype*);
typedef Prototype (*MemberFunction)(Prototype*, Prototype*);
typedef Prototype& (*event_watcher)(const Prototype*);
#endif //KASAI_UTILITY_H
