//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_SUBVECTOR_H
#define KASAI_SUBVECTOR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "Util.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

template <class T>
class Subvector : public Util<int>::Range, public IReversible {
public:
    int layer;
    vector<Subvector<T>*> subvectors;
    vector<string> tags;
    static const bool isClip = false;

    Subvector();

    Subvector(int start, int end);

    ~Subvector();

    void NewLayer();

    bool _push_back(int start, int end, vector<string> tags);
    bool push_back(int start, int end, vector<string> tags);

    void Render(vector<vector<T>> * result, const vector<T>& raw, string tag = "");

    void RenderWithTags(vector<pair<string, T>> *result, const vector<T>& raw);
    void RenderWithTags(vector<string> tags, vector<pair<string, T>> *result, const vector<T>& raw);

    void RenderAsStringsWithTags(vector<string> tags, vector<pair<string, T>> *result, const vector<T>& raw);
    void RenderAsStringsWithTags(vector<pair<string, T>> *result, const vector<T>& raw);

    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    vector<T> Reverse(vector<T> toReplaceWith, vector<T> line, int &prevSubstringEnd);
    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith
    // but filtered to be only those with tags that intersect with the parameter 'tags' TODO: test thoroughly
    vector<T> Reverse(vector<string> tags, vector<T> toReplaceWith, vector<T> line, int &prevSubstringEnd);
    // Replaces the subvector's highlighted contents and replaces it with toReplaceWith TODO: test thoroughly
    vector<T> Reverse(vector<T> toReplaceWith, const vector<T>& raw, int& prevSubvectorEnd);

    string GetTag(int index=0);

    void AddTag(string tag);

    void SetTag(string tag);
};

template<class T>
Subvector<T>::Subvector(int start, int end) {
    this->min = start;
    this->max = end;
    layer = 0;
}

template<class T>
Subvector<T>::~Subvector() {
    for (auto val : subvectors) {
        delete val;
    }
}

template<class T>
bool Subvector<T>::_push_back(int start, int end, vector<string> tags) {
    if (end > this->max) {
        if (isClip)
            end = this->max;
        else
            return false;

    }
    auto result = new Subvector(start, end);
    subvectors.push_back(result);
    result->tags = tags;
    return true;
}

template<class T>
bool Subvector<T>::push_back(int start, int end, vector<string> tags) {
    if (layer == 1) {
        _push_back(start, end, tags);
    }
    else if (layer > 1) {
        for (auto val : subvectors) {
            if (!val->tags.empty() && !Util<string>::Intersects(tags, val->tags)) {
                if (val->In(start) || val->In(end)) {
                    return false;
                }
            }

            if (val->In(start) && val->In(end)) {
                if (val->max < end) {
                    if (isClip)
                        end = val->max;
                    else
                        return false;
                }
                return val->push_back(start, end, tags);
            }
        }
    }
    else {
        return false;     // Do nothing
    }

    return false;
}

template<class T>
void Subvector<T>::Render(vector<vector<T>> *result, const vector<T>& raw, string tag) {

    if (tag != "") {
        for (Subvector* subarray : subvectors) {
            subarray->Render(result, raw);
        }

        if (subvectors.empty() && layer < 2 && GetTag() == tag) {
            vector<T> subresult;
            Util<T>::Slice(raw, subresult, this->min, this->max - this->min);
            result->push_back(subresult);

        }
    }
    else {
        for (Subvector* subarray : subvectors) {
            subarray->Render(result, raw);
        }

        if (subvectors.empty() && layer < 2 && Util<string>::Has(this->tags, tag)) {
            vector<T> subresult;
            Util<T>::Slice(raw, subresult, this->min, this->max - this->min);
            result->push_back(subresult);
        }
    }
}

template<class T>
vector<T> Subvector<T>::Reverse(vector<T> toReplaceWith, const vector<T>& raw, int &prevSubvectorEnd) {
    auto _prevSubvectorEnd = prevSubvectorEnd;
    if (layer > 1) {
        vector<T> result;
        for (int index = 0; index < subvectors.size(); index++) {
            auto current = subvectors[index];
            Util<T>::Merge(result, current->Reverse(toReplaceWith, raw, prevSubvectorEnd));
        }

        return result;
    }
    else if (subvectors.empty()) {
        prevSubvectorEnd = this->max;
        if (_prevSubvectorEnd > 0)
            return Util<T>::Merge(Util<T>::Slice(raw, _prevSubvectorEnd, this->min - _prevSubvectorEnd), toReplaceWith);
        else
            return Util<T>::Merge(Util<T>::Slice(raw, 0, this->min), toReplaceWith);
    }
    else {
        return vector<T>();
    }

}

template<class T>
void Subvector<T>::AddTag(string tag) {
    tags.push_back(tag);
}

template<class T>
void Subvector<T>::SetTag(string tag) {
    if (tags.empty() && !tag.empty())
        tags.push_back(tag);
}

template<class T>
string Subvector<T>::GetTag(int index) {
    if (tags.empty())
        return "";
    else {
        return tags[index];
    }
}

template<class T>
void Subvector<T>::RenderWithTags(vector<pair<string, T>> *result, const vector<T>& raw) {
    for (Subvector* subarray : subvectors) {
        subarray->RenderWithTags(result, raw);
    }

    auto tag = GetTag(0); // TODO: use a tag class so that the tags are not lost
    if (tag != "") {
        if (subvectors.empty() && layer < 2) {
            vector<T> array;
            result->push_back(pair(this->tags, Util<T>::Slice(raw, array, this->min, this->max - this->min)));
        }
    }
}

template<class T>
void Subvector<T>::RenderWithTags(vector<string> tags, vector<pair<string, T>> *result, const vector<T>& raw) {
    for (Subvector* subarray : subvectors) {
        subarray->RenderWithTags(tags, result, raw);
    }

    // TODO: use a tag class so that the tags are not lost
    if (!this->tags.empty()) {
        if (Util<string>::Intersects(tags, this->tags)) {
            if (subvectors.empty() && layer < 2) {
                vector<T> array;
                result->push_back(pair(this->tags, Util<T>::Slice(raw, array, this->min, this->max - this->min)));
            }
        }
    }
}

template<class T>
void Subvector<T>::RenderAsStringsWithTags(vector<pair<string, T>> *result, const vector<T>& raw) {
    for (Subvector* subarray : subvectors) {
        subarray->RenderWithTags(result, raw);
    }

    auto tag = GetTag(0); // TODO: use a tag class so that the tags are not lost

    // TODO: optimize (this whole Util<T> class for usage with strings
    if (tag != "") {
        if (subvectors.empty() && layer < 2) {
            vector<T> array;
            result->push_back(pair(this->tags, Util<T>::ToString(Util<T>::Slice(raw, array, this->min, this->max - this->min))));
        }
    }
}

template<class T>
void Subvector<T>::RenderAsStringsWithTags(vector<string> tags, vector<pair<string, T>> *result, const vector<T>& raw) {
    for (Subvector* subarray : subvectors) {
        subarray->RenderWithTags(tags, result, raw);
    }

    // TODO: use a tag class so that the tags are not lost
    // TODO: optimize (this whole Util<T> class for usage with strings
    if (!this->tags.empty()) {
        if (Util<string>::Intersects(tags, this->tags)) {
            if (subvectors.empty() && layer < 2) {
                vector<T> array;
                result->push_back(pair(this->tags, Util<T>::ToString(Util<T>::Slice(raw, array, this->min, this->max - this->min))));
            }
        }
    }
}

template<class T>
vector<T> Subvector<T>::Reverse(vector<T> toReplaceWith, vector<T> line, int &prevSubstringEnd) {
    auto _prevSubstringEnd = prevSubstringEnd;
    if (layer > 1) {
        vector<T> result;
        for (int index = 0; index < subvectors.size(); index++) {
            auto current = subvectors[index];
            Util<T>::Merge(result, current->_replace_all(toReplaceWith, line, prevSubstringEnd));
        }

        return result;
    } else if (subvectors.empty()) {
        prevSubstringEnd = max;

        auto anchor = std::max(0, _prevSubstringEnd);

        return Util<T>::Merge(Util<T>::Slice(line, anchor, min - anchor), toReplaceWith);
    } else {
        return vector<T>();
    }
}

template<class T>
vector<T> Subvector<T>::Reverse(vector<string> tags, vector<T> toReplaceWith, vector<T> line, int &prevSubstringEnd) {
    auto _prevSubstringEnd = prevSubstringEnd;
    if (layer > 1) {
        vector<T> result;
        for (int index = 0; index < subvectors.size(); index++) {
            auto current = subvectors[index];
            Util<T>::Merge(result, current->_replace_all(tags, toReplaceWith, line, prevSubstringEnd));
        }

        return result;
    } else if (subvectors.empty()) {
        prevSubstringEnd = max;
        auto anchor = std::max(0, _prevSubstringEnd);

        if (Util<T>::Intersects(tags, this->tags)) {
            return Util<T>::Merge(Util<T>::Slice(line, anchor, min - anchor), toReplaceWith);
        } else {
            auto result = vector<T>();
            return Util<T>::Slice(line, result, anchor, max - anchor);
        }
    } else {
        return vector<T>();
    }
}

template<class T>
void Subvector<T>::NewLayer() {
    layer++;
    for (Subvector *subvector : subvectors) {
        subvector->NewLayer();
    }
}

template<class T>
Subvector<T>::Subvector() : Util<int>::Range(0, 0, false, -1), layer(0), tags(vector<string>()) {

}

#endif //KASAI_SUBVECTOR_H
