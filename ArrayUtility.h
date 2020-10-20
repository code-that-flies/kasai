//
// Created by Nathaniel Blair on 20/10/20.
//

#ifndef KASAI_ARRAYUTILITY_H
#define KASAI_ARRAYUTILITY_H

#include "Utility.h"
#include <iterator>

template <class T>
class ArrayUtility {
public:
    static void Slice(const vector<T> raw, vector<T>& result, unsigned int start, unsigned int length);

    static void Copy(const vector<T> raw, vector<T>& result);

    static void Merge(vector<T>& result, vector<T>& input);

    static string ToString(const vector<T>& value);

    static vector<char> FromString(const string& input);

};


template<class T>
void ArrayUtility<T>::Slice(const vector<T> raw, vector<T>& result, unsigned int start, unsigned int length) {
    std::copy(std::begin(raw) + start, std::begin(raw) + start + length, std::begin(result));
}

template<class T>
void ArrayUtility<T>::Copy(const vector<T> raw, vector<T>& result) {
    std::copy(std::begin(raw), std::begin(raw), std::begin(result));

}

template<class T>
void ArrayUtility<T>::Merge(vector<T> &result, vector<T> &input) {
    result.insert(result.end(), input.begin(), input.end());
}

template<class T>
vector<char> ArrayUtility<T>::FromString(const string &input) {
    return vector<char>(input.begin(), input.end());
}

template<class T>
string ArrayUtility<T>::ToString(const vector<T> &value) {
    return string(value.begin(), value.end());
}

#endif //KASAI_ARRAYUTILITY_H
