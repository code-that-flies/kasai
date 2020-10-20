#include <iostream>
#include <string>
#include <vector>
#include "Prototype.h"
#include "Pattern.h"
#include "typed_Prototype.h"
#include "Prototype_Engine.h"

using std::string;
using std::vector;

#include "Subvector.h"
#include "Query.h"

template <typename T>
T Evaluate(T a, char char_operator, T b) {
    switch (char_operator) {
        case '*':
            return a * b;
        case '/':
            return a / b;
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '^':
            return pow(a, b);
    }
}



string subscan(vector<string> lines) {
    return "";
}

string scan(vector<string> lines) {
    for (auto index = 0; index < lines.size(); index++) {

    }
}

bool is_in(char value, string characters) {
    for (char character : characters) {
        if (character == value)
            return true;
    }

    return false;
}

Prototype * _load(string raw, char type) {
    switch (type) {
        case 'f': {
            auto* result = new typed_Prototype<float>(0, ETYPE::FLOAT);
            result->load(raw);
            return (Prototype*)result;
            break;
        }
        case 'i': {
            auto* result2 = new typed_Prototype<int>(0, ETYPE::INT);
            result2->load(raw);
            return (Prototype*)result2;
            break;
        }
    }
}

Prototype * _load(string raw) {
    auto* result = new typed_Prototype<string>("", ETYPE::STRING);
    result->load(raw);
    return (Prototype*)result;
}

Prototype * load(string raw) {

    for (auto index = raw.size() - 1; index >= 0; index--) {
        if (is_in(raw.at(index), "+-/*^")) {
            auto left = raw.substr(0, index);
            auto right = raw.substr(index + 1, raw.size() - index);


        }

        if (is_in(raw.at(index), "'\"")) {
            auto* valueA = _load(raw.substr(1, index - 1));
            return valueA;
        }

        if (is_in(raw.at(index), "if")) {
            auto* valueA = _load(raw.substr(0, index), raw.at(index));
            return valueA;
        }
    }
}
/*

int main() {
    std::cout << "Hello, World!" << std::endl;
    vector<string> lines;

    lines.push_back("\"test\"");
    lines.push_back("1.0f");
    lines.push_back("1.0");
    lines.push_back("1");
    lines.push_back("2i");

    auto* value = load(lines[1]);
    std::cout << std::to_string(((float_Prototype*)value)->value) << std::endl;

    delete value;
    return 0;
}
*/
//     typedef bool (*Matchmaker)(const vector<T>& raw, int& rawIndex, vector<T>& query, int& queryIndex, bool inverted, Range* range);

bool MatchMakkr320(const vector<char>& raw, int& rawIndex, vector<char>& query, int& queryIndex, bool inverted, TUtil<char>::Range* range) {
    return true;
}

int main() {
    // TODO: add asserts to test here
    Subvector<char> test(0, 10);
    vector<vector<char>> result;
    vector<char> chars = {'a', 'b', 'c'};
    test.render(&result, chars, "test2");

    Query<char> charSet;
    charSet.Add('c', MatchMakkr320, true, nullptr);

    for (auto item : result[0]) {
        std::cout << item;
    }
    Prototype_Engine::End();
    return 0;
}
