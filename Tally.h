//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_TALLY_H
#define KASAI_TALLY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Pattern.h"

using std::string;
using std::vector;
using std::map;

typedef bool (*CharacterFilter)(char, bool);
class Tally : public map<char, int> {
public:
    map<char, string> names;
    map<string, char> reverse_names;
    map<CharacterFilter, string> group_names;

    Tally(string& input);

    string Report();

    string Name(char character, bool details = false);

    int Get(string name);

    void Initialize();

    void push_back(char character, string name);

};



#endif //KASAI_TALLY_H
