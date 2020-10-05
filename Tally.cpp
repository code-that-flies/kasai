//
// Created by Nathaniel Blair on 5/10/20.
//

#include "Tally.h"

Tally::Tally(string &input) {
    Initialize();

    for (int index = 0; index < input.size(); index++) {
        if (this->find(input.at(index)) != this->end()) {
            (*this)[input.at(index)]++;
        }
        else {
            (*this)[input.at(index)] = 1;
        }
    }
}

string Tally::Report() {
    string result = "";
    bool toContinue = false;
    for( auto const& [character, count] : *this ) {
        for (auto const& [match, name] : group_names) {
            if (match(character, false)) {
                result += "Number of " + name + " (" + string(1, character) + ") is " + std::to_string(count) + "\n";
                toContinue = true;
                break;
            }
        }
        if (toContinue) {
            toContinue = false;
            continue;
        }
        else {
            if (names.find(character) != names.end()) {
                result += string("\n") + "Number of " + names[character] + " is " + std::to_string(count) + "\n";
                continue;
            }
        }
        result += "Number of " + string(1, character) + " is " + std::to_string(count); + "\n";
    }

    return result;
}

string Tally::Name(char character, bool details) {
    string result = "";
    for (auto const& [match, name] : group_names) {
        if (match(character, false)) {
            result += name;

            if (details)
                result += "(" + string(1, character) + ")";

            return result;
        }
    }

    if (names.find(character) != names.end()) {
        result += names[character];
        return result;
    }

    return string(1, character);
}

int Tally::Get(string name) {
    int result = 0;

    if (reverse_names.find(name) != reverse_names.end()) {
        return (*this)[reverse_names[name]];
    }
    else {
        for (auto const& [match, currentName] : group_names) {
            if (name == currentName) {
                for (auto const& [character, count] : *this) {
                    if (match(character, false)) {
                        result += count;
                    }
                }
            }
        }
    }

    return result;
}

void Tally::Initialize() {
    push_back('\n', "Enter");
    push_back(' ', "Space");
    push_back('\t', "Tab");
    push_back('\r', "Carriage Return");
    group_names[Pattern::MatchLetter] = "Letter";
    group_names[Pattern::MatchNumber] = "Number";
}

void Tally::push_back(char character, string name) {
    names[character] = name;
    reverse_names[name] = character;
}
