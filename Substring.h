//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_SUBSTRING_H
#define KASAI_SUBSTRING_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using std::string;
using std::vector;
using std::map;
using std::pair;

class Substring {
public:
    int start;
    int end;
    int layer;
    vector<Substring*> substrings;
    vector<string> tags;
    static const bool isClip = false;

    Substring(int start, int end);

    ~Substring();

    void new_layer();

    bool _push_back(int start, int end, string tag = "");
    bool push_back(int start, int end, string tag = "");

    void render(vector<string> * result, string * raw, string tag = "");

    void render_with_tags(vector<pair<string, string>> * result, string * raw);

    string _replace_all(string toReplaceWith, string * raw, int& prevSubstringEnd);

    bool Contains(int index);

    string GetTag(int index=0);

    void AddTag(string tag);

    void SetTag(string tag);
};



#endif //KASAI_SUBSTRING_H
