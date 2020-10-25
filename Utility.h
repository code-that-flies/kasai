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
    STRING, INT, FLOAT, BOOL,           // Simple types               // Implemented!
    VEC, MATRIX, IMAGE, AUDIO, VIDEO,   // Complex types             // TODO: implement complex types
    MEMBER_FUNCTION,                    // Code types               // TODO: implement code types
    JSON, XML, KASAI,                   // Serialization types     // TODO: implement serialization types
    NONE = 0
};

enum E_MERGE_MODE {
    LEFT_INNER, RIGHT_INNER, OUTER, AVERAGE
};

typedef bool (*Comparator)(Prototype*, Prototype*);
typedef bool (*StrComparator)(Prototype*, string);
typedef bool (*IntComparator)(Prototype*, int);
typedef bool (*FloatComparator)(Prototype*, float);
typedef bool (*BoolComparator)(Prototype*, bool);
typedef Prototype (*MemberFunction)(Prototype*, Prototype*);
typedef Prototype& (*event_watcher)(const Prototype*);

Prototype* Parse(string type, string command);

#endif //KASAI_UTILITY_H
