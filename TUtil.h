//
// Created by Nathaniel Blair on 20/10/20.
//

#ifndef KASAI_TUTIL_H
#define KASAI_TUTIL_H

#include "Utility.h"
#include "IReversible.h"
#include <iterator>

template <class T>
class TUtil {
public:
    static void Slice(const vector<T> raw, vector<T>& result, unsigned int start, unsigned int length);

    static void Copy(const vector<T> raw, vector<T>& result);

    static void Merge(vector<T>& result, vector<T>& input);

    static string ToString(const vector<T>& value);

    static vector<char> FromString(const string& input);

    class Range : public IReversible {
    public:
        T min;
        T max;
        bool relative;
        T defaultValue;

        Range(T min, T max, bool relative, T defaultValue);

        Range();

        bool In(T queryValue, T rawValue);

        // Returns the reverse of In
        // ( AKA a 'built' [default] value as *output* )
        T Reverse();
    };
};


template<class T>
void TUtil<T>::Slice(const vector<T> raw, vector<T>& result, unsigned int start, unsigned int length) {
    std::copy(std::begin(raw) + start, std::begin(raw) + start + length, std::begin(result));
}

template<class T>
void TUtil<T>::Copy(const vector<T> raw, vector<T>& result) {
    std::copy(std::begin(raw), std::begin(raw), std::begin(result));

}

template<class T>
void TUtil<T>::Merge(vector<T> &result, vector<T> &input) {
    result.insert(result.end(), input.begin(), input.end());
}

template<class T>
vector<char> TUtil<T>::FromString(const string &input) {
    return vector<char>(input.begin(), input.end());
}

template<class T>
string TUtil<T>::ToString(const vector<T> &value) {
    return string(value.begin(), value.end());
}

template<class T>
bool TUtil<T>::Range::In(T queryValue, T rawValue) {
    if (relative) {
        if ((queryValue - min) <= rawValue && (max + queryValue) >= rawValue) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (rawValue >= min && rawValue <= max) {
            return true;
        }
        else {
            return false;
        }
    }
}

template<class T>
TUtil<T>::Range::Range(T min, T max, bool relative, T defaultValue)
        : min(min), max(max), relative(relative), defaultValue(defaultValue) {
}

template<class T>
T TUtil<T>::Range::Reverse() {
    return defaultValue;
}

template<class T>
TUtil<T>::Range::Range() : relative(false) {
}


#endif //KASAI_TUTIL_H
