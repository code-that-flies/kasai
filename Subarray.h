//
// Created by Nathaniel Blair on 5/10/20.
//

#ifndef KASAI_SUBARRAY_H
#define KASAI_SUBARRAY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "ArrayUtility.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

template <class T>
class Subarray {
public:
    int start;
    int end;
    int layer;
    vector<Subarray<T>*> subarrays;
    vector<string> tags;
    static const bool isClip = false;

    Subarray(int start, int end);

    ~Subarray();

    void new_layer();

    bool _push_back(int start, int end, string tag = "");
    bool push_back(int start, int end, string tag = "");

    void render(vector<vector<T>> * result, const vector<T>& raw, string tag = "");

    void render_with_tags(vector<pair<string, T>> *result, T *raw);

    vector<T> _replace_all(T toReplaceWith[], T * raw, int& prevSubstringEnd);
    vector<T> _replace_all(vector<T> toReplaceWith, T * raw, int& prevSubstringEnd);

    bool Contains(int index);

    string GetTag(int index=0);

    void AddTag(string tag);

    void SetTag(string tag);
};


template<class T>
Subarray<T>::Subarray(int start, int end) {
    this->start = start;
    this->end = end;
    layer = 0;
}

template<class T>
Subarray<T>::~Subarray() {
    for (auto val : subarrays) {
        delete val;
    }
}

template<class T>
void Subarray<T>::new_layer() {
    layer++;
    for (Subarray* subarray : subarrays) {
        subarray->new_layer();
    }
}

template<class T>
bool Subarray<T>::_push_back(int start, int end, string tag) {
    if (end > this->end) {
        if (isClip)
            end = this->end;
        else
            return false;

    }
    auto result = new Subarray(start, end);
    subarrays.push_back(result);
    SetTag(tag);
    return true;
}

template<class T>
bool Subarray<T>::push_back(int start, int end, string tag) {
    auto result = new Subarray(start, end);

    if (layer == 1) {
        _push_back(start, end, tag);
    }
    else if (layer > 1) {
        for (auto val : subarrays) {
            if (!val->tags.empty()) {
                if (val->Contains(start) || val->Contains(end)) {
                    return false;
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

template<class T>
void Subarray<T>::render(vector<vector<T>> *result, const vector<T>& raw, string tag) {

    if (tag != "") {
        for (Subarray* subarray : subarrays) {
            subarray->render(result, raw);
        }

        if (subarrays.empty() && layer < 2 && GetTag() == tag) {
            vector<T> subresult;
            ArrayUtility<T>::Slice(raw, subresult, start, end - start);
            result->push_back(subresult);

        }
    }
    else {
        for (Subarray* subarray : subarrays) {
            subarray->render(result, raw);
        }

        if (subarrays.empty() && layer < 2 && GetTag() == tag) {

            vector<T> subresult;
            ArrayUtility<T>::Slice(raw, subresult, start, end - start);
            result->push_back(subresult);
        }
    }
}

template<class T>
vector<T> Subarray<T>::_replace_all(T toReplaceWith[], T *raw, int &prevSubstringEnd) {
    auto _prevSubstringEnd = prevSubstringEnd;
    if (layer > 1) {
        vector<T> result;
        for (int index = 0; index < subarrays.size(); index++) {
            auto current = subarrays[index];
            ArrayUtility<T>::Merge(result, current->_replace_all(toReplaceWith, raw, prevSubstringEnd));
        }

        return result;
    }
    else if (subarrays.empty()) {
        prevSubstringEnd = end;
        if (_prevSubstringEnd > 0)
            return ArrayUtility<T>::Merge(ArrayUtility<T>::Slice(*raw, _prevSubstringEnd, start - _prevSubstringEnd), toReplaceWith);
        else
            return ArrayUtility<T>::Merge(ArrayUtility<T>::Slice(raw, 0, start), toReplaceWith);
    }
    else {
        return vector<T>();
    }

}

template<class T>
vector<T> Subarray<T>::_replace_all(vector<T> toReplaceWith, T *raw, int &prevSubstringEnd) {
    auto _prevSubstringEnd = prevSubstringEnd;
    if (layer > 1) {
        vector<T> result;
        for (int index = 0; index < subarrays.size(); index++) {
            auto current = subarrays[index];
            ArrayUtility<T>::Merge(result, current->_replace_all(toReplaceWith, raw, prevSubstringEnd));
        }

        return result;
    }
    else if (subarrays.empty()) {
        prevSubstringEnd = end;
        if (_prevSubstringEnd > 0)
            return ArrayUtility<T>::Merge(ArrayUtility<T>::Slice(*raw, _prevSubstringEnd, start - _prevSubstringEnd), toReplaceWith);
        else
            return ArrayUtility<T>::Merge(ArrayUtility<T>::Slice(raw, 0, start), toReplaceWith);
    }
    else {
        return vector<T>();
    }

}

template<class T>
bool Subarray<T>::Contains(int index) {
    return index >= start && index <= end;
}

template<class T>
void Subarray<T>::AddTag(string tag) {
    tags.push_back(tag);
}

template<class T>
void Subarray<T>::SetTag(string tag) {
    if (tags.empty() && tag != "")
        tags.push_back(tag);
}

template<class T>
string Subarray<T>::GetTag(int index) {
    if (tags.empty())
        return "";
    else {
        return tags[index];
    }
}

template<class T>
void Subarray<T>::render_with_tags(vector<pair<string, T>> *result, T *raw) {
    for (Subarray* subarray : subarrays) {
        subarray->render_with_tags(result, raw);
    }

    auto tag = GetTag();
    if (tag != "") {
        if (subarrays.empty() && layer < 2) {
            T array[end - start];
            result->push_back(pair(tag, ArrayUtility<T>::Slice(raw, array, start, end - start)));
        }
    }
}






#endif //KASAI_SUBARRAY_H
