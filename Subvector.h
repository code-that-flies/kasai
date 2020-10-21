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
#include "TUtil.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

template <class T>
class Subvector : public TUtil<int>::Range, public IReversible {
public:
    int layer;
    vector<Subvector<T>*> subvectors;
    vector<string> tags;
    static const bool isClip = false;

    Subvector(int start, int end);

    ~Subvector();

    void new_layer();

    bool _push_back(int start, int end, vector<string> tags);
    bool push_back(int start, int end, vector<string> tags);

    void render(vector<vector<T>> * result, const vector<T>& raw, string tag = "");

    void render_with_tags(vector<pair<string, T>> *result, const vector<T>& raw);

    // Produces the reverse of render, reproducing a raw
    // but using the given replacements instead of the original matches
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
void Subvector<T>::new_layer() {
    layer++;
    for (Subvector* subvector : subvectors) {
        subvector->new_layer();
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
            if (!val->tags.empty() && !TUtil<string>::Intersects(tags, val->tags)) {
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
void Subvector<T>::render(vector<vector<T>> *result, const vector<T>& raw, string tag) {

    if (tag != "") {
        for (Subvector* subarray : subvectors) {
            subarray->render(result, raw);
        }

        if (subvectors.empty() && layer < 2 && GetTag() == tag) {
            vector<T> subresult;
            TUtil<T>::Slice(raw, subresult, this->min, this->max - this->min);
            result->push_back(subresult);

        }
    }
    else {
        for (Subvector* subarray : subvectors) {
            subarray->render(result, raw);
        }

        if (subvectors.empty() && layer < 2 && TUtil<string>::Has(this->tags, tag)) {
            vector<T> subresult;
            TUtil<T>::Slice(raw, subresult, this->min, this->max - this->min);
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
            TUtil<T>::Merge(result, current->Reverse(toReplaceWith, raw, prevSubvectorEnd));
        }

        return result;
    }
    else if (subvectors.empty()) {
        prevSubvectorEnd = this->max;
        if (_prevSubvectorEnd > 0)
            return TUtil<T>::Merge(TUtil<T>::Slice(raw, _prevSubvectorEnd, this->min - _prevSubvectorEnd), toReplaceWith);
        else
            return TUtil<T>::Merge(TUtil<T>::Slice(raw, 0, this->min), toReplaceWith);
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
void Subvector<T>::render_with_tags(vector<pair<string, T>> *result, const vector<T>& raw) {
    for (Subvector* subarray : subvectors) {
        subarray->render_with_tags(result, raw);
    }

    auto tag = GetTag(0); // TODO: use a tag class so that the tags are not lost
    if (tag != "") {
        if (subvectors.empty() && layer < 2) {
            vector<T> array;
            result->push_back(pair(tag, TUtil<T>::Slice(raw, array, this->min, this->max - this->min)));
        }
    }
}

#endif //KASAI_SUBVECTOR_H
