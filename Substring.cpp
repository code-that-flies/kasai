//
// Created by Nathaniel Blair on 5/10/20.
//

#include "Substring.h"

Substring::Substring(int start, int end) {
    this->start = start;
    this->end = end;
    layer = 0;
}

Substring::~Substring() {
    for (auto val : substrings) {
        delete val;
    }
}

void Substring::new_layer() {
    layer++;
    for (Substring* substring : substrings) {
        substring->new_layer();
    }
}

bool Substring::_push_back(int start, int end) {
    if (end > this->end) {
        if (isClip)
            end = this->end;
        else
            return false;

    }
    auto result = new Substring(start, end);
    substrings.push_back(result);
    return true;
}

bool Substring::push_back(int start, int end) {
    auto result = new Substring(start, end);

    if (layer == 1) {
       _push_back(start, end);
    }
    else if (layer > 1) {
        for (auto val : substrings) {
            if (!val->tags.empty()) {
                if (val->Contains(start) || val->Contains(end)) {
                    return false;
                }
                else {
                    if (layer == 2) {
                        _push_back(start, end);
                    }
                }
            }

            if (val->Contains(start) && val->Contains(end)) {
                if (val->end < end) {
                    if (isClip)
                        end = val->end;
                    else
                        return false;
                }
                return val->push_back(start, end);
            }
        }
    }
    else {
        return false;     // Do nothing
    }

    return false;
}

void Substring::render(vector<string> *result, string *raw) {
    for (Substring* substring : substrings) {
        substring->render(result, raw);
    }

    if (substrings.empty() && layer < 2)
        result->push_back(raw->substr(start, end - start));
}

string Substring::_replace_all(string toReplaceWith, string *raw, int &prevSubstringEnd) {
    auto _prevSubstringEnd = prevSubstringEnd;
    if (layer > 1) {
        string result = "";
        for (int index = 0; index < substrings.size(); index++) {
            auto current = substrings[index];
            result += current->_replace_all(toReplaceWith, raw, prevSubstringEnd);
        }

        return result;
    }
    else if (substrings.empty()) {
        prevSubstringEnd = end;
        if (_prevSubstringEnd > 0)
            return (*raw).substr(_prevSubstringEnd, start - _prevSubstringEnd) + toReplaceWith;
        else
            return (*raw).substr(0, start) + toReplaceWith;
    }
    else {
        return "";
    }

}

bool Substring::Contains(int index) {
    return index >= start && index <= end;
}

void Substring::AddTag(string tag) {
    tags.push_back(tag);
}

void Substring::SetTag(string tag) {
    if (tags.empty())
        tags.push_back(tag);
}
